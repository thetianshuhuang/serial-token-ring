/*
	C client framework for the serial token ring network protocol
*/

#ifndef C_CLIENT_H
#define C_CLIENT_H

#include <stdint.h>


// Struct for storing IP info
struct ipInfo {
	uint8_t deviceAddress;
	char*(*recieveFunction)();
	void(*sendFunction)(uint8_t);
};


// ----------ipConfig----------
// Configure the token ring network
// Parameters:
// 		(uint8_t) device address (7-bit only)
//		char*(*recieveFunction)() recieve function; user defined
//		void(*sendFunction)(char*)	send function; user defined
// Returns: none
void ipConfig(uint8_t deviceAddress, char*(*recieveFunction)(), void(*sendFunction)(uint8_t));


// ----------sendMessage------------
// Send a packet on the network.
// Parameters:
//		(uint8_t) destination: destination address; pass 0xFF in order to broadcast
//		(char*) message to send
//		(uint8_t) length of message
// Returns: none
void sendMessage(uint8_t destination, char* message, uint8_t length);


// ----------updateNetwork----------
// Updates the token ring network; should be called at least once every 500us
// Parameters: none
// Modifies: the input buffer is filled with the input string
//		the first byte specifies the length of the buffer
//		(since null characters are allowed in the message)
// Returns: none
void updateNetwork(char* buffer);


#endif
