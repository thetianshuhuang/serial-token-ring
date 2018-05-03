/*
 * network.h
 *
 * TM4C network driver
 * For use by the Serial Token Ring protocol
 *
 * Written by Tianshu Huang, April 2018 
 */
 
#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>


// ----------networkInit----------
// Initialize the network
// Gets the network address from PC6, PC7, PD6, PD7
void networkInit(void);


// ----------discoverPeers----------
// Run peer discovery
void discoverPeers(void);

#endif
