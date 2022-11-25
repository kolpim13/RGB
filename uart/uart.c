#include "uart.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"

static uint8_t buffer_a[UART_RX_LEN] __attribute__ ((aligned (4)));
static volatile uint32_t len_u32 = 0;
static volatile UART_TX_RX_State_e status_tx = TX_RX_Ready;
static volatile UART_TX_RX_State_e status_rx = TX_RX_Ready;

static void UART_init_dma(void){
    // TX
    dma_channel_config config_tx = {0};
    dma_channel_claim(DMA_CHANEL_UART0_TX);
    channel_config_set_read_increment(&config_tx, true);
    channel_config_set_write_increment(&config_tx, false);
    channel_config_set_dreq(&config_tx, DREQ_UART0_TX);
    channel_config_set_chain_to(&config_tx, DMA_CHANEL_UART0_TX);
    channel_config_set_transfer_data_size(&config_tx, DMA_SIZE_8);
    channel_config_set_enable(&config_tx, true);
    dma_channel_set_write_addr(DMA_CHANEL_UART0_TX, &UART_HW->dr, false);
    dma_channel_set_config(DMA_CHANEL_UART0_TX, &config_tx, false);

    // RX
    dma_channel_config config_rx = {0};
    dma_channel_claim(DMA_CHANEL_UART0_RX);
    channel_config_set_read_increment(&config_rx, false);
    channel_config_set_write_increment(&config_rx, true);
    channel_config_set_dreq(&config_rx, DREQ_UART0_RX);
    channel_config_set_chain_to(&config_rx, DMA_CHANEL_UART0_RX);
    channel_config_set_transfer_data_size(&config_rx, DMA_SIZE_8);
    channel_config_set_enable(&config_rx, true);
    dma_channel_set_read_addr(DMA_CHANEL_UART0_RX, &UART_HW->dr, false);
    dma_channel_set_config(DMA_CHANEL_UART0_RX, &config_rx, false);
}

void UART_init(uint32_t baud){
    gpio_set_function(UART_PIN_TX, GPIO_FUNC_UART);
    gpio_set_function(UART_PIN_RX, GPIO_FUNC_UART);

    uart_init(UART, baud);
    UART_init_dma();
    /* Clear FIFO mode */
    UART_HW->lcr_h &= ~(UART_UARTLCR_H_FEN_BITS);

    /* Interrupts: Receive timeout */
    UART_HW->imsc |= UART_UARTIMSC_RTIM_BITS;
    irq_set_exclusive_handler(UART0_IRQ, (irq_handler_t)UART_IRQ_Handler);
    irq_set_enabled(UART0_IRQ, true);
}

void UART_readAsync(void){
    UART_RX_State_Set(TX_RX_Busy);
    dma_channel_transfer_to_buffer_now(DMA_CHANEL_UART0_RX, buffer_a, UART_RX_LEN);
}

void UART_transferAsync(const volatile uint8_t* buffer_a, uint32_t len){
    UART_TX_State_Set(TX_RX_Busy);
    dma_channel_transfer_from_buffer_now(DMA_CHANEL_UART0_TX, buffer_a, len);
}

__always_inline UART_TX_RX_State_e UART_TX_State_Get(void) { return status_tx; }
__always_inline static void UART_TX_State_Set(UART_TX_RX_State_e state){ status_tx = state; }

__always_inline UART_TX_RX_State_e UART_RX_State_Get(void) { return status_rx; }
__always_inline static void UART_RX_State_Set(UART_TX_RX_State_e state){ status_rx = state; }

__always_inline uint8_t* UART_ReceiveBuffer_Get(void) { return &buffer_a[0]; }
__always_inline uint32_t UART_ReceiveBufferLen_Get(void) { return len_u32; }

void UART_IRQ_Handler(void){
    /* If receive timeout interrupt */
    if (UART_HW->mis & UART_UARTMIS_RXMIS_BITS){
        len_u32 = UART_RX_LEN - dma_hw->ch[DMA_CHANEL_UART0_RX].transfer_count;
        UART_RX_State_Set(TX_RX_Ready);

        UART_HW->icr |= UART_UARTICR_RTIC_BITS;
    }
}
void UART_TX_DMA_Handler(void){

}