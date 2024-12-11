#include "L1_L2_2WCache.h"

// Global variables
uint32_t time;
uint8_t DRAM[DRAM_SIZE];
Cache cache_L1;
CacheL2 cache_L2;

/**
 * @brief Initialize the L2 cache by setting all ways to invalid and clean.
 */
void initCacheL2() {
	memset(&cache_L2, 0, sizeof(CacheL2)); // set all bytes to zero
	cache_L2.init = 1;
}

/**
 * @brief Initialize the L1 cache by setting all ways to invalid and clean.
 */
void initCacheL1() {
	memset(&cache_L1, 0, sizeof(Cache)); // set all bytes to zero
	cache_L1.init = 1;
}

/**
 * @brief Initialize both L1 and L2 caches.
 */
void initCache() {
	initCacheL1();
	initCacheL2();
}

/**
 * @brief Reset the global time variable to zero.
 */
void resetTime() {
	time = 0;
}

/**
 * @brief Get the current value of the global time variable.
 * @return The current time.
 */
uint32_t getTime() {
	return time;
}

/**
 * @brief Reset the memory address to the nearest block boundary.
 * @param memAddress A pointer to the memory address to reset.
 */
void resetOffset(uint32_t* memAddress) {
	(*memAddress) &= ~(BLOCK_SIZE - 1); // clear the lower bits
}

/**
 * @brief Calculate the tag, index, and offset from a given memory address.
 * @param address The memory address to analyze.
 * @param numLines The number of ways in the cache.
 * @param tag Pointer to store the calculated tag.
 * @param index Pointer to store the calculated index.
 * @param offset Pointer to store the calculated offset.
 */
void calculateTagIndexOffset(uint32_t address, int numLines, uint32_t* tag, uint32_t* index, uint32_t* offset) {
	*offset = address % BLOCK_SIZE;
	*index = (address / BLOCK_SIZE) % numLines;
	*tag = address / (BLOCK_SIZE * numLines);
}

/**
 * @brief Calculate the memory address from the tag, index, and offset.
 * @param tag The tag part of the address.
 * @param index The index part of the address.
 * @param offset The offset part of the address.
 * @param numLines The number of ways in the cache.
 * @return The calculated memory address.
 */
uint32_t calculateAddress(uint32_t tag, uint32_t index, uint32_t offset, int numLines) {
    return (tag * (BLOCK_SIZE * numLines)) + (index * BLOCK_SIZE) + offset;
}

/**
 * @brief Access the DRAM to read or write data.
 */
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {
    if (address >= DRAM_SIZE) {
        exit(EXIT_FAILURE);
    }

    if (mode == MODE_READ) {
        memcpy(data, &DRAM[address], BLOCK_SIZE); 
        time += DRAM_READ_TIME;
    } else if (mode == MODE_WRITE) {
        memcpy(&DRAM[address], data, BLOCK_SIZE); 
        time += DRAM_WRITE_TIME;
    }
}

/**
 * @brief Access the L2 cache to read or write data.
 */
void accessL2(uint32_t address, uint8_t *data, uint32_t mode) {
    uint32_t index, tag, offset, memAddress = address;
    CacheLine *refWay;
    CacheSet *set;

    if (!cache_L2.init) {
        initCache(); // initialize if not done
    }

    calculateTagIndexOffset(address, L2_LINES, &tag, &index, &offset);

    set = &cache_L2.sets[index];
    refWay = &set->ways[0]; // start with the first way in the set
    
    int hitWay = -1;

    for (int i = 0; i < WAYS_PER_SET; i++) { // check both ways in the set
        refWay = &set->ways[i];
        if (refWay->valid && refWay->tag == tag) { // cache hit
            hitWay = i;
            break;
        }
    }

    if (hitWay == -1) { // miss
        uint8_t tempBlock[BLOCK_SIZE];
        resetOffset(&memAddress);
        accessDRAM(memAddress, tempBlock, MODE_READ);

        
        int lruWay = set->lru != 0; // determine which way to replace based on LRU

        if (set->ways[lruWay].valid && set->ways[lruWay].dirty) {
            uint32_t dirtyAddress;
			dirtyAddress = calculateAddress(refWay->tag, index, 0, L2_LINES);
			accessDRAM(dirtyAddress, set->ways[lruWay].block, MODE_WRITE);
        }

        memcpy(set->ways[lruWay].block, tempBlock, BLOCK_SIZE);
        set->ways[lruWay].valid = 1;
        set->ways[lruWay].dirty = 0;
        set->ways[lruWay].tag = tag;

        hitWay = lruWay; // update the hit way to newly loaded way
    }

    set->lru = (hitWay == 0); // update the LRU bit

    if (mode == MODE_READ) {
        memcpy(data, &(set->ways[hitWay].block[offset]), WORD_SIZE);
        time += L2_READ_TIME;
    } else if (mode == MODE_WRITE) {
        memcpy(&(set->ways[hitWay].block[offset]), data, WORD_SIZE);
        time += L2_WRITE_TIME;
        set->ways[hitWay].dirty = 1;
    }
}

/**
 * @brief Access the L1 cache to read or write data.
 */
void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
    uint32_t index, tag, offset, memAddress = address;
    CacheLine *refWay;
    CacheSet *set;

    if (!cache_L1.init) {
        initCache(); // initialize if not done
    }

    calculateTagIndexOffset(address, L1_LINES, &tag, &index, &offset);

    set = &cache_L1.sets[index];
    refWay = &set->ways[0]; // start with the first way in the set
    int hitWay = -1;

    for (int i = 0; i < WAYS_PER_SET; i++) { // check both ways in the set
        refWay = &set->ways[i];
        if (refWay->valid && refWay->tag == tag) { // cache hit
            hitWay = i;
            break;
        }
    }
    
    // If we missed in the cache
    if (hitWay == -1) {
        uint8_t tempBlock[BLOCK_SIZE];
        accessL2(address, tempBlock, MODE_READ);

        // Determine which way to replace based on LRU
        int lruWay = set->lru != 0;

        if (set->ways[lruWay].valid && set->ways[lruWay].dirty) {
            uint32_t dirtyAddress;
			dirtyAddress = calculateAddress(refWay->tag, index, 0, L2_LINES);
			accessL2(dirtyAddress, set->ways[lruWay].block, MODE_WRITE);
        }

        memcpy(set->ways[lruWay].block, tempBlock, BLOCK_SIZE);
        set->ways[lruWay].valid = 1;
        set->ways[lruWay].dirty = 0;
        set->ways[lruWay].tag = tag;

        hitWay = lruWay; // Update hit way to newly loaded way
    }

    // Update LRU status
    set->lru = (hitWay == 0); // Set the LRU bit accordingly

    if (mode == MODE_READ) {
        memcpy(data, &(set->ways[hitWay].block[offset]), WORD_SIZE);
        time += L1_READ_TIME;
    } else if (mode == MODE_WRITE) {
        memcpy(&(set->ways[hitWay].block[offset]), data, WORD_SIZE);
        time += L1_WRITE_TIME;
        set->ways[hitWay].dirty = 1;
    }
}

/**
 * @brief Read data from a specified address starting with the L1 cache.
 */
void read(uint32_t address, uint8_t *data) {
    accessL1(address, data, MODE_READ);
}

/**
 * @brief Write data to a specified address starting with the L1 cache.
 */
void write(uint32_t address, uint8_t *data) {
    accessL1(address, data, MODE_WRITE);
}
