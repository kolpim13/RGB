#include "hub75.h"
#include "colorRGB111.h"
#include "hub75.pio.h"

static uint32_t hub75_rgb111_buffer_rows = 32;
static uint32_t hub75_rgb111_buffer_columns = 128;
static uint8_t* hub75_rgb111_buffer0;
static uint8_t* hub75_rgb111_buffer1;
//===============================================================

void hub75_rgb111_init(){
    // Fill buffer1 with full white color
    for (uint32_t i = 0; i < 2048; i++){
        if (i % 2 == 0){ 
            hub75_rgb111_buffer1[i] = RGB111_WHITE | RGB111_WHITE << 3;
        }
        else{
            hub75_rgb111_buffer1[i] = RGB111_BLACK | RGB111_BLACK << 3;
        }
    }

    // Set up pio state machine
        // Data transfer
    uint sm_data = 0;
    pio_sm_claim(pio0, sm_data);
    uint offset_rgb111 = pio_add_program(pio0, &hub75_data_rgb111_program);
    hub75_data_rgb111_program_init(pio0, sm_data, offset_rgb111, 0, 6, 1000000.0);    // [0-5] - data, 6 - clk

        // Row selection
    uint sm_row = 1;
    pio_sm_claim(pio0, sm_row);
    uint offset_row = pio_add_program(pio0, &hub75_row_program);
    hub75_row_program_init(pio0, sm_row, offset_row, 10, 14, 1000000.0);    // [0-5] - data, 6 - clk

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

        // Enable irq for choosen dma_rgb111_data channel
    dma_channel_set_irq0_enabled(dma_channel_rgb111_data, true);

        // Enable irq
    irq_set_exclusive_handler(DMA_IRQ_0, hub75_rgb111_data_dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);    

    // Execute handler to run transaction sequence
    hub75_data_dma_handler();
}

void hub75_rgb111_set_buffer(uint8_t* buf_pa, uint number){
    if (number == 0){
        hub75_rgb111_buffer0 = buf_pa;
    }
    else if (number == 1){
        hub75_rgb111_buffer1 = buf_pa;
    }
    else{
        return;
    }
}
//===============================================================

void hub75_rgb111_data_dma_handler(void){
    static uint8_t row = 255 ;
    
    // Clr intr request
    dma_hw->ints0 = 1u << dma_channel_rgb111_data;

    // Start transfer of data from the current row
    dma_channel_transfer_from_buffer_now(dma_channel_rgb111_data, &hub75_rgb111_buffer1[0], 128);

    // Send correct row number
    row++;
    if (row > 15){
        row = 0;
    }

    // Send row number
    pio_sm_put_blocking(pio0, 1, row);
}
//===============================================================
