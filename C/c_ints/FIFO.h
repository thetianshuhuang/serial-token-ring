/*
 * fifo.h
 *
 * FIFO queues for UART TX and RX
 */

#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>

// Set FIFO size
#define _FIFO_SIZE 512

// FIFO Data
typedef struct fifo_struct_t {
    char buffer[_FIFO_SIZE];    // Data buffer
    uint16_t putIndex;           // Head index
    uint16_t getIndex;           // Tail index
    uint16_t size;               // Size
} FIFO_QUEUE;


// ----------fifoInit----------
// Initialize a software fifo.
// Parameters:
//      *FIFO_QUEUE fifo: FIFO queue to initialize
void fifoInit(FIFO_QUEUE *fifo);


// ----------fifoPut----------
// Adds an element to the FIFO
// Parameters:
//      *FIFO_QUEUE fifo: FIFO queue to push to
//      char data: character to add to FIFO
// Returns:
//      uint8_t: 1 for success, 0 for failure
uint8_t fifoPut(FIFO_QUEUE *fifo, char data);


// ----------fifoGet----------
// Gets an element from the FIFO
// Parameters:
//      *FIFO_QUEUE fifo: FIFO queue to pop from
//      *datapt: pointer to character to fill
// Returns:
//      uint8_t: 1 for success, 0 for failure
uint8_t fifoGet(FIFO_QUEUE *fifo, char *datapt);

#endif
