#pragma once

#define HUB75_RGB111    1
#define HUB75_RGB555    0

#if HUB75_RGB111 == 1
    #include "hub75_rgb111.h"
#endif

#if HUB75_RGB555 == 1
    #include "hub75_rgb555.h"
#endif

