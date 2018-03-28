/*
	C client framework for the serial token ring network protocol
	Only function definitions supplied; no implementation included.

	For use with the Serial Token Ring communication protocol
	https://github.com/thetianshuhuang/serial-token-ring
*/

#include <stdint.h>
#include "c_client.h"


uint8_t currentCheckSum;
struct ipInfo configuration;


// ----------calculateChecksum----------
// Private helper function for calculating the checksum of a string
// Parameters:
//		(char*) input string
//		(uint8_t) length of input string (since null is an allowed value)
// Returns: (uint8_t) checksum
uint8_t calculateChecksum(char* inputString, uint8_t length) {

	return;
}


// ----------clearChecksum----------
// Private helper function for clearing the tracked checksum
// Parameters: none
// Returns: none
void clearChecksum(void) {

	return;
}


// ----------sendByte----------
// Private helper function for sending a byte and updating the tracked checksum
// Parameters: (uint8_t) character to send and XOR into the checksum
// Returns: (uint8_t) current value of the checksum
uint8_t sendByte(uint8_t input) {

	return;
}


// ----------ipConfig----------
// Configure the token ring network
// Parameters:
// 		(uint8_t) device address (7-bit only)
//		char*(*recieveFunction)() recieve function; user defined
//		void(*sendFunction)(char*)	send function; user defined
// Returns: none
void ipConfig(
	uint8_t deviceAddress,
	char*(*recieveFunction)(),
	void(*sendFunction)(uint8_t)) {

	return;
}


// ----------sendMessage------------
// Send a packet on the network.
// Parameters:
//		(uint8_t) destination
//		(char*) message to send
//		(uint8_t) length of message
// Returns: none
void sendMessage(uint8_t destination, char* message, uint8_t length) {

	return;
}


// ----------updateNetwork----------
// Updates the token ring network
// should be called whenever a character is recieved
// Parameters: none
// Modifies: the input buffer is filled with the input string
//		the first byte specifies the length of the buffer
//		(since null characters are allowed in the message)
// Returns: none
void updateNetwork(char* buffer) {

	return;
}
