#include "uart.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"

static volatile uint32_t len_u32 = 0;
static uint8_t buffer_a[UART_RX_LEN] __attribute__ ((aligned (4)));
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

    /*
    // RX initial channel
    dma_channel_config config_rx_init = {0};

    // RX main channel
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
    */
}

void UART_init(uint32_t baud){
    gpio_set_function(UART_PIN_TX, GPIO_FUNC_UART);
    gpio_set_function(UART_PIN_RX, GPIO_FUNC_UART);
    //gpio_set_function(UART_PIN_RTS, GPIO_FUNC)

    uart_init(UART, baud);
    //uart_set_hw_flow(UART, false, true);
    UART_init_dma();

    /* Disable FIFO mode and receiver */
    UART_HW->lcr_h &= ~(UART_UARTLCR_H_FEN_BITS);
    UART_HW->cr &= ~(UART_UARTCR_RXE_BITS);

    /* Interrupts: Receive */
    UART_HW->imsc |= UART_UARTIMSC_RXIM_BITS;
    irq_set_exclusive_handler(UART0_IRQ, (irq_handler_t)UART_IRQ_Handler);
    irq_set_enabled(UART0_IRQ, true);
}

void UART_readAsync(void){
    len_u32 = 0;
    UART_RX_State_Set(TX_RX_Busy);
    UART_HW->cr |= UART_UARTCR_RXE_BITS;
    //dma_channel_transfer_to_buffer_now(DMA_CHANEL_UART0_RX, buffer_a, UART_RX_LEN);
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
    /* Read interrupt mask */
    uint32_t mis = UART_HW->mis;

    /* If byte receive */
    if (mis & UART_UARTMIS_RXMIS_BITS){
        uint8_t symb = UART_HW->dr;

        // If 
        if (symb == UART_SYMB_END_OF_MES){
            UART_HW->cr &= ~(UART_UARTCR_RXE_BITS);
            UART_RX_State_Set(TX_RX_Ready);
            return;
        }
        buffer_a[len_u32] = UART_HW->dr;
        len_u32++;
    }
}
void UART_TX_DMA_Handler(void){}
