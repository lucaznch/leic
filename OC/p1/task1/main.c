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

#define NUM_OF_LINES_L1 256
#define ADDRESS_WIDTH 32				// word size * 8 bits = 4B * 8b = 32 bits

/*
#define L1_NUM_OF_BLOCKS 256			// total number of blocks in the L1 cache
#define L2_NUM_OF_BLOCKS 512			// total number of blocks in the L2 cache

#define L1_NUM_OF_BLOCKS_PER_LINE 1		// direct-mapped cache we use 1 block per line
#define L2_NUM_OF_BLOCKS_PER_LINE 1		// direct-mapped cache we use 1 block per line

#define NUM_OF_LINES(total_num_of_blocks, num_blocks_per_line) ((total_num_of_blocks) / (num_blocks_per_line))
*/


typedef struct cache_line {
	uint8_t valid;
	uint8_t dirty;
	uint32_t tag;
	uint32_t off;
	uint8_t data[BLOCK_SIZE];
} CacheLine;

typedef struct cache {
	uint32_t offset_width;
	uint32_t index_width;
	uint32_t tag_width;
	CacheLine lines[NUM_OF_LINES_L1];
} Cache;


Cache cache_L1;
uint8_t main_memory[DRAM_SIZE];
uint32_t time;


uint32_t binary_log(uint32_t n) {
    uint32_t log = 0;

	if (n == 0) { return 1; }
    while (n > 1) {
        n >>= 1;	// divide by 2
        log++;
    }
    return log;
}

void reset_time() { time = 0; }

uint32_t get_time() { return time; }

void init_cache() {
	int i;

	cache_L1.offset_width = binary_log(BLOCK_SIZE);
	cache_L1.index_width = binary_log(NUM_OF_LINES_L1);
	cache_L1.tag_width = ADDRESS_WIDTH - cache_L1.offset_width - cache_L1.index_width; 

	for (i = 0; i < NUM_OF_LINES_L1; i++) {
		cache_L1.lines[i].valid = 0;
		cache_L1.lines[i].dirty = 0;
		cache_L1.lines[i].tag = 0;
		memset(cache_L1.lines[i].data, 0, BLOCK_SIZE);
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

/// @brief simulates an access to the L1 cache with write-back policy.
/// @param address address to read/write data from
/// @param data data read/written at address
/// @param mode memory access mode (read or write)
void access_cache_L1(uint32_t address, uint8_t *data, int mode) {
	uint32_t tag, index, offset, mask;

	/*
	offset = address << (cache_L1.tag_width + cache_L1.index_width);
	offset = offset >> (cache_L1.tag_width + cache_L1.index_width);
	index = address << cache_L1.tag_width;
	index = index >> (cache_L1.tag_width + cache_L1.offset_width);
	tag = address >> (cache_L1.offset_width + cache_L1.index_width);	
	 
	*/

	offset = address % BLOCK_SIZE;

	index = (address / BLOCK_SIZE) % NUM_OF_LINES_L1;

	tag = address / (BLOCK_SIZE * NUM_OF_LINES_L1);



	// the current offset indicates the selected byte in the block out of 64 bytes (0-63)
	// since we want to read/write one WORD (4 bytes)
	// we need to make sure that the offset indicates the first byte of that word
	// e.g. offset = 51 = 0b 11 0011 => byte in index 51
	// word offset = 12 = 0b 1100 , byte offset within the word = 3 = 0b 0011
	// offset = 51 must be turned into 48 to be the first byte of the word
	// | word 0  | ... |      word 11     |      word 12     |     word 13      | ... |      word 15     |
	// | 0 1 2 3 | ... | (44)(45)(46)(47) | (48)(49)(50)(51) | (52)(53)(54)(55) | ... | (60)(61)(62)(63) |
	// if (offset % 4 != 0) { while (offset % 4 != 0) { offset--; } }

	printf("looking for address:%u, tag:%u, index:%u, offset:%u\n", address, tag, index, offset);

	// HIT
	// read/write one word from the block of data
	if (cache_L1.lines[index].valid && cache_L1.lines[index].tag == tag) {
		if (mode == READ_MODE) {
			memcpy(data, &(cache_L1.lines[index].data[offset]), WORD_SIZE);
			time += L1_READ_TIME;
			printf("\033[32mL1 hit\033[0m, mode: READ, address: %u, data: %u, time: %u\n\n", address, *data, time);
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
			printf("\033[32mL1 hit\033[0m, mode: WRITE, address: %u, data: %u, time: %u\n\n", address, *data, time);
		}
	}

	// MISS
	// we apply the principle of locality by copying the whole block that contains the memory address
	// the data block is necessarily copied from main memory to L1
	// note that in write mode it doesn't write in the RAM and only in L1
	// only when that block is getting replaced do we write to RAM
	else {
		uint8_t temp[BLOCK_SIZE];

		// there was a miss in this line
		// => this line was valid but with a diff tag OR this line was not valid

		// if the line was valid but with a diff tag
		// we are going to replace the block in this line from the old tag with the block from the new tag
		// we need to check if it is dirty and if so, we update in RAM before replacing it!
		if (cache_L1.lines[index].dirty) {

			uint32_t n, ind = index << 6;
			n = cache_L1.lines[index].tag + cache_L1.lines[index].off + ind;
			printf("dirty. curr: %d\told:%d\n", address, n);
			access_main_memory(n, cache_L1.lines[index].data, WRITE_MODE);
		}

		// start replacing the current line
		cache_L1.lines[index].valid = 1;
		cache_L1.lines[index].dirty = 0;
		cache_L1.lines[index].tag = tag;
		cache_L1.lines[index].off = offset;



		access_main_memory(address, temp, READ_MODE);				// get the block the contains the address
		memcpy(&(cache_L1.lines[index].data[0]), temp, BLOCK_SIZE);	// and put it the current line

		// now we read/write the desired word from the block
		if (mode == READ_MODE) {
			memcpy(data, &(cache_L1.lines[index].data[offset]), WORD_SIZE);
			time += L1_READ_TIME;
			printf("\033[31mL1 miss\033[0m, mode: READ, address: %u, data: %u, time: %u\n\n", address, *data, time);
		}
		else {
			memcpy(&(cache_L1.lines[index].data[offset]), data, WORD_SIZE);
			cache_L1.lines[index].dirty = 1;	// set this line as dirty to indicate that something new was written to it and not to RAM
			time += L1_WRITE_TIME;
			printf("\033[31mL1 miss\033[0m, mode: WRITE, address: %u, data: %u, time: %u\n\n", address, *data, time);
		}
	}
}

int main() {

	int i, k;
	uint8_t first_block[BLOCK_SIZE], data;
	uint32_t mem_addr;



	printf("\033[33m\nblock of data placed in RAM\n\n\033[0m");

	for (i = 0; i < BLOCK_SIZE; i++) {

		data = rand() % 256;
		first_block[i] = data;

		if (i != 0 && (i % 4) == 0) { printf("  <-  word %d\n", (i/4)-1); }
		printf("byte %d = %u\t", i, data);
	}
	printf("  <-  word %d\n\n\n", (i/4)-1);
	memcpy(&main_memory, &first_block, BLOCK_SIZE);

	for (i = 16384; i < BLOCK_SIZE; i++) {

		data = rand() % 256;
		first_block[i] = data;
	}


	printf("\033[33m\nblock of data in line 0 in L1 cache\n\033[0m");
	for (i = 0; i < BLOCK_SIZE; i++) {
		if (i != 0 && (i % 4) == 0) { printf("  <-  word %d\n", (i/4)-1); }
		printf("byte %d = %u\t", i, cache_L1.lines[0].data[i]);
	}
	printf("  <-  word %d\n\n\n\n", (i/4)-1);



	mem_addr = 2;
	access_cache_L1(mem_addr, &data, READ_MODE);

	reset_time();
	access_cache_L1(mem_addr, &data, READ_MODE);


	printf("\033[33m\nblock of data in line 0 in L1 cache after copying block from RAM\n\033[0m");
	for (i = 0; i < BLOCK_SIZE; i++) {
		if (i != 0 && (i % 4) == 0) { printf("  <-  word %d\n", (i/4)-1); }
		printf("byte %d = %u\t", i, cache_L1.lines[0].data[i]);
	}
	printf("  <-  word %d\n\n\n", (i/4)-1);


	mem_addr = 7;
	reset_time();
	access_cache_L1(mem_addr, &data, READ_MODE);

	mem_addr = 2;
	data = 222;
	reset_time();
	access_cache_L1(mem_addr, &data, WRITE_MODE);
	printf("\t\twrite-back checker: data DRAM at %u = %u\n\n\n\n", 2, main_memory[2]);


	mem_addr = 16389;
	data = 111;
	reset_time();
	access_cache_L1(mem_addr, &data, WRITE_MODE);
	printf("\t\twrite-back checker: data DRAM at %u = %u\n\n\n\n", 2, main_memory[2]);



	/* 
	data = 44;
	reset_time();
	access_cache_L1(mem_addr, &data, WRITE_MODE);
	printf("\t\twrite-back checker: data DRAM at %u = %u\n\n\n\n", mem_addr, main_memory[mem_addr]);


	mem_addr = 7;
	reset_time();
	access_cache_L1(mem_addr, &data, WRITE_MODE);

	*/


	/*
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
	*/
	return 0;
}