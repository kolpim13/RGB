#include <stdio.h>
#include "pico/stdlib.h"

#include "hub75.h"
#include "core_pico.h"
#include "colorRGB111.h"

#define LED_PICO    25
#define LED_PICO_W  0
#define LED LED_PICO

int main() {
    // For diod
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    for(;;){
        // To be sure chip works in normal mode
        gpio_put(LED, true);
        sleep_ms(500);
        gpio_put(LED, false);
        sleep_ms(500);
    }
}