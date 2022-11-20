# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

#include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ Standards
project(RGB C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Point out the CMake, where to find the executable source file
add_executable(${PROJECT_NAME}
        main.c
)

# Add include directories
target_include_directories(RGB 
        PUBLIC hub75
        colors
        core
        RIOmini
        uart
)

# Add sources
target_sources(RGB 
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/hub75/hub75_rgb111.c 
        ${CMAKE_CURRENT_SOURCE_DIR}/hub75/hub75_rgb555.c
        ${CMAKE_CURRENT_SOURCE_DIR}/core/core_pico.c
        ${CMAKE_CURRENT_SOURCE_DIR}/uart/uart.c
)

# For pio programs
pico_generate_pio_header(RGB ${CMAKE_CURRENT_LIST_DIR}/hub75/hub75_rgb111.pio) 
pico_generate_pio_header(RGB ${CMAKE_CURRENT_LIST_DIR}/hub75/hub75_rgb555.pio)

# Pull in our pico_stdlib which pulls in commonly used features (gpio, timer-delay etc)
target_link_libraries(${PROJECT_NAME}
        PUBLIC 
        pico_stdlib
        hardware_gpio
        hardware_pio
        hardware_clocks
        hardware_dma
        hardware_irq
        hardware_uart
)

# create map/bin/hex/uf2 files.
pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
