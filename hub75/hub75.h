#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"
#include "hardware/irq.h"

// Constant module values 
#define dma_channel_rgb111_data DREQ_PIO0_TX0         

// Function definitions
void hub75_rgb111_init(void);
void hub75_rgb111_start(void);
void hub75_rgb111_stop(void);

void hub75_rgb111_set_buffer(uint8_t *buf_pa, uint number);
uint8_t* hub75_rgb111_get_buffer(uint number);
size_t hub75_rgb111_get_activeBufferNumber(void);

void hub75_rgb111_data_dma_handler(void);
//===============================================================