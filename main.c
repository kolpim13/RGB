#include <stdio.h>
#include "pico/stdlib.h"

#include "hub75.h"

int main() {
    hub75_init();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    for(;;){
        gpio_put(25, true);
        sleep_ms(500);
        gpio_put(25, false);
        sleep_ms(500);
    }
}