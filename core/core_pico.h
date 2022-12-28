#pragma once

#define LED_PICO    25
#define LED_PICO_W  0
#define LED LED_PICO

#define LED_OSCYLOSCOP  28

/* List of DMA channels */
#define DMA_CHANEL_UART0_TX     0U
#define DMA_CHANEL_UART0_RX     1U
#define DMA_CHANNEL_RGB111_DATA 2U
#define DMA_CHANNEL_RGB111_ROW  3U

#include "hub75.h"
#include "uds.h"

void PICO_IRQ_Set_Handlers(void);