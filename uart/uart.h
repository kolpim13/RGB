/* UART module general description
 * This uart is used only for communication with PC [high level program] using dedicated protocol. No general purpose.
 It means that if it possible to win performance with the cost of flexibility it is need to be done.
 Transfer is performed via dma.
 Receive implemented based on irq on every single byte. End of mes - special character
 
 * Settings:
 1. FIFO mode is disabled 

 * Performance:
 Transfer provide no additional affords except of start and stop sequence
 Receiving of one byte is about 250 ns.

 ! Discovered HW problems:
 1. while dma on receive is working, it is impossible to trigger related uart irq.
 This fact leads to situation where it is impossible to spot end of dma transfer by timeout after last character.

 TODO:
 1. Add uart errors detection and corresponding reaction of the module on them.
 2. Try to implement FIFO mode for receiving data.  
    ? timeout interrupt - end of mes, receive interrupt - another part of the mes.
    Use RTS signal!
    Calculate performance and compare it to the current method.
 3. Add TX_RX_State that would indicate someone is working with resources, but no transfer or receiving data.
*/

#pragma once

#include "core_pico.h"

#define UART            uart0
#define UART_HW         uart0_hw
#define UART_PIN_TX     16U
#define UART_PIN_RX     17U
#define UART_PIN_RTS    18U
#define UART_RX_LEN     4096

#define UART_SYMB_END_OF_MES    0xFF

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

/* UART_TX_DMA_Handler
 * Method to observe and manage TX transfer state. 
 End of transfer, errors, etc.
*/
void UART_TX_DMA_Handler(void);