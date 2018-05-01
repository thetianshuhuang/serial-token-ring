/*
	C client framework for the serial token ring network protocol

	For use with the Serial Token Ring communication protocol
	https://github.com/thetianshuhuang/serial-token-ring
*/

#ifndef C_CLIENT_H
#define C_CLIENT_H

#include <stdint.h>


// Struct for storing IP info
struct ipInfo {
	char deviceAddress;
	char(*recieveFunction)();
	void(*sendFunction)(char);
};


// ----------ipConfig----------
// Configure the token ring network
// Parameters:
// 		(uint8_t) device address (7-bit only)
//		char*(*recieveFunction)() recieve function; user defined
//		void(*sendFunction)(char*)	send function; user defined
// Returns: none
void ipConfig(uint8_t deviceAddress, char(*recieveFunction)(), void(*sendFunction)(char));


// ----------sendMessage------------
// Send a packet on the network.
// Parameters:
//		(uint8_t) destination: destination address; pass 0xFF in order to broadcast
//		(char*) message to send
//		(uint8_t) length of message
// Returns: none
void sendMessage(uint8_t destination, char* message, uint8_t length);


// ----------updateNetwork----------
// Updates the token ring network
// should be called whenever a character is recieved
// Parameters: none
char* updateNetwork(void);


#endif
