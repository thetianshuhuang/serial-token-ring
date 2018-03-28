/*
    Arduino network driver example implementation

    For use with the Serial Token Ring communication protocol
    https://github.com/thetianshuhuang/serial-token-ring
*/

#include "arduino_driver.hpp"


char* recieveBuffer;

int main() {

    pinMode(LED_BUILTIN, OUTPUT);
    uint8_t ledState = 0x00;

    arduinoTokenRing network(0xAA);

    while(1) {
        recieveBuffer = network.updateTokenRing();
        if(recieveBuffer[0] != 0) {
            analogwRITE(LED_BUILTIN, ledState);
            ledState = ~ledState;
        }
    }
}