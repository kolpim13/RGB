#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "hardware/pio_instructions.h"
#include "core_pico.h"

void hub75_rgb111_init(PIO pio, uint sm_data, uint sm_row, uint pin_color, uint pin_sckl, uint pin_row, uint pin_latch, float freq);
void hub75_rgb111_set_buffer(uint8_t *buf_pa, uint number);
uint8_t* hub75_rgb111_get_buffer(void);
void hub75_rgb111_data_dma_handler(void);