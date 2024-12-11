#ifndef L1CACHE_H
#define L1CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Cache.h"

// Define the number of lines in the L1 cache
#define L1_LINES (L1_SIZE / BLOCK_SIZE)

// Structures for L1 cache and cache lines
typedef struct CacheLine {
    uint8_t valid;
    uint8_t dirty;
    uint32_t tag;
    uint8_t block[BLOCK_SIZE]; // data block
} CacheLine;

typedef struct Cache {
    uint32_t init; // initialization flag
    CacheLine lines[L1_LINES]; // cache lines
} Cache;

// Global variables
extern uint32_t time;
extern uint8_t DRAM[DRAM_SIZE];
extern Cache cache_L1;

/*********************** Cache Functions *************************/

// Initialize the L1 cache
void initCache();

// Reset the global time
void resetTime();

// Return the current value of the global time variable
uint32_t getTime();

// Reset the memory address to the nearest block boundary
void resetOffset(uint32_t* memAddress);

// Calculate tag, index, and offset from a memory address
void calculateTagIndexOffset(uint32_t address, int numLines, uint32_t* tag, uint32_t* index, uint32_t* offset);

// Calculate the memory address from the tag, index, and offset
uint32_t calculateAddress(uint32_t tag, uint32_t index, uint32_t offset, int numLines);

/*********************** DRAM Functions *************************/

// Access the DRAM to read or write data
void accessDRAM(uint32_t address, uint8_t* data, uint32_t mode);

/*********************** L1 Cache Functions *************************/

// Access the L1 cache to read or write data
void accessL1(uint32_t address, uint8_t* data, uint32_t mode);

/*********************** Read/Write Interfaces *************************/

// Read data from the L1 cache
void read(uint32_t address, uint8_t* data);

// Write data to the L1 cache
void write(uint32_t address, uint8_t* data);

#endif // L1CACHE_H
