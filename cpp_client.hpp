/*
	C++ client framework for the serial token ring network protocol

	For use with the Serial Token Ring communication protocol
	https://github.com/thetianshuhuang/serial-token-ring
*/

#ifndef C_CLIENT_H
#define C_CLIENT_H

#include <stdint.h>


class tokenRing {

	public:

		// ----------Constructor----------
		// Create a token ring object
		// Parameters:
		// 		(uint8_t) device address (7-bit only)
		//		char*(*recieveFunction)() recieve function; user defined
		//		void(*sendFunction)(char*)	send function; user defined
		// Returns: none
		void tokenRing(
			uint8_t deviceAddress,
			char*(*recieveFunction)(),
			void(*sendFunction)(uint8_t));

		// ----------sendMessage------------
		// Send a packet on the network.
		// Parameters:
		//		(uint8_t) destination: destination address
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

	private:

		// Device information
		uint8_t deviceAddress;
		char*(*recieveFunction)();
		void(*sendFunction)(uint8_t);

		// ----------calculateChecksum----------
		// Private helper function for calculating the checksum of a string
		// Parameters:
		//		(char*) input string
		//		(uint8_t) length of input string (since null is an allowed value)
		// Returns: (uint8_t) checksum
		uint8_t calculateChecksum(char* inputString, uint8_t length);

		// ----------clearChecksum----------
		// Private helper function for clearing the tracked checksum
		// Parameters: none
		// Returns: none
		void clearChecksum(void);

		// ----------sendByte----------
		// Private helper function for sending a byte and updating the tracked checksum
		// Parameters: (uint8_t) character to send and XOR into the checksum
		// Returns: (uint8_t) current value of the checksum
		uint8_t sendByte(uint8_t input);

};

#endif
