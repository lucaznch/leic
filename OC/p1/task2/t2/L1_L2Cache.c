#include "Cache.h"
#include "L1_L2Cache.h"

// Global variables
uint32_t time;
uint8_t DRAM[DRAM_SIZE];
Cache cache_L1;
CacheL2 cache_L2;

/**
 * @brief Initialize the L2 cache by setting all lines to invalid and clean.
 */
void initCacheL2() {
	memset(&cache_L2, 0, sizeof(CacheL2)); // set all bytes to zero
	cache_L2.init = 1;
}

/**
 * @brief Initialize the L1 cache by setting all lines to invalid and clean.
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
 * @param numLines The number of lines in the cache.
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
 * @param numLines The number of lines in the cache.
 * @return The calculated memory address.
 */
uint32_t calculateAddress(uint32_t tag, uint32_t index, uint32_t offset, int numLines) {
    return (tag * (BLOCK_SIZE * numLines)) + (index * BLOCK_SIZE) + offset;
}

/**
 * @brief Access the DRAM to read or write data.
 * @param address The address in DRAM to access.
 * @param data Pointer to the data buffer to read into or write from.
 * @param mode The mode of operation.
 */
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {
	if (address >= DRAM_SIZE) { // out of bounds
		exit(EXIT_FAILURE);
	}

	if (mode == MODE_READ) { // read from DRAM
		memcpy(data, &DRAM[address], BLOCK_SIZE); 
		time += DRAM_READ_TIME;
	} else if (mode == MODE_WRITE) { // write to DRAM
		memcpy(&DRAM[address], data, BLOCK_SIZE); 
		time += DRAM_WRITE_TIME;
	}
}

/**
 * @brief Access the L2 cache to read or write data.
 * @param address The address to access in the L2 cache.
 * @param data Pointer to the data buffer to read into or write from.
 * @param mode The mode of operation.
 */
void accessL2(uint32_t address, uint8_t *data, uint32_t mode) {
	uint32_t index, tag, offset, memAddress = address;
	CacheLine *refLine;

	if (!cache_L2.init) {
		initCacheL2(); // initialize if not done
	}

	calculateTagIndexOffset(address, L2_LINES, &tag, &index, &offset); // get the tag, index, and offset

	refLine = &cache_L2.lines[index];

	if (!refLine->valid || refLine->tag != tag) { // cache miss
		uint8_t tempBlock[BLOCK_SIZE];

		resetOffset(&memAddress); // reset the offset to get the memory address
		accessDRAM(memAddress, tempBlock, MODE_READ); // load the requested data from DRAM

		if (refLine->valid && refLine->dirty) { // if the line was dirty, write back to DRAM
			uint32_t dirtyAddress;
			dirtyAddress = calculateAddress(refLine->tag, index, 0, L2_LINES);
			accessDRAM(dirtyAddress, refLine->block, MODE_WRITE);
		}

		memcpy(refLine->block, tempBlock, BLOCK_SIZE); // copy the new block to the cache
		
		refLine->valid = 1;  // mark line as valid
		refLine->dirty = 0;  // mark line as clean
		refLine->tag = tag;  // update tag
	}

	if (mode == MODE_READ) { // read from l2 cache
		memcpy(data, &(refLine->block[offset]), WORD_SIZE);
		time += L2_READ_TIME;
	} else if (mode == MODE_WRITE) { // write from l2 cache
		memcpy(&(refLine->block[offset]), data, WORD_SIZE);
		time += L2_WRITE_TIME;
		refLine->dirty = 1;
	}
}

/**
 * @brief Access the L1 cache to read or write data.
 * @param address The address to access in the L1 cache.
 * @param data Pointer to the data buffer to read into or write from.
 * @param mode The mode of operation.
 */
void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
	uint32_t index, tag, offset, memAddress = address;
	CacheLine *refLine;

	if (!cache_L1.init) {
		initCache(); // initialize if not done
	}

	calculateTagIndexOffset(address, L1_LINES, &tag, &index, &offset); // get the tag, index, and offset

	refLine = &cache_L1.lines[index];

	if (!refLine->valid || refLine->tag != tag) { // cache miss
		uint8_t tempBlock[BLOCK_SIZE];

		accessL2(address, tempBlock, MODE_READ); // load the requested data from the l2 cache

		if (refLine->valid && refLine->dirty) { // if line was dirty, write back to the l2 cache
			uint32_t dirtyAddress;
			dirtyAddress = calculateAddress(refLine->tag, index, 0, L2_LINES);
			accessL2(dirtyAddress, refLine->block, MODE_WRITE);
		}

		memcpy(refLine->block, tempBlock, BLOCK_SIZE); // copy the new block to cache line
		
		refLine->valid = 1;  // mark line as valid
		refLine->dirty = 0;  // mark line as clean
		refLine->tag = tag;  // update tag
	}

	if (mode == MODE_READ) { // read from l1 cache
		memcpy(data, &(refLine->block[offset]), WORD_SIZE);
		time += L1_READ_TIME;
	} else if (mode == MODE_WRITE) { // write from l1 cache
		memcpy(&(refLine->block[offset]), data, WORD_SIZE);
		time += L1_WRITE_TIME;
		refLine->dirty = 1;
	}
}

/**
 * @brief Read data from a specified address starting with the L1 cache.
 * @param address The address to read from.
 * @param data Pointer to the data buffer to read into.
 */
void read(uint32_t address, uint8_t *data) {
	accessL1(address, data, MODE_READ);
}

/**
 * @brief Write data to a specified address starting with the L1 cache.
 * @param address The address to write to.
 * @param data Pointer to the data buffer to write from.
 */
void write(uint32_t address, uint8_t *data) {
	accessL1(address, data, MODE_WRITE);
}
