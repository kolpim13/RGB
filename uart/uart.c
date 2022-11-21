#include "uart.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"

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
    dma_channel_set_write_addr(DMA_CHANEL_UART0_TX, &uart0_hw->dr, false);
    dma_channel_set_config(DMA_CHANEL_UART0_TX, &config_tx, false);
}

void UART_init(uint32_t baud){
    gpio_set_function(UART_PIN_TX, GPIO_FUNC_UART);
    gpio_set_function(UART_PIN_RX, GPIO_FUNC_UART);

    uart_init(UART, baud);

    UART_init_dma();
}

void UART_transferAsync(const volatile uint8_t* buffer_a, uint32_t len){
    dma_channel_transfer_from_buffer_now(DMA_CHANEL_UART0_TX, buffer_a, len);
}