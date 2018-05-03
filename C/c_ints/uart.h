/*
 * uart.h
 *
 * UART driver for the TM4C
 */
 
#ifndef UART_H
#define UART_H
 
#include <stdint.h>
 
// ----------uartInit----------
// Initialize UART
void uartInit(void);

// ----------uartWrite----------
// Write a single byte to UART
// Parameters:
//      uint8_t data: input byte
void uartWrite(uint8_t data);

#endif
