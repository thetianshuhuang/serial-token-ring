/*
    Arduino network driver

    For use with the Serial Token Ring communication protocol
    https://github.com/thetianshuhuang/serial-token-ring
*/

#ifndef ARDUINO_DRIVER_H
#define ARDUINO_DRIVER_H

#include "../cpp_client.hpp"
#include "arduino.h"

class arduinoTokenRing : public tokenRing {

    public: 

        // ----------Constructor----------
        // Create a token ring object with arduino hardware configuration
        // Parameters: (uint8_t) address - network address of the Arduino
        // Parameters used:
        //      Baudrate - 115200
        //      Serial port - Port 0
        //      In function - arduino default
        //      Out function - arduino default
        void arduinoTokenRing(uint8_t address);


        // ----------Update function----------
        // Checks if the serial buffer is not empty
        // Calls updateNetwork until empty
        // Returns:
        //      char*, where the first char specifies the length
        char* updateTokenRing(void);

}

#endif
