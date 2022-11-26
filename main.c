#include <stdio.h>
#include "pico/stdlib.h"

#include "hub75.h"
#include "core_pico.h"
#include "colorRGB111.h"
#include "uart.h"

#define LED_PICO    25
#define LED_PICO_W  0
#define LED LED_PICO

int main() {
    // For diod
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    UART_init(115200);
    UART_readAsync();
    
    for(;;){
        if (UART_RX_State_Get() == TX_RX_Ready){
            UART_transferAsync("Mo", 2);
            UART_readAsync();
        }
        //sleep_ms(5000);
    }
}