#include <stdio.h>						// printf()
#include <stdint.h>						// uint8_t_ uint32_t
#include <string.h>						// memset(), memcpy()
#include <stdlib.h>						// srand(), rand()


#define WORD_SIZE 4						// 4 Bytes = 32 bits

#define BLOCK_SIZE (16 * WORD_SIZE)		// 16 words = 16 * 4B = 64B
										// 2**4 words = (2**4+2)B
										// 4 bits for word offset (within the block)
										// 2 bits for byte offset (within the word)
										// => 6 bits for offset
										// or, log_2_(block_size) = log_2_(64) = 6

#define L1_SIZE (256 * BLOCK_SIZE)		// 256 blocks = (256 * 64)B = 16KiB
										// 2**8 blocks => 8 bits for index
										// or, log_2_(numer_of_lines_L1) = log_2_(256) = 8

#define L2_SIZE (512 * BLOCK_SIZE)		// 512 blocks = (512 * 64)B = 32KiB
										// 2**9 blocks => 9 bits for index

#define DRAM_SIZE (1024 * BLOCK_SIZE)	// 1024 blocks = (1024 * 64)B = 64KiB

#define READ_MODE 1
#define WRITE_MODE 2

#define DRAM_READ_TIME 100
#define DRAM_WRITE_TIME 50
#define L1_READ_TIME 1
#define L1_WRITE_TIME 1
#define L2_READ_TIME 10
#define L2_WRITE_TIME 5

#define ADDRESS_WIDTH 32				// word size * 8 bits = 4B * 8b = 32 bits

#define NUM_OF_LINES_L1 256				// total_num_of_blocks / num_of_blocks_per_line
										// direct-mapped => 1 block/line
										// 256 blocks / 1 block/line = 256 lines

#define NUM_OF_SETS_L2 256				// N-way set associate => num_sets = num_blocks/N
										// num_sets = 512 blocks / 2 blocks/set = 256 sets
										// 2**8 sets => 8 bits for index
										// or, log_2_(numer_of_lines_L1) = log_2_(256) = 8
#define ASSOCIATIVITY 2


/// @brief a cache line
typedef struct cache_line {
	uint8_t valid;
	uint8_t dirty;
	uint32_t tag;
	uint8_t data[BLOCK_SIZE];
} CacheLine;

/// @brief direct-mapped L1 cache
typedef struct cache_l1 {
	uint32_t offset_width;
	uint32_t index_width;
	uint32_t tag_width;
	CacheLine lines[NUM_OF_LINES_L1];
} CacheL1;

typedef struct cache_set {
	uint8_t lru;						// index for the LeastRecentlyUsed line
	CacheLine lines[ASSOCIATIVITY];		// one set has 2 lines
} CacheSet;

/// @brief 2-way set associative L2 cache
/// @note cache where each line can store 2 blocks (2 caches)
/// @note collection of sets
/// @note each set has 2 blocks
/// @note lest recently used (lru) policy: we evict the lru block
typedef struct cache_l2 {
	uint32_t offset_width;
	uint32_t index_width;
	uint32_t tag_width;
	CacheSet sets[NUM_OF_SETS_L2];		// 256 sets, each with 2 lines => 2 blocks
} CacheL2;


void access_cache_L1(uint32_t address, uint8_t *data, int mode);
void access_cache_L2(uint32_t address, uint8_t *data, int mode);
void access_main_memory(uint32_t address, uint8_t *data, int mode);
void init_cache();
uint32_t binary_log(uint32_t n);
void reset_time();
uint32_t get_time();


CacheL1 cache_L1;
CacheL2 cache_L2;
uint8_t main_memory[DRAM_SIZE];
uint32_t time;


/// @brief simulates an access to the L1 cache with write-back policy.
/// @param address address to read/write data from
/// @param data data read/written at address
/// @param mode memory access mode (read or write)
void access_cache_L1(uint32_t address, uint8_t *data, int mode) {
	uint32_t tag, index, offset;

	offset = address << (cache_L1.tag_width + cache_L1.index_width);
	offset = offset >> (cache_L1.tag_width + cache_L1.index_width);
	index = address << cache_L1.tag_width;
	index = index >> (cache_L1.tag_width + cache_L1.offset_width);
	tag = address >> (cache_L1.offset_width + cache_L1.index_width);

	// the current offset indicates the selected byte in the block out of 64 bytes (0-63)
	// since we want to read/write one WORD (4 bytes)
	// we need to make sure that the offset indicates the first byte of that word
	// 
	// e.g. address = 243 = 0x 00 00 00 F4 = 0b 0...0 1111 0011
	// offset width = 6 bits => offset = 0b 11 0011 = 51 => byte index = 51 (0-63)
	// word offset = 0b 1100 = 12 , byte offset within the word = 0b 0011 = 3
	//
	// offset = 51 must be turned into 48 to be the first byte of the word
	// | word 0  | ... |      word 11     |      word 12     |     word 13      | ... |      word 15     |
	// | 0 1 2 3 | ... | (44)(45)(46)(47) | (48)(49)(50)(51) | (52)(53)(54)(55) | ... | (60)(61)(62)(63) |
	if (offset % 4 != 0) { while (offset % 4 != 0) { offset--; } }


	// memory hierarchy with L1, L2 and RAM and write-back policy
	//
	// example for write operations
	// 1 - mem. hierarchy with just L1, RAM
	//		* L1 hit	=> write the data to the block in L1
	//					=> mark line as dirty in L1 
	//		* L1 miss	=> if line is dirty we copy dirty block to RAM
	//					=> fetch block from RAM and copy it to L1
	//					=> write the data to the block in L1
	//					=> mark line as dirty in L1
	//
	// 2- mem. hierarchy with L1, L2, RAM
	//		* L1 hit	=> write the data to the block in L1
	//					=> mark line as dirty in L1
	//		* L1 miss	=> if line is dirty we copy dirty block to L2
	//			* L2 hit	=> fetch block from L2 and copy it to L1
	//						=> write the data to the block in L1
	//						=> mark line as dirty in L1
	//			* L2 miss	=> fetch block from RAM and copy it to L1 (and L2?)
	//						=> write the data to the block in L1
	//						=> mark line as dirty in L1
	//
	// cache hierarchy inclusive (both caches contain the same block)
	// => L1 and L2 miss - the data block is typically copied from RAM into both L1 and L2 to ensure future hits.


	// L1 hit
	// read/write one word from the block of data
	if (cache_L1.lines[index].valid && cache_L1.lines[index].tag == tag) {
		if (mode == READ_MODE) {
			memcpy(data, &(cache_L1.lines[index].data[offset]), WORD_SIZE);
			time += L1_READ_TIME;
			//printf("R - cache hit.\taddress: %u\tdata read: %u\ttime: %u\n", address, *data, time);
		}
		else {
			if (cache_L1.lines[index].dirty) {
				// note that if this line is dirty
				// i.e. it was written something in it and not in RAM
				// we don't need to update RAM since this block is only getting modified and not replaced!
			}		
			memcpy(&(cache_L1.lines[index].data[offset]), data, WORD_SIZE);
			cache_L1.lines[index].dirty = 1;
			time += L1_WRITE_TIME;
			//printf("W - cache hit.\taddress: %u\tdata written: %u\ttime: %u\n", address, *data, time);
		}
	}

	// L1 miss
	// follow the hierarchy and apply the principle of locality
	else {
		int was_dirty = 0;
		uint8_t temp[BLOCK_SIZE], write_buffer[BLOCK_SIZE];

		// there was a miss in this line
		// => this line was valid but with a diff tag OR this line was not valid
		// we are going to replace OR introduce a block in this line

		// if this line is dirty we need to copy it to L2 before replacing it
		// we can only copy this block to L2 after we access L2 to get the new block
		if (cache_L1.lines[index].dirty) {
			was_dirty = 1;
			memcpy(write_buffer, cache_L1.lines[index].data, BLOCK_SIZE);	// copy dirty block
		}

		// start replacing the current line
		cache_L1.lines[index].valid = 1;
		cache_L1.lines[index].dirty = 0;
		cache_L1.lines[index].tag = tag;

		access_cache_L2(address, temp, READ_MODE);					// get the block the contains the address from L2 or RAM
		memcpy(&(cache_L1.lines[index].data[0]), temp, BLOCK_SIZE);	// and put it the current line

		// now we read/write the desired word from the block
		if (mode == READ_MODE) {
			memcpy(data, &(cache_L1.lines[index].data[offset]), WORD_SIZE);
			time += L1_READ_TIME;
		}
		else {
			memcpy(&(cache_L1.lines[index].data[offset]), data, WORD_SIZE);
			cache_L1.lines[index].dirty = 1;	// set this line as dirty to indicate that something new was written to it and not to RAM
			time += L1_WRITE_TIME;
		}

		if (was_dirty) { access_cache_L2(address, write_buffer, WRITE_MODE); }
	}
}

/// @brief simulates an access to the L2 cache with write-back policy.
/// @param address address to read/write data from
/// @param data data read/written at address
/// @param mode memory access mode (read or write)
void access_cache_L2(uint32_t address, uint8_t *data, int mode) {
	int i, miss = 1;
	uint32_t tag, index, offset;

	offset = address << (cache_L2.tag_width + cache_L2.index_width);
	offset = offset >> (cache_L2.tag_width + cache_L2.index_width);
	index = address << cache_L2.tag_width;
	index = index >> (cache_L2.tag_width + cache_L2.offset_width);
	tag = address >> (cache_L2.offset_width + cache_L2.index_width);

	if (offset % 4 != 0) { while (offset % 4 != 0) { offset--; } }

	// iterate over the set specified by the index to look for a L2 hit
	for (i = 0; i < ASSOCIATIVITY; i++) {
		// L1 miss + L2 hit
		if (cache_L2.sets[index].lines[i].valid && cache_L2.sets[index].lines[i].tag == tag) {
			if (mode == READ_MODE) {
				memcpy(data, &(cache_L2.sets[index].lines[i].data[offset]), WORD_SIZE);
				time += L2_READ_TIME;
			}
			else {
				memcpy(&(cache_L2.sets[index].lines[i].data[offset]), data, WORD_SIZE);
				cache_L2.sets[index].lines[i].dirty = 1;
				time += L2_WRITE_TIME;
			}

			// the other line is now the LRU
			cache_L2.sets[index].lru = (i + 1) % 2;
			miss = 0;
			break;
		}
	}	

	// L1 miss + L2 miss
	// follow the hierarchy and apply the principle of locality
	if (miss) {
		uint8_t temp[BLOCK_SIZE], i;

		// get the index that points to the LRU line in the set 
		i = cache_L2.sets[index].lru;

		// if the block to be replaced is dirty
		// update in RAM before replacing the block from line[i]
		if (cache_L2.sets[index].lines[i].dirty) {
			access_main_memory(address, cache_L2.sets[index].lines[i].data, WRITE_MODE);
		}

		cache_L2.sets[index].lru = (i + 1) % 2;		// indicate that the other line is now the LRU
		cache_L2.sets[index].lines[i].valid = 1;
		cache_L2.sets[index].lines[i].dirty = 0;
		cache_L2.sets[index].lines[i].tag = tag;

		// get the block that contains the address
		access_main_memory(address, temp, READ_MODE);
		// and put it the current line
		memcpy(&(cache_L2.sets[index].lines[i].data[0]), temp, BLOCK_SIZE);

		if (mode == READ_MODE) {
			memcpy(data, &(cache_L2.sets[index].lines[i].data[offset]), WORD_SIZE);
			time += L2_READ_TIME;
		}
		else {
			memcpy(&(cache_L2.sets[index].lines[i].data[offset]), data, WORD_SIZE);
			cache_L2.sets[index].lines[i].dirty = 1;
			time += L2_WRITE_TIME;
		}
	}
		
}

/// @brief simulates an access to RAM
/// @param address address to read/write data from
/// @param data data as a block (64 bytes) that was read/written at address
/// @param mode memory access mode (read or write)
void access_main_memory(uint32_t address, uint8_t *data, int mode) {
	if (mode == READ_MODE) {
		memcpy(data, &(main_memory[address]), BLOCK_SIZE);
		time += DRAM_READ_TIME;
	}
	else {
		// used only when the block in L1 is getting replaced
		memcpy(&(main_memory[address]), data, BLOCK_SIZE);
		time += DRAM_WRITE_TIME;
	}
}

/// @brief initializes the cache structs for both L1 and L2 caches
/// @note calculates the offset, index and tag widths
/// @note invalidates all cache lines, marking them as not valid and not dirt
/// @note clears the data (block) in each cache line to 0
void init_cache() {
	int i, j;

	cache_L1.offset_width = binary_log(BLOCK_SIZE);
	cache_L1.index_width = binary_log(NUM_OF_LINES_L1);
	cache_L1.tag_width = ADDRESS_WIDTH - cache_L1.offset_width - cache_L1.index_width; 
	for (i = 0; i < NUM_OF_LINES_L1; i++) {
		cache_L1.lines[i].valid = 0;
		cache_L1.lines[i].dirty = 0;
		cache_L1.lines[i].tag = 0;
		memset(cache_L1.lines[i].data, 0, BLOCK_SIZE);
	}

	cache_L2.offset_width = binary_log(BLOCK_SIZE);
	cache_L2.index_width = binary_log(NUM_OF_SETS_L2);
	cache_L2.tag_width = ADDRESS_WIDTH - cache_L2.offset_width - cache_L2.index_width; 
	for (i = 0; i < NUM_OF_SETS_L2; i++) {
		cache_L2.sets[i].lru = 0;
		for (j = 0; j < ASSOCIATIVITY; j++) {
			cache_L2.sets[i].lines[j].valid = 0;
			cache_L2.sets[i].lines[j].dirty = 0;
			cache_L2.sets[i].lines[j].tag = 0;
			memset(cache_L2.sets[i].lines[j].data, 0, BLOCK_SIZE);
		}
	}
}

int main() {
	int clock1, value;

	srand(0);
	memset(main_memory, 0, DRAM_SIZE);

	for(int n = 1; n <= DRAM_SIZE/4; n*=WORD_SIZE) {
		reset_time();
		init_cache();

		printf("\nNumber of words: %d\n", (n-1)/WORD_SIZE + 1);

		for(int i = 0; i < n; i+=WORD_SIZE) {
			access_cache_L1(i, (unsigned char *)(&i), WRITE_MODE);
			clock1 = get_time();
			printf("Write; Address %d; Value %d; Time %d\n", i, i, clock1);
		}
		for(int i = 0; i < n; i+=WORD_SIZE) {
			access_cache_L1(i, (unsigned char *)(&value), READ_MODE);
			clock1 = get_time();
			printf("Read; Address %d; Value %d; Time %d\n", i, value, clock1);
		}  
	}

	printf("\nRandom accesses\n");
	for(int i = 0; i < 100; i++) {
		int address = rand() % (DRAM_SIZE/4);
		address = address - address % WORD_SIZE;
		int mode = rand() % 2;
		
		if (mode == READ_MODE) {
			access_cache_L1(address, (unsigned char *)(&value), READ_MODE);
			clock1 = get_time();
			printf("Read; Address %d; Value %d; Time %d\n", address, value, clock1);
		}
		else {
			access_cache_L1(address, (unsigned char *)(&address), WRITE_MODE);
			clock1 = get_time();
			printf("Write; Address %d; Value %d; Time %d\n", address, address, clock1);
		}
	}
	return 0;
}

/// @brief computes the binary logarithm of a given number that is a power of two
/// @param n number to compute
/// @return result of the computation
uint32_t binary_log(uint32_t n) {
    uint32_t p = 0;

    if (n == 0) { return 1; }  // Special case for 0
    while (n > 1) {
        n >>= 1;   // Divide n by 2 using bitwise right shift
        p++;       // Increment the exponent
    }
    return p;       // Return the result
}

void reset_time() { time = 0; }

uint32_t get_time() { return time; }
