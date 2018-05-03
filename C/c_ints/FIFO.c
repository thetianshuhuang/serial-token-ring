/*
 * fifo.c
 *
 * FIFO queues for UART TX and RX
 */

#include "fifo.h"

// ----------fifoInit----------
// Initialize a software fifo.
// Parameters:
//      *FIFO_QUEUE fifo: FIFO queue to initialize
void fifoInit(FIFO_QUEUE *fifo) {
    fifo->getIndex = 0;
    fifo->putIndex = 0;
    fifo->size = 0;
}


// ----------fifoPut----------
// Adds an element to the FIFO
// Parameters:
//      *FIFO_QUEUE fifo: FIFO queue to push to
//      char data: character to add to FIFO
// Returns:
//      uint8_t: 1 for success, 0 for failure
uint8_t fifoPut(FIFO_QUEUE *fifo, char data) {
    if(fifo->size + 1 > _FIFO_SIZE) {
        return(0);
    }
    fifo->size += 1;
    (fifo->buffer)[fifo->putIndex] = data;
    fifo->putIndex += 1;
    fifo->putIndex %= _FIFO_SIZE;
    return(1);
}


// ----------fifoGet----------
// Gets an element from the FIFO
// Parameters:
//      *FIFO_QUEUE fifo: FIFO queue to pop from
//      *datapt: pointer to character to fill
// Returns:
//      uint8_t: 1 for success, 0 for failure
uint8_t fifoGet(FIFO_QUEUE *fifo, char *datapt) {
    if(fifo->size == 0) {
        return(0);
    }
    *datapt = (fifo->buffer)[fifo->getIndex];
    fifo->getIndex += 1;
    fifo->getIndex %= _FIFO_SIZE;
    fifo->size -= 1;
    return(1);
}
