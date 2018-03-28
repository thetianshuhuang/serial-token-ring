/*
    TM4C network driver example implementation

    For use with the Serial Token Ring communication protocol
    https://github.com/thetianshuhuang/serial-token-ring
*/

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1

#include <stdint.h>
#include "PLL.h"
#include "UART.h"
#include "tm4c_client.h"

int main(void){
    
    UART_Init();
    
}
