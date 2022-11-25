#include <stdio.h>
#include "pico/stdlib.h"

#include "hub75.h"
#include "core_pico.h"
#include "colorRGB111.h"
#include "uart.h"

#define LED_PICO    25
#define LED_PICO_W  0
#define LED LED_PICO

static int a = 0;

int main() {
    // For diod
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    UART_init(115200);
    UART_readAsync();
    
    for(;;){
        if (UART_RX_State_Get() == TX_RX_Ready){
            a = 1;
            UART_readAsync();
        }
        if (a == 1){
            UART_transferAsync("Answ", 4);
        }
        else{
            UART_transferAsync("Maks", 4);
        }
        a = 0;
        sleep_ms(500);
    }
}