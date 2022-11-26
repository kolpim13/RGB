#include <stdio.h>
#include "pico/stdlib.h"

#include "hub75.h"
#include "core_pico.h"
#include "colorRGB111.h"
#include "uart.h"

int main() {
    // For diod
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    gpio_init(LED_OSCYLOSCOP);
    gpio_set_dir(LED_OSCYLOSCOP, GPIO_OUT);

    UART_init(115200);
    UART_readAsync();
    
    for(;;){
        
    }
}