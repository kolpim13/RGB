#pragma once

#define LED_PICO    25
#define LED_PICO_W  0
#define LED LED_PICO

#define LED_OSCYLOSCOP  22

/* List of DMA channels */
#define DMA_CHANEL_UART0_TX     0U
#define DMA_CHANEL_UART0_RX     1U

#include "hub75.h"

void PICO_IRQ_Set_Handlers(void);