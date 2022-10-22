#include "core_pico.h"

/* Static functions definitions */
void IRQ_Handler_DMA_0(void);
//===============================================================

void PICO_IRQ_Set_Handlers(void){
    // Link handlers to the irq
    irq_set_exclusive_handler(DMA_IRQ_0, IRQ_Handler_DMA_0);

    // Enable all used irq interruptions
    irq_set_enabled(DMA_IRQ_0, true);    
}
//===============================================================

/* ALL IRQ HANDLERS */

void IRQ_Handler_DMA_0(void){
     /* For time measuring (oscyloscop) start */ 
    gpio_put(22, true);

    hub75_rgb555_data_dma_handler();

    /* For time measuring (oscyloscop) stop */
    gpio_put(22, false);
}
//===============================================================
