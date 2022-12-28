#include "core_pico.h"

/* Static functions definitions */
static void IRQ_Handler_DMA_0(void);
//===============================================================

void PICO_IRQ_Set_Handlers(void){
    // Link handlers to the irq
    irq_set_exclusive_handler(DMA_IRQ_0, IRQ_Handler_DMA_0);

    // Enable all used irq interruptions
    irq_set_enabled(DMA_IRQ_0, true);    
}
//===============================================================

/* ALL DMA IRQ HANDLERS */

static void IRQ_Handler_DMA_0(void){
    uint32_t ints0 = dma_hw->ints0;

    ///hub75_rgb555_data_dma_handler();
    if (ints0 & (1U << DMA_CHANNEL_RGB111_DATA)){
        hub75_rgb111_dma_handler();
    }

    if (ints0 & (1U << DMA_CHANEL_UART0_TX)){
        UART_TX_DMA_Handler();
    }
}
//===============================================================
