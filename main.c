#include <stdio.h>
#include "pico/stdlib.h"

#include "hub75.h"
#include "core_pico.h"
#include "colorRGB111.h"
#include "uart.h"
#include "uds.h"

uint8_t rgb111_a[1024];

int main() {
    // For diod
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    gpio_init(LED_OSCYLOSCOP);
    gpio_set_dir(LED_OSCYLOSCOP, GPIO_OUT);

    PICO_IRQ_Set_Handlers();

    hub75_rgb111_set_buffer(rgb111_a, 0);
    hub75_rgb111_init(pio0, 0, 1, 2, 8, 10, 14, 1000000.0);

    UART_init(115200);
    UART_readAsync();
    
    for(;;){
        UDS_Manage();
    }
}