#include "L1Cache.h"

// Global variables
uint32_t time;
uint8_t DRAM[DRAM_SIZE];
Cache cache_L1;

/**
 * @brief Initialize the L1 cache.
 */
void initCache() {
	memset(&cache_L1, 0, sizeof(Cache));
	cache_L1.init = 1;
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
	if (address >= DRAM_SIZE - BLOCK_SIZE + 1) {
		exit(-1);
	}

	if (mode == MODE_READ) {
		memcpy(data, &(DRAM[address]), BLOCK_SIZE);
		time += DRAM_READ_TIME;
	}

	if (mode == MODE_WRITE) {
		memcpy(&(DRAM[address]), data, BLOCK_SIZE);
		time += DRAM_WRITE_TIME;
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

	if (cache_L1.init == 0) {
		initCache();
	}

	calculateTagIndexOffset(address, L1_LINES, &tag, &index, &offset);

	refLine = &cache_L1.lines[index];

	if (refLine->valid == 0 || refLine->tag != tag) { // miss
		uint8_t tempBlock[BLOCK_SIZE];

		resetOffset(&memAddress);
		accessDRAM(memAddress, tempBlock, MODE_READ);

		if ((refLine->valid) && (refLine->dirty)) {
			uint32_t dirtyAddress;
			dirtyAddress = calculateAddress(refLine->tag, index, 0, L1_LINES);
			accessDRAM(dirtyAddress, refLine->block, MODE_WRITE);
		}

		memcpy(refLine->block, tempBlock, BLOCK_SIZE); // copy new block to cache refLine
		refLine->valid = 1;  // set the refLine as valid
		refLine->dirty = 0;  // set the refLine as clean
		refLine->tag = tag;  // set the new tag
	}

	if (mode == MODE_READ) { // read from the L1
		memcpy(data, &(refLine->block[offset]), WORD_SIZE);
		time += L1_READ_TIME;
	}

	if (mode == MODE_WRITE) { // write to the L1
		memcpy(&(refLine->block[offset]), data, WORD_SIZE);
		time += L1_WRITE_TIME;
		refLine->dirty = 1; // set the refLine as dirty (write-back policy)
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
