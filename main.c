#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "core_pico.h"
#include "usb_user.h"
#include "tusb.h"

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
        usb_task();
        UDS_Manage();
    }
}
