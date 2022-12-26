#include "hub75.h"
#include "colorRGB111.h"
#include "hub75_rgb111.pio.h"

static volatile PIO hub75_rgb111_pio = pio0;
static volatile uint hub75_rgb111_sm_data = 0;
static volatile uint hub75_rgb111_sm_row = 0;
static volatile uint hub75_rgb111_offset_data = 0;
static volatile uint hub75_rgb111_offset_row = 0;

static volatile uint32_t hub75_rgb111_rows = 32;
static volatile uint32_t hub75_rgb111_rows_half = 16;
static volatile uint32_t hub75_rgb111_columns = 64;
static volatile uint8_t* hub75_rgb111_buffer0_p;
static volatile uint8_t* hub75_rgb111_buffer1_p;
static volatile size_t hub75_rgb111_activeBuffer = 0;

static volatile uint32_t hub75_rgb111_row = 0;

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
    dma_channel_config c_data = { 0 };
    dma_channel_claim(DMA_CHANNEL_RGB111_DATA);
    channel_config_set_read_increment(&c_data, true);
    channel_config_set_write_increment(&c_data, false);
    channel_config_set_dreq(&c_data, DREQ_PIO0_TX0);
    channel_config_set_chain_to(&c_data, DMA_CHANNEL_RGB111_DATA);
    channel_config_set_transfer_data_size(&c_data, DMA_SIZE_8);
    channel_config_set_enable(&c_data, true);
    dma_channel_set_write_addr(DMA_CHANNEL_RGB111_DATA, &pio0_hw->txf[0], false);
    dma_channel_set_config(DMA_CHANNEL_RGB111_DATA, &c_data, false);

     // Enable channel for rgb111 interruption for DMA IRQ 0
    dma_channel_set_irq0_enabled(DMA_CHANNEL_RGB111_DATA, true);

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
uint8_t* hub75_rgb111_get_buffer(void){
    return hub75_rgb111_buffer0_p;
}

uint16_t hub75_rgb111_width_get(void){ return hub75_rgb111_columns; }
uint16_t hub75_rgb111_height_get(void){ return hub75_rgb111_rows; }
uint16_t hub75_rgb111_height_half_get(void){ return hub75_rgb111_rows_half; }

void hub75_rgb111_data_dma_handler(void){
    #define ROW_BEG 4

    // Clr intr request
    dma_hw->ints0 |= (1u << DMA_CHANNEL_RGB111_DATA);

    // Send row number
    pio_sm_put_blocking(hub75_rgb111_pio, hub75_rgb111_sm_row, hub75_rgb111_row);

    // Start transfer of data from the current row
    /* Temp beg */
    uint32_t row = hub75_rgb111_row - ROW_BEG;
    row = (row < 0) ? 16 - row - ROW_BEG : row; 
    dma_channel_transfer_from_buffer_now(DMA_CHANNEL_RGB111_DATA, &hub75_rgb111_buffer0_p[row * hub75_rgb111_columns], hub75_rgb111_columns);
    /* Temp end */
    
    /* Normal */
    // dma_channel_transfer_from_buffer_now(DMA_CHANNEL_RGB111_DATA, &hub75_rgb111_buffer0_p[hub75_rgb111_row * hub75_rgb111_columns], hub75_rgb111_columns);

    // Row 
    hub75_rgb111_row++;
    if (hub75_rgb111_row >= hub75_rgb111_rows_half + ROW_BEG){
        hub75_rgb111_row = ROW_BEG;
    }
}