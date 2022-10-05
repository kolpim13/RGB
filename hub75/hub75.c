#include "hub75.h"
#include "colorRGB111.h"
#include "hub75.pio.h"

static uint32_t hub75_rgb111_buffer_rows = 32;
static uint32_t hub75_rgb111_buffer_columns = 128;
static uint8_t* hub75_rgb111_buffer0;
static uint8_t* hub75_rgb111_buffer1;
static size_t hub75_rgb111_activeBuffer = 0;
//===============================================================

void hub75_rgb111_init(){
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
}

void hub75_rgb111_start(void){
    // Enable channel for rgb111 interruption for DMA IRQ 0
    dma_channel_set_irq0_enabled(dma_channel_rgb111_data, true);

    // Execute handler to run transaction sequence
    hub75_rgb111_data_dma_handler();
}

void hub75_rgb111_stop(void){
    // Disable channel for rgb111 interruption on DMA IRQ 0
    dma_channel_set_irq0_enabled(dma_channel_rgb111_data, false);
    // Abort channel
    dma_channel_abort(dma_channel_rgb111_data);
    // Clear the spurious (maybe there will be one)
    dma_channel_acknowledge_irq0(dma_channel_rgb111_data);
    // Now channel is ready to be re-enabled ...
}
//===============================================================

/* GET & SET */

inline void hub75_rgb111_set_buffer(uint8_t *buf_pa, uint number){
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
inline uint8_t* hub75_rgb111_get_buffer(uint number){
    if (number == 0){
        return hub75_rgb111_buffer0;
    }
    else if (number == 1){
        return hub75_rgb111_buffer1;
    }
    else{
        return NULL;
    }
}
inline size_t hub75_rgb111_get_activeBufferNumber(void){
    return hub75_rgb111_activeBuffer;
}
//===============================================================

void hub75_rgb111_data_dma_handler(void){
    static uint8_t row = 255 ;

    // Check if event was
    /*if ((dma_hw->ints0 & (1u << dma_channel_rgb111_data)) == 0){
        return;
    }*/
    
    // Clr intr request
    dma_hw->ints0 = 1u << dma_channel_rgb111_data;

    // Start transfer of data from the current row
    dma_channel_transfer_from_buffer_now(dma_channel_rgb111_data, &hub75_rgb111_buffer1[hub75_rgb111_activeBuffer], 128);

    // Send correct row number
    row++;
    if (row > 15){
        row = 0;
    }

    // Send row number
    pio_sm_put_blocking(pio0, 1, row);
}
//===============================================================
