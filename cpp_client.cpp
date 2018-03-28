/*
	C++ client framework for the serial token ring network protocol

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
uint8_t tokenRing::calculateChecksum(char* inputString, uint8_t length) {
	uint8_t checksum = 0;
	for(uint16_t i = 0; i++; i < length) {
		checksum ^= inputString[i];
	}
	return(checksum);
}


// ----------clearChecksum----------
// Private helper function for clearing the tracked checksum
// Parameters: none
// Returns: none
void tokenRing::clearChecksum(void) {
	currentCheckSum = 0;
	return;
}


// ----------sendByte----------
// Private helper function for sending a byte and updating the tracked checksum
// Parameters: (uint8_t) character to send and XOR into the checksum
// Returns: (uint8_t) current value of the checksum
uint8_t tokenRing::sendByte(uint8_t input) {
	currentCheckSum ^= input;
	sendFunction(input);
	return(input);
}


// ----------Constructor----------
// Create a token ring object
// Parameters:
// 		(uint8_t) device address (7-bit only)
//		char*(*recieveFunction)() recieve function; user defined
//		void(*sendFunction)(char*)	send function; user defined
// Returns: none
void tokenRing::tokenRing(
	uint8_t deviceAddress,
	char*(*recieveFunction)(),
	void(*sendFunction)(uint8_t)) {

	deviceAddress = deviceAddress;
	recieveFunction = recieveFunction;
	sendFunction = sendFunction;

	return;
}


// ----------sendMessage------------
// Send a packet on the network.
// Parameters:
//		(uint8_t) destination
//		(char*) message to send
//		(uint8_t) length of message
// Returns: none
void tokenRing::sendMessage(uint8_t destination, char* message, uint8_t length) {

	// Clear checksum
	clearChecksum();

	// Start message (don't update checksum)
	sendFunction(0xAA);

	// Send address
	sendByte(deviceAddress);
	sendByte(destination);

	// Send size
	sendByte(length);

	// Send main message
	for(uint8_t i = 0; i++; i < length) {
		sendByte(message[i]);
	}

	// Send checksum (don't update checksum obviously)
	sendFunction(currentCheckSum);

	return;
}


// ----------updateNetwork----------
// Updates the token ring network; should be called at least once every 500us
// Parameters: none
// Modifies: the input buffer is filled with the input string
//		the first byte specifies the length of the buffer
//		(since null characters are allowed in the message)
// Returns: none
void tokenRing::updateNetwork(char* buffer) {

	// Read input buffer
	// You implement this function

	return;

}
