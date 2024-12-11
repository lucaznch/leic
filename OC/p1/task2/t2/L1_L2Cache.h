#ifndef L1_L2CACHE_H
#define L1_L2CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Cache.h"

// Define the number of lines in the L1 and L2 caches
#define L1_LINES (L1_SIZE / BLOCK_SIZE)
#define L2_LINES (L2_SIZE / BLOCK_SIZE)

// Cache line structure for both L1 and L2 caches
typedef struct CacheLine {
    uint8_t valid;
    uint8_t dirty;
    uint32_t tag;
    uint8_t block[BLOCK_SIZE]; // data block
} CacheLine;

// Structure for L1 cache
typedef struct Cache {
    uint32_t init; // initialization flag
    CacheLine lines[L1_LINES]; // L1 cache lines
} Cache;

// Structure for L2 cache
typedef struct CacheL2 {
    uint32_t init; // initialization flag
    CacheLine lines[L2_LINES]; // L2 cache lines
} CacheL2;

// Global variables
extern uint32_t time;
extern uint8_t DRAM[DRAM_SIZE];
extern Cache cache_L1;
extern CacheL2 cache_L2;

/*********************** Cache Functions *************************/

// Initialize the L1 cache
void initCacheL1();

// Initialize the L2 cache
void initCacheL2();

// Initialize both L1 and L2 caches
void initCache();

// Reset the global time variable
void resetTime();

// Get the current value of the global time variable
uint32_t getTime();

// Reset a memory address to the nearest block boundary
void resetOffset(uint32_t* memAddress);

// Calculate tag, index, and offset from a memory address
void calculateTagIndexOffset(uint32_t address, int numLines, uint32_t* tag, uint32_t* index, uint32_t* offset);

// Calculate the memory address from tag, index, and offset
uint32_t calculateAddress(uint32_t tag, uint32_t index, uint32_t offset, int numLines);

/*********************** DRAM Functions *************************/

// Access the DRAM to read or write data
void accessDRAM(uint32_t address, uint8_t* data, uint32_t mode);

/*********************** L2 Cache Functions *************************/

// Access the L2 cache to read or write data
void accessL2(uint32_t address, uint8_t* data, uint32_t mode);

/*********************** L1 Cache Functions *************************/

// Access the L1 cache to read or write data
void accessL1(uint32_t address, uint8_t* data, uint32_t mode);

/*********************** Read/Write Interfaces *************************/

// Read data from the L1 cache
void read(uint32_t address, uint8_t* data);

// Write data to the L1 cache
void write(uint32_t address, uint8_t* data);

#endif // L1_L2CACHE_H
