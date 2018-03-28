/*
    Arduino network driver

    For use with the Serial Token Ring communication protocol
    https://github.com/thetianshuhuang/serial-token-ring
*/

class arduinoTokenRing : public tokenRing {

    public: 

        // ----------Constructor----------
        // Create a token ring object with arduino hardware configuration
        // Parameters used:
        //      Baudrate - 115200
        //      Serial port - Port 0
        //      In function - arduino default
        //      Out function - arduino default
        arduinoTokenRing();


        // ----------Update function----------
        // Checks if the serial buffer is not empty
        // Calls updateNetwork until empty
        updateTokenRing();

}