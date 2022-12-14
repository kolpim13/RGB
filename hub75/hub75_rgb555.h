#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "hardware/pio_instructions.h"

#define dma_channel_rgb555_data DREQ_PIO0_TX0

void hub75_rgb555_init(PIO pio, uint sm_data, uint sm_row, uint pin_color, uint pin_sckl, uint pin_row, uint pin_latch, float freq);
void hub75_rgb555_set_buffer(uint32_t *buf_pa, uint number);
void hub75_rgb555_data_dma_handler(void);