#pragma once

#include "core_pico.h"

#define UART            uart0
#define UART_HW         uart0_hw
#define UART_PIN_TX     16U
#define UART_PIN_RX     17U
#define UART_RX_LEN     4096

/*
 * All possible states for uart transmitter and receiver.
 ? I should think about what kind of states can be aplicapable to my code
*/
typedef enum _UART_TX_RX_State_e{
    TX_RX_Ready,
    TX_RX_Listen __attribute__((unused)),
    TX_RX_Busy,
    TX_RX_Error,
}UART_TX_RX_State_e;

/* Init uart */
void UART_init(uint32_t baud);

/* UART_transferAsync
    Start transfering data via dma.
*/
void UART_transferAsync(const volatile uint8_t* buffer_a, uint32_t len);

/* UART_readAsync
    Start reading data via dma.
*/
void UART_readAsync(void);

/*
 * Getter and setter for state of transiver module
 ! Get is public and set is private.
*/
UART_TX_RX_State_e UART_TX_State_Get(void);
static void UART_TX_State_Set(UART_TX_RX_State_e state);

/*
 * Getter and setter for state of receiver module
 ! Get is public and set is private.
*/
UART_TX_RX_State_e UART_RX_State_Get(void);
static void UART_RX_State_Set(UART_TX_RX_State_e state);

/*
 * Get receive buffer
 * Get current amount of data in bytes in the read buffer
*/
uint8_t* UART_ReceiveBuffer_Get(void);
uint32_t UART_ReceiveBufferLen_Get(void);

/* UART_IRQ_Handler
 * Method to manage irq events
*/
void UART_IRQ_Handler(void);