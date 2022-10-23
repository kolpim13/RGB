#include "hub75.h"
#include "colorRGB111.h"
#include "hub75_rgb111.pio.h"

static PIO hub75_rgb111_pio = pio0;
static uint hub75_rgb111_sm_data = 0;
static uint hub75_rgb111_sm_row = 0;
static uint hub75_rgb111_offset_data = 0;
static uint hub75_rgb111_offset_row = 0;

static uint32_t hub75_rgb111_rows = 32;
static uint32_t hub75_rgb111_rows_half = 16;
static uint32_t hub75_rgb111_columns = 64;
static uint8_t* hub75_rgb111_buffer0_p;
static uint8_t* hub75_rgb111_buffer1_p;
static size_t hub75_rgb111_activeBuffer = 0;

static uint32_t hub75_rgb555_row = 0;

void hub75_rgb111_init(PIO pio, uint sm_data, uint sm_row, uint pin_color, uint pin_sckl, uint pin_row, uint pin_latch, float freq){
    hub75_rgb111_pio = pio;
    hub75_rgb111_sm_data = sm_data;
    hub75_rgb111_sm_row = sm_row;
    
    // Set up pio state machine
        // Data transfer
    pio_sm_claim(pio0, sm_data);
    uint hub75_rgb111_offset_data = pio_add_program(pio0, &hub75_data_rgb111_program);
    hub75_data_rgb111_program_init(pio0, sm_data, hub75_rgb111_offset_data, pin_color, pin_sckl, freq);

        // Row selection
    pio_sm_claim(pio0, sm_row);
    uint hub75_rgb111_offset_row = pio_add_program(pio0, &hub75_row_rgb111_program);
    hub75_row_rgb111_program_init(pio0, sm_row, hub75_rgb111_offset_row, pin_row, pin_latch, freq);

    // DMA rgb111 data transfer
    dma_channel_claim(dma_channel_rgb111_data);
    dma_channel_config c_data = dma_channel_get_default_config(dma_channel_rgb111_data);   // default configs
    channel_config_set_transfer_data_size(&c_data, DMA_SIZE_8);
    channel_config_set_read_increment(&c_data, true);
    channel_config_set_write_increment(&c_data, false);
    channel_config_set_dreq(&c_data, dma_channel_rgb111_data);
    
    dma_channel_configure(
        dma_channel_rgb111_data,    // Number of dma_rgb111 channel
        &c_data,                    // Сonfig
        &pio0_hw->txf[0],           // Write address
        NULL,                       // Read address
        1,                          // Transfer count
        false                       // Start right now
    );

     // Enable channel for rgb111 interruption for DMA IRQ 0
    dma_channel_set_irq0_enabled(dma_channel_rgb111_data, true);

    // Execute handler to run transaction sequence
    hub75_rgb111_data_dma_handler();
}

void hub75_rgb111_set_buffer(uint8_t *buf_pa, uint number){
    if (number == 0){
        hub75_rgb111_buffer0_p = buf_pa;
    }
    else if (number == 1){
        hub75_rgb111_buffer1_p = buf_pa;
    }
    else{
        return;
    }
}
void hub75_rgb111_data_dma_handler(void){
    // Clr intr request
    dma_hw->ints0 = 1u << dma_channel_rgb111_data;

    // Send row number
    pio_sm_put_blocking(hub75_rgb111_pio, hub75_rgb111_sm_row, hub75_rgb555_row);

    // Row 
    if (hub75_rgb555_row >= hub75_rgb111_rows_half){
        hub75_rgb555_row = 0;
    }

    // Start transfer of data from the current row
    dma_channel_transfer_from_buffer_now(dma_channel_rgb111_data, &hub75_rgb111_buffer0_p[hub75_rgb555_row * hub75_rgb111_columns], hub75_rgb111_columns);

    hub75_rgb555_row++;
}