#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"
#include "hardware/irq.h"

// Constant module values 
static const uint dma_channel_rgb111_data = DREQ_PIO0_TX0;        
static const uint dma_channel_rgb111_row = DREQ_PIO0_TX1;        

// Function definitions
void hub75_init(void);
void hub75_data_dma_handler(void);
//===============================================================