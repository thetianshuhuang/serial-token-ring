/*
	C client framework for the serial token ring network protocol

	For use with the Serial Token Ring communication protocol
	https://github.com/thetianshuhuang/serial-token-ring
    
    Modified for Lab 10 (n-player Rock Band)
*/

#ifndef C_CLIENT_H
#define C_CLIENT_H

#include <stdint.h>


// Struct for storing IP info
struct ipInfo {
	uint8_t deviceAddress;
	void(*sendFunction)(uint8_t);
    uint8_t* packetBuffer;
    uint8_t bufferSize;
};


// ----------ipConfig----------
// Configure the token ring network
// Parameters:
// 		(uint8_t) device address (7-bit only)
//		void(*sendFunction)(char*):	send function; user defined
// Returns: none
void ipConfig(
    uint8_t deviceAddress,
    void(*sendFunction)(uint8_t)
);


// ----------sendMessage------------
// Send a packet on the network.
// Parameters:
//		(uint8_t) destination: destination address; pass 0xFF in order to broadcast
//		(char*) message to send
//		(uint8_t) length of message
// Returns: none
void sendMessage(uint8_t destination, uint8_t* message, uint8_t length);


// ----------updateNetwork----------
// Updates the token ring network
// should be called whenever a character is recieved
// Parameters:
//      char rx: character to update the network with
uint8_t* updateNetwork(uint8_t rx);


#endif
