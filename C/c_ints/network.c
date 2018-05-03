/*
 * network.c
 *
 * TM4C network driver
 * For use by the Serial Token Ring protocol
 *
 * Written by Tianshu Huang, April 2018 
 */
 
#include "network.h"
#include "uart.h"
#include "c_client.h"
#include "../tm4c123gh6pm.h"
#include "fifo.h"
#include "../display/ST7735.h"
#include "../controller/controller.h"

// Game ID ifndef for testing
#ifndef GAME_ID
#define GAME_ID 0x42
#endif


// Peer tracking
uint8_t peers[16];
uint8_t numPeers;


// ----------getAddress----------
// Initialize the network
// Read the network address from PC6, PC7, PD6, PD7
// Returns:
//      uint8_t: address |0000|PC6|PC7|PD6|PD7|
uint8_t getAddress(void) {

    // Enable ports C and D
    SYSCTL_RCGCGPIO_R |= 0x0C;
    __asm{NOP};
    __asm{NOP};

    // PC6, PC7, PD6, PD7 digital in
    GPIO_PORTC_AMSEL_R &= ~0xC0;
    GPIO_PORTD_AMSEL_R &= ~0xC0;
    GPIO_PORTC_PUR_R &= 0xC0;
    GPIO_PORTD_PUR_R &= 0xC0;
    GPIO_PORTC_DIR_R &= ~0xC0;
    GPIO_PORTD_DIR_R &= ~0xC0;
    GPIO_PORTC_AFSEL_R &= ~0xC0;
    GPIO_PORTD_AFSEL_R &= ~0xC0;
    GPIO_PORTC_DEN_R |= 0xC0;
    GPIO_PORTC_DEN_R |= 0xC0;
    
    // Read address
    return(
        (GPIO_PORTC_DATA_R & 0xC0) >> 4 |
        (GPIO_PORTD_DATA_R & 0xC0) >> 6);
}


// ----------networkInit----------
// Initialize the network
// Gets the network address from PC6, PC7, PD6, PD7
void networkInit(void) {
    ipConfig(getAddress(), &uartWrite);
    numPeers = 0;
}


// ----------outHex----------
// Helper function to print out a one byte hex value
// Parameters:
//      uint8_t data: hex value
const char hexString[16] = "0123456789ABCEDF";
void outHex(uint8_t data) {
    ST7735_OutChar(hexString[(data & 0xF0) >> 4]);
    ST7735_OutChar(hexString[data & 0x0F]);
}


// ----------discoverPeers----------
// Run peer discovery
void discoverPeers(void) {
    
    // Clear screen
    ST7735_FillScreen(0);
    ST7735_SetTextColor(0x0000);
    ST7735_SetCursor(0,0);
    
    // Show IP address
    ST7735_OutString("Your IP:");
    ST7735_SetCursor(1,8);
    outHex(getAddress());
    
    // Show start message
    ST7735_SetCursor(0,0);
    ST7735_OutString("Press red to start");
    
    // Show lobby
    ST7735_SetCursor(2,0);
    ST7735_OutString("Current Lobby");
    for(int i = 0; controllerRead() & 0x1000; i++) {
        // Clear previous members
        for(uint8_t i = 0; i < 16; i++) {
            ST7735_SetCursor(i + 4, 0);
            ST7735_OutString("  ");
        }
        // Draw members
        for(uint8_t i = 0; i < numPeers; i++) {
            ST7735_SetCursor(i+2, 0);
            outHex(peers[i]);
        }
        // Send broadcast once every 255 loops
        if(i == 0) {
            uint8_t message[1];
            message[0] = GAME_ID;
            sendMessage(0x00, message, 1);
        }
    }
}
