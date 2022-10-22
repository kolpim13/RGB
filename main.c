#include <stdio.h>
#include "pico/stdlib.h"

#include "hub75.h"
#include "core_pico.h"
#include "colorRGB111.h"

// 2 pixels in one uint32 word (32 * 64 : 2 = 1024 words)
static uint32_t temporary_a[1024];

int main() {
    // For diod
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    // For oscyloscop
    gpio_init(22);
    gpio_set_dir(22, GPIO_OUT);

    // Shared hadlers
    PICO_IRQ_Set_Handlers();

    // Fill the temporarry buffer for rgb888 colors
    uint32_t red = 0, green = 0, blue = 0;
    for (size_t i = 0; i < 2048; i++){
        /* Normal
        if (red == 32){
            red = 0;
            green += 2;
        }
        if (green == 32){
            green = 0;
            blue += 2;
        }

        uint32_t color = red | (green << 5) | (blue << 11);
        if (i < 1024){
            temporary_a[i] = color;
        }
        else{
            temporary_a[i - 1024] = color << 16;
        }
        red += 4;
        */
       /* Test */
       if (i < 1024){
            temporary_a[i] = 1 | (1 << 5);
        }
        else{
            temporary_a[i - 1024] = 1 | (1 << 5);
        }
    }

    hub75_rgb555_set_buffer(temporary_a, 0);
    hub75_rgb555_init(pio0, 0, 1, 0, 6, 10, 14, 1000000.0);

    for(;;){
        // To be sure chip works in normal mode
        gpio_put(25, true);
        sleep_ms(500);
        gpio_put(25, false);
        sleep_ms(500);
    }
}