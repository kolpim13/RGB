#include "hub75_rgb555.h"
#include "hub75_rgb555.pio.h"

static PIO hub75_rgb555_pio = pio0;
static uint hub75_rgb555_sm_data = 0;
static uint hub75_rgb555_sm_row = 0;
static uint hub75_rgb555_offset_data = 0;
static uint hub75_rgb555_offset_row = 0;

static size_t hub75_rgb555_buffer_rows = 32;
static size_t hub75_rgb555_buffer_rows_half = 16;
static size_t hub75_rgb555_buffer_columns = 64;
static uint32_t* hub75_rgb555_buffer0_p;
static uint32_t* hub75_rgb555_buffer1_p;

static uint32_t hub75_rgb555_row = 0;
static uint32_t hub75_rgb555_shift = 0;

void hub75_rgb555_init(PIO pio, uint sm_data, uint sm_row, uint pin_color, uint pin_sckl, uint pin_row, uint pin_latch, float freq){
    hub75_rgb555_pio = pio;
    hub75_rgb555_sm_data = sm_data;
    hub75_rgb555_sm_row = sm_row;
    
    // Set up state machines for pio
        // SM for data transfer
    pio_sm_claim(pio, sm_data);
    hub75_rgb555_offset_data = pio_add_program(pio0, &hub75_data_rgb555_program);
    hub75_data_rgb555_program_init(pio0, sm_data, hub75_rgb555_offset_data, pin_color, pin_sckl, freq);
        // SM for row selection
    pio_sm_claim(pio, sm_row);
    hub75_rgb555_offset_row = pio_add_program(pio0, &hub75_row_rgb555_program);
    hub75_row_rgb555_program_init(pio0, sm_row, hub75_rgb555_offset_row, pin_row, pin_latch, freq);

    // DMA channel configure
    dma_channel_claim(dma_channel_rgb555_data);
    dma_channel_config c_data = dma_channel_get_default_config(dma_channel_rgb555_data);
    channel_config_set_transfer_data_size(&c_data, DMA_SIZE_32);
    channel_config_set_read_increment(&c_data, true);
    channel_config_set_write_increment(&c_data, false);
    channel_config_set_dreq(&c_data, dma_channel_rgb555_data);

    dma_channel_configure(
        dma_channel_rgb555_data,    // Number of dma_rgb555 channel
        &c_data,                    // Сonfig
        &pio0_hw->txf[0],           // Write address
        NULL,                       // Read address
        1,                          // Transfer count
        false                       // Start right now
    );

    dma_channel_set_irq0_enabled(dma_channel_rgb555_data, true);
    hub75_rgb555_data_dma_handler();
}
inline void hub75_rgb555_set_buffer(uint32_t *buf_pa, uint number){
    if (number == 0){
        hub75_rgb555_buffer0_p = buf_pa;
    }
    else if (number == 1){
        hub75_rgb555_buffer1_p = buf_pa;
    }
    else{
        return;
    }
}

void hub75_rgb555_data_dma_handler(void){
    // Clr intr request
    dma_hw->ints0 = 1u << dma_channel_rgb555_data;

    /*
    // Max bit shift is equal to 5. 
    if (hub75_rgb555_shift >= 5){
        hub75_rgb555_shift = 0;

        // And if program reach the max shift limit, row can be changed
        hub75_rgb555_row++;
        if (hub75_rgb555_row >= hub75_rgb555_buffer_rows_half){
            hub75_rgb555_row = 0;
        }
    }

    hub75_data_rgb555_set_shift(hub75_rgb555_pio, hub75_rgb555_sm_data, hub75_rgb555_offset_data, hub75_rgb555_shift); 

    // Start transfer of data from the current row
    dma_channel_transfer_from_buffer_now(dma_channel_rgb555_data, (void *)&hub75_rgb555_buffer0_p[hub75_rgb555_row * hub75_rgb555_buffer_columns], hub75_rgb555_buffer_columns);
    
    // Send row number
    uint32_t row_and_delay = (hub75_rgb555_row | (1 << hub75_rgb555_shift) << 4);
    pio_sm_put_blocking(hub75_rgb555_pio, hub75_rgb555_sm_row, row_and_delay);

    // Increment current bit shift
    hub75_rgb555_shift++; 
    */

    if (hub75_rgb555_row >= hub75_rgb555_buffer_rows_half){
        hub75_rgb555_row = 0;
    }

    dma_channel_transfer_from_buffer_now(dma_channel_rgb555_data, &hub75_rgb555_buffer0_p[hub75_rgb555_row * hub75_rgb555_buffer_columns], hub75_rgb555_buffer_columns);
    hub75_rgb555_row++;
}