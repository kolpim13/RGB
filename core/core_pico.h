#pragma once

/* List of DMA channels */
#define DMA_CHANEL_UART0_TX     0U
#define DMA_CHANEL_UART0_RX     1U

#include "hub75.h"

void PICO_IRQ_Set_Handlers(void);