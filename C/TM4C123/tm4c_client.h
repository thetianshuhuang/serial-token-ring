/*
    TM4C network driver

    For use with the Serial Token Ring communication protocol
    https://github.com/thetianshuhuang/serial-token-ring
*/

#ifndef TM4C_CLIENT_H
#define TM4C_CLIENT_H

#include <stdint.h>


// ----------initTokenRing----------
// Initialize a token ring network
// Parameter: address - network address to use
void initTokenRing(uint8_t address);


// ----------Update function----------
// Checks if the RX fifo is not empty
// Calls updateNetwork until empty
char* updateTokenRing(void);

#endif
