#include <stdio.h>
#include "pico/stdlib.h"

#include "hub75.h"
#include "core_pico.h"
#include "colorRGB111.h"

static uint8_t temporary_a[2048];

int main() {
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    // Test shared hadlers
    PICO_IRQ_Set_Handlers();

    // Fill the temporary buffer for test
    for (size_t i = 0; i < 2048; i++){
        if (i % 2 == 0){
            temporary_a[i] = RGB111_WHITE | RGB111_WHITE << 3;
        }
        else{
            temporary_a[i] = RGB111_BLACK | RGB111_BLACK << 3;
        }
    }
    hub75_rgb111_set_buffer(temporary_a, 0);
    hub75_rgb111_init();
    hub75_rgb111_start();

    for(;;){
        gpio_put(25, true);
        sleep_ms(500);
        gpio_put(25, false);
        sleep_ms(500);
    }
}