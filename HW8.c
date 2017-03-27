#include <stdio.h>
#include <stdlib.h>

struct cache_entry
{
	unsigned char valid_bit;
	unsigned int tag;
	unsigned int value;
};

int main()
{
	struct cache_entry* cache = malloc(sizeof(struct cache_entry) * 16); // malloc 16 * sizeof()

	//loop to set each cache entry value to 0
	for(int i = 0; i < 16; i++){
		cache[i].value = 0;
		cache[i].valid_bit = 0;
	}

	int cont_check = 1;

	unsigned int offset_mask = 0x00000003;
	unsigned int set_mask = 0x3c;  // still have to shift after

	unsigned int addr_input;	// input address declaration
	unsigned int addr_set;		// input set declaration
	unsigned int addr_tag;		// input tag declaration
	unsigned int addr_offset;	// input offset declaration

	unsigned int input_address;		// address input declaration
	unsigned int input_value;		// address value declaration
	unsigned int input_set;			// declare cache set var

	while(cont_check != 0){

		char input;

		printf("Enter 'r' for read, 'w' for write, 'p' to print, 'q' to quit: ");
		scanf(" %c", &input);

		switch(input){
			case 'r':
				printf("Enter an address: ");
				scanf("%x", &addr_input);		//get input address (**** is it & or *?)
				
				addr_set = (addr_input & set_mask) >> 2;	// calculate address set
				addr_tag = addr_input >> 6;			// calculate address tag
				addr_offset = addr_input & offset_mask;		// calculate address offset
				//for(int i = 0; i < 16; i++){
				if(cache[addr_set].valid_bit == 1){			//check if valid bit is set
					if(cache[addr_set].tag == addr_tag){	//check if tags match
						//return value
						printf("Found set: %d - tag: %d - offset: %d - valid: %d - value: %02x\n", addr_set, addr_tag, addr_offset, cache[addr_set].valid_bit, (cache[addr_set].value >> (8 * addr_offset)) & 0xFF);
					}
					else{
						printf("Miss! Tags don't match!\n");
					}
				}
				else{
					printf("Miss! Invalid block!\n");
				}
				//}

				break;
			case 'w':
				//get address and input
				printf("Enter 32-bit unsigned hex address: ");
				scanf("%x", &input_address);					// set address var
				printf("Enter 32-bit unsigned hex value: ");	
				scanf("%x", &input_value);						// set value var

				//mask the address to get the 4 set bits
				input_set = (input_address & set_mask) >> 2;	// set cache set var
				if(cache[input_set].valid_bit == 0){				// if entered cache set is not valid..
					cache[input_set].tag = input_address >> 6;		// set cache entry tag to entered tag
					cache[input_set].valid_bit = 1;					// set cache set to valid	
					cache[input_set].value = input_value;			// write cache block
				}
				else{												// if valid bit already set.. print current val
					printf("Block occupied. Evicting block at set: %i, - tag: %i, - valid: %i, - value: %x\n", input_set, cache[input_set].tag, cache[input_set].valid_bit, cache[input_set].value);
					cache[input_set].tag = input_address >> 6;		// set entry to new tag
					cache[input_set].value = input_value;			// write cache block
				}


				//fill in
				break;
			case 'p':
				for(int i = 0; i < 16; i++){
					if(cache[i].valid_bit == 1){
						printf("set: %d - tag: %d - valid: %d - value: %02x %02x %02x %02x\n", i, cache[i].tag, cache[i].valid_bit, cache[i].value & 0xFF, (cache[i].value >> 8) & 0xFF, (cache[i].value >> 16) & 0xFF, (cache[i].value >> 24) & 0xFF);
					}
				}
				//fill in
				break;
			case 'q':
				cont_check = 0;
				break;
		}
	}

	return 0;
}
