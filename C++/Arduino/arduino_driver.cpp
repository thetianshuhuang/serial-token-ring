/*
    Arduino network driver

    For use with the Serial Token Ring communication protocol
    https://github.com/thetianshuhuang/serial-token-ring
*/

#include "arduino_driver.hpp"


// ----------Send function----------
void arduinoSend(uint8_t output) {
    Serial.write(output);
    return;
}


// ----------Recieve function----------
uint8_t arduinoRecieve(void) {
    return(Serial.read(1));
}


// ----------Constructor----------
// Create a token ring object with arduino hardware configuration
// Parameters used:
//      Baudrate - 115200
//      Serial port - Port 0
//      In function - arduino default
//      Out function - arduino default
arduinoTokenRing::arduinoTokenRing(uint8_t address) :
    tokenRing(address, &arduinoRecieve, &arduinoSend); 
{
    Serial.begin(115200);
    return;
}


// ----------Update function----------
// Checks if the serial buffer is not empty
// Calls updateNetwork until empty
arduinoTokenRing::updateTokenRing() {
    char* returnBuffer;
    while(Serial.available()) {
        returnBuffer = updateNetwork();
	if(returnBuffer[0] != 0) {
            return(returnBuffer);
	}
    }
    else {
        return("\0");
    }
}
