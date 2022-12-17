#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core_pico.h"
#include "bsp/board.h"
#include "tusb.h"

static void cdc_task(void);

volatile uint8_t rgb111_a[1024];

int main() {
    // For diod
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    PICO_IRQ_Set_Handlers();

    hub75_rgb111_set_buffer(rgb111_a, 0);
    hub75_rgb111_init(pio0, 0, 1, 2, 8, 10, 14, 1000000.0);

    tusb_init();
    
    for(;;){
        tud_task();
        UDS_Manage();
    }
}

static void echo_serial_port(uint8_t itf, uint8_t buf[], uint32_t count)
{
  for(uint32_t i=0; i<count; i++)
  {
    if (itf == 0)
    {
      // echo back 1st port as lower case
      if (isupper(buf[i])) buf[i] += 'a' - 'A';
    }
    else
    {
      // echo back 2nd port as upper case
      if (islower(buf[i])) buf[i] -= 'a' - 'A';
    }

    tud_cdc_n_write_char(itf, buf[i]);
  }
  tud_cdc_n_write_flush(itf);
}

static void cdc_task(void){
    uint8_t itf;

  for (itf = 0; itf < CFG_TUD_CDC; itf++)
  {
    // connected() check for DTR bit
    // Most but not all terminal client set this when making connection
    // if ( tud_cdc_n_connected(itf) )
    {
      if ( tud_cdc_n_available(itf) )
      {
        uint8_t buf[64];

        uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

        // echo back to both serial ports
        echo_serial_port(0, buf, count);
      }
    }
  }
}