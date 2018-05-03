/*
 * uart.c
 *
 * UART driver for the TM4C
 */

#include "uart.h"
#include "../tm4c123gh6pm.h"
#include "c_client.h"
#include "fifo.h"
#include "../game/core.h"


// Register Bit Values
#define UART_TXFF   0x0020      // UART TX not full
#define UART_RXFF   0x0040      // UART RX not full
#define UART_TXRIS  0x0020      // UART TX interrupt flag
#define UART_RXRIS  0x0010      // UART RX interrupt flag
#define UART_TXIC   0x0020      // UART TX interrupt acknowledge
#define UART_RXIC   0x0010      // UART RX interrupt acknowledge
#define UART_TXIM   0x0020      // UART TX interrupt enable

// FIFO queues
FIFO_QUEUE txFifo;

// Game ID ifndef for testing
#ifndef GAME_ID
#define GAME_ID 0x42
#endif

 
// ----------uartInit----------
// Initialize UART
 void uartInit(void) {

    // Turn on UART1
    SYSCTL_RCGCUART_R |= 0x0002;    // UART 1
    SYSCTL_RCGCGPIO_R |= 0x0004;    // Port C
    
    // Create fifo queues
    fifoInit(&txFifo);
     
    // Disable UART during init
    UART1_CTL_R &= ~0x0001;
    
    // 115200 Baud, 8 bit word length
    UART1_IBRD_R = 43;
    UART1_FBRD_R = 26;
    // 8 bit length
    UART1_LCRH_R = 0x70;

    // Enable UART1 (RXE[9], TXE[8], UARTEN[0])
    UART1_CTL_R |= 0x0301;
   
    // Set up interrupts (TXIM[5] and RXIM[4])
    UART1_IM_R |= 0x30;
    // Interrupts at 1/8 full for RX, 1/2 full for TX
    UART1_IFLS_R = (UART1_IFLS_R & ~0x3F) | 0x02;
    // Priority
    NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFF00FF) | 0x00004000;
    NVIC_EN0_R = NVIC_EN0_INT6;
   
    // Enable alternate function
    GPIO_PORTC_AFSEL_R |= 0x30;     // PC4,5
    GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFF00FFFF) + 0x00220000; // ???
    GPIO_PORTC_DEN_R |= 0x30;       // Enable digital
    GPIO_PORTC_AMSEL_R &= ~0x30;    // Disable analog
}


// ----------writeToHardware----------
// Write software TX FIFO to hardware FIFO
void writeToHardware(void) {
    char datapt;
    while(((UART1_FR_R & UART_TXFF) == 0) && (txFifo.size > 0)) {
        fifoGet(&txFifo, &datapt);
        UART1_DR_R = datapt;
    }
}


// ----------uartWrite----------
// Write a single byte to UART
// Parameters:
//      uint8_t data: input byte
void uartWrite(uint8_t data) {
    while(txFifo.size >= _FIFO_SIZE){};
    UART1_IM_R &= ~UART_TXIM;
    fifoPut(&txFifo, data);
    UART1_IM_R |= UART_TXIM;
}


// ----------UART1_Handler----------
// Handler for UART1 interrupt
void UART1_Handler(void) {
    
    // Output of updateNetwork
    uint8_t* currentBuffer;

    // Acknowledge TX interrupt
    if(UART1_RIS_R & UART_TXRIS) {
        UART1_ICR_R = UART_TXIC;
        writeToHardware();
        if(txFifo.size == 0) {
            UART0_IM_R &= ~UART_TXIM;
        }
    }

    // Acknowledge RX interrupt
    if(UART1_RIS_R & UART_RXRIS) {
        UART1_ICR_R = UART_RXIC;
        while((UART1_FR_R & 0x10) == 0) {
            // Update network with recieved bytes
            currentBuffer = updateNetwork(UART1_DR_R);
            if((currentBuffer[0] != 0) && (currentBuffer[4] == GAME_ID)) {
                // Update game
                updateGame(currentBuffer);
            }
        }
    }

}
