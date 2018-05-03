/*
	C client framework for the serial token ring network protocol

	For use with the Serial Token Ring communication protocol
	https://github.com/thetianshuhuang/serial-token-ring
    
    Modified for Lab 10 (n-player Rock Band)
*/

#include <stdint.h>
#include "c_client.h"


uint8_t currentCheckSum;
struct ipInfo configuration;


enum section {INIT, SRC, DST, SIZE, MSG, CHK};
enum section currentSection;
uint8_t source;
uint8_t destination;
uint8_t length;
uint8_t bytesRemaining;
uint8_t recieveBuffer[255];

void DisableInterrupts(void);   // Disable interrupts
void EnableInterrupts(void);    // Enable interrupts


// ----------calculateChecksum----------
// Private helper function for calculating the checksum of a string
// Parameters:
//		(char*) input string
//		(uint8_t) length of input string (since null is an allowed value)
// Returns: (uint8_t) checksum
uint8_t calculateChecksum(uint8_t* inputString, uint8_t length) {
	uint8_t checksum = 0;
	for(uint16_t i = 0; i < length; i++) {
		checksum ^= inputString[i];
	}
	return(checksum);
}


// ----------clearChecksum----------
// Private helper function for clearing the tracked checksum
// Parameters: none
// Returns: none
void clearChecksum(void) {
	currentCheckSum = 0;
	return;
}


// ----------sendByte----------
// Private helper function for sending a byte and updating the tracked checksum
// Parameters: (uint8_t) character to send and XOR into the checksum
// Returns: (uint8_t) current value of the checksum
uint8_t sendByte(uint8_t input) {
	currentCheckSum ^= input;
	configuration.sendFunction(input);
	return(input);
}


// ----------ipConfig----------
// Configure the token ring network
// Parameters:
// 		(uint8_t) device address (7-bit only)
//		char*(*recieveFunction)() recieve function; user defined
//		void(*sendFunction)(char*)	send function; user defined
// Returns: none
void ipConfig(uint8_t deviceAddress, void(*sendFunction)(uint8_t)) {
	configuration.deviceAddress = deviceAddress;
	configuration.sendFunction = sendFunction;
}


// ----------sendMessage------------
// Send a packet on the network.
// Parameters:
//		(uint8_t) destination
//		(char*) message to send
//		(uint8_t) length of message
// Returns: none
void sendMessage(uint8_t destination, uint8_t* message, uint8_t length) {
    
    // Start critical
    DisableInterrupts();
	// Clear checksum
	clearChecksum();
    
	// Start message (don't update checksum)
	configuration.sendFunction(0xAA);
	// Send address
	sendByte(configuration.deviceAddress);
	sendByte(destination);
	// Send size
	sendByte(length);
	// Send main message
	for(uint8_t i = 0; i < length; i++) {
		sendByte(message[i]);
	}
	// Send checksum (don't update checksum obviously)
	configuration.sendFunction(currentCheckSum);

    // End critical
    EnableInterrupts();
}


// ----------updateNetwork----------
// Updates the token ring network
// should be called whenever a character is recieved
// Parameters:
//      uint8_t rx: character to update the network with
uint8_t* updateNetwork(uint8_t rx) {

    // No incoming bytes yet
    if(currentSection == INIT) {
        if(rx == 0xAA) {
            currentSection = SRC;
            length = 0;
        }
        // Set the return buffer length to 0
        recieveBuffer[0] = 0;
    }
    // Read SRC address
    else if(currentSection == SRC) {
        recieveBuffer[length] = rx;
        currentSection = DST;
    }
    // Read DST address
    else if(currentSection == DST) {
        recieveBuffer[length] = rx;
        currentSection = SIZE;
    }
    // Get size
    else if(currentSection == SIZE) {
        bytesRemaining = rx;
        recieveBuffer[length] = rx;
        currentSection = MSG;
    }
    // Get Message
    else if(currentSection == MSG) {
        if(bytesRemaining > 0) {
            recieveBuffer[length] = rx;
            bytesRemaining -= 1;
        }
        // Read CHK
        else {
            // Discard invalid checksum
            if(calculateChecksum(recieveBuffer, length) !=
               recieveBuffer[length])
            {
                return(recieveBuffer);
            }
            // Forward packet if SRC, DST not reached
            if(recieveBuffer[1] != configuration.deviceAddress && recieveBuffer[2] !=
               configuration.deviceAddress)
            {
                configuration.sendFunction(0xAA);
                for(uint8_t i = 1; i<=length; i++) {
                    configuration.sendFunction(recieveBuffer[i]);
                }
            }
            // Return message if DST matches or is broadcast
            if(recieveBuffer[2] == 0x00 ||
               recieveBuffer[2] == configuration.deviceAddress)
            {
                recieveBuffer[0] = length;
                return(recieveBuffer);
            }

        }
    }

    // The recieve buffer is indexed starting at 1
    // This is so the return array can use the first byte as a length indicator
    length += 1;

    // Return if the packet is incomplete
    return(recieveBuffer);
}
