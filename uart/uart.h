#pragma once

#include "core_pico.h"

#define UART            uart0
#define UART_PIN_TX     16U
#define UART_PIN_RX     17U

void UART_init(uint32_t baud);
void UART_transferAsync(const volatile uint8_t* buffer_a, uint32_t len);