
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(){

	// line reader
	char* line = NULL;
	size_t length = 0;
	ssize_t read;

	// command array
	char* command[10];

	
	char* heap = (char*) malloc(128 * sizeof(char));

	
	// HEAP IS 128 BYTES LONG including header
 	uint8_t begin = 0;
	char*p = heap;
	int i = 0;
	for (i = 0; i < 128; i++){
		p = 0;
		p = p + 1;
	}
	*heap = begin;	
	*(heap + 1) = 254;
	

	// Assume no more than 256 blocks will be allocated, a block is never larger than 128 bytes, and payload size is always multiple of 2
	// 	Most significant byte is the block number, 
	// 	The least significant byte is block size and 
	// 	Least significant bit is allocation status

	int block_number = 0;

	while(1){
		
		printf("> ");
		fflush(stdout);
	
		read = getline(&line, &length, stdin);

		if (read < 0)
			break;

		if (line[read-1] == '\n')
			line[read-1] = '\0';

		char* token = strtok(line, " ");

		if (token == NULL){}
		else{
			int i = 0;
			while(token != NULL){
				command[i] = token;
				token = strtok(NULL, " ");
				i++;
			}
			command[i] = NULL;



			// Command 7: quit
			// Quits program
			if (strcmp(command[0], "quit") == 0 ){
				exit(0);
			}


			// Command 1: allocate #_of_bytes
			// Allows user to allocate block of memory from heap
			// Print out a unique block number associated with the memory that was just allocated
			else if (strcmp(command[0], "allocate") == 0 & command[1] != NULL){ 

				
				int size = atoi(command[1]);
				
				int prev = block_number;

				block_number++;


				if (size > 0){
					// HEADER = |8 bit block number|7 bit block size|1 bit allocated| 
					uint8_t header_one = block_number;
	
					uint8_t block_size = atoi(command[1]);
					uint8_t allocate = 1;
					uint8_t header_two = block_size << 1; header_two = header_two | allocate;

					printf("|%02hhx|%02hhx|%2hhx\n", header_one, header_two >> 1, header_two & 1); 

					// check if there's free space
					
					char* pointer = heap;

					int byte = 0;
		
					while((*(pointer+1) & 1) || ((*(pointer + 1)&0xff) >> 1)+2 < block_size){
						int o = (*(pointer + 1)&0xff) >> 1;
						pointer = pointer + o + 2;
						printf("New free block at %hhd\n", o+2);
						byte += o + 2;
					}
					
					printf("pointer is at %hhd byte right now\n", byte);

					printf("free space found at byte %hhd with size %02hhx\n", byte, (*(pointer+1)&0xff) >> 1);					

					int existing = (*(pointer+1)&0xff) >> 1;

					if(existing == size){
						*(pointer) = header_one;
						*(pointer + 1) = header_two;
						continue;
					}

					int old_size = (*(pointer+1)&0xff)>>1;
					if (old_size == 127){
						old_size ++;
					}
					printf("old size: %d\n", old_size);

					uint8_t new_size = old_size - size - 2;
					printf("new size: %hhd\n", new_size);

					*(pointer) = header_one;
					*(pointer + 1) = header_two;

					if (new_size > 1){
						*(pointer + size + 2) = 0;
						*(pointer + size + 3) = ((new_size << 1)&0xff) & -2;
					}
					if(new_size == 1){
						*(pointer + size + 2) = 0;
					}

					printf("|%02hhx|%02hhx|%2hhx\n", *(pointer + size + 2), *(pointer+size +3), *(pointer+size+3) & 1);
			


					// split the free block


					printf("%d\n",block_number);
				}
			}


			// Command 2: free block_#
			// Allows user to free a block of memory, the block assigned in allocate
			else if (strcmp(command[0], "free") == 0 && command[1] != NULL){

				int find_block = atoi(command[1]);
				if (find_block > 0){
	
					char* pointer = heap;

					while(*pointer != find_block){
					
						pointer += (*(pointer+1)&0xff) >> 1;

					} 

					*(pointer + 1) = (*(pointer+1))&-2;
				}

			}
			

			// Command 3: blocklist
			// prints out info about all blocks in heap, only 1 argument
			else if (strcmp(command[0], "blocklist") == 0){
				// Size      Allocated(yes or no)   Start address     End address
				// 5 spaces  10 spaces              15 spaces         15 spaces
				printf("Size  Allocated  Start            End\n");
				
				char* pointer = heap;
				char* end = heap+128;
				//printf("%d\n", (*(pointer+1)&0xff) & 1);
				
				while(pointer != end){
					if ((*(pointer+1)&0xff) & 1){
						printf("%-3hhd   yes        0x%08x       0x%08x\n",(*(pointer+1)&0xff) >> 1, pointer, pointer + ((*(pointer+1)&0xff) >> 1) + 1);
						pointer = pointer + ((*(pointer+1)&0xff) >> 1) + 2;
					}
					else{
						printf("%-3hhd   no         0x%08x       0x%08x\n",(*(pointer+1)&0xff) >> 1, pointer, pointer + ((*(pointer+1)&0xff) >> 1) + 1);
						pointer = pointer + ((*(pointer+1)&0xff) >> 1);
					}
				}
			}
		

			// Command 4: writeheap  block_number  char  #_of_char
			// Writes characters into a block on the heap.
			else if (strcmp(command[0], "writeheap") == 0 && command[1] != NULL && command[2] != NULL && command[3] != NULL) {
				
				int block_number = atoi(command[1]);
				char* write = command[2];
				int num_of_char = atoi(command[3]);

				if (block_number < 0 || num_of_char < 0){
					printf("Error\n");
					continue;
				}

				char* pointer = heap;
				char* end = heap+128;
				//printf("%d\n", (*(pointer+1)&0xff) & 1);
				
				while(pointer != end){
					if ((*(pointer+1)&0xff) & 1){
						if (*pointer == block_number){
							if (num_of_char > ((*(pointer+1)&0xff) >> 1)){
								printf("Write too big");
								break;
							}
							else{
								int i = 0;
								for(i = 0; i < num_of_char; i++){
									*(pointer + 2 + i) = *write;
								}
							}
							break;
						}
						pointer = pointer + ((*(pointer+1)&0xff) >> 1) + 2;
					}
					else{
						pointer = pointer + ((*(pointer+1)&0xff) >> 1);
					}
				}


			
			}


			// Command 5: printheap block_number #_of_bytes to print
			// Prints the contents of a portion of the heap
			else if (strcmp(command[0], "printheap") == 0 && command[1] != NULL && command[2] != NULL){

				int block_number = atoi(command[1]);
				int bytes = atoi(command[2]);

				if (block_number < 0 || bytes < 0){
					printf("Error\n");
					continue;
				}

				char* pointer = heap;
				char* end = heap+128;
				//printf("%d\n", (*(pointer+1)&0xff) & 1);
				
				while(pointer != end){
					if ((*(pointer+1)&0xff) & 1){
						if (*pointer == block_number){
								int i = 0;
								for (i = 0; i < bytes; i++){
									printf("%c", *(pointer+2+i));
								}
								printf("\n");
								break;
						}
						pointer = pointer + ((*(pointer+1)&0xff) >> 1) + 2;
					}
					else{
						pointer = pointer + ((*(pointer+1)&0xff) >> 1);
					}
				}


			}


			// Command 6: printheader block_number
			// Prints the header of a block specified. 
			// Header is 2 bytes long and in hexadecimal
			else if (strcmp(command[0], "printheader") == 0 && command[1] != NULL){
					
				int block_number = atoi(command[1]);

				char* pointer = heap;
				char* end = heap+128;
				//printf("%d\n", (*(pointer+1)&0xff) & 1);
				
				while(pointer != end){
					if ((*(pointer+1)&0xff) & 1){
						if (*pointer == block_number){
							printf("%02hhx%02hhx\n",*pointer,*(pointer+1));
							break;
						}
						pointer = pointer + ((*(pointer+1)&0xff) >> 1) + 2;
					}
					else{
						pointer = pointer + ((*(pointer+1)&0xff) >> 1);
					}
				}
			}

			else{
				printf("Error\n");
			}

		}
	
	}

	free(heap);

	return 0;

}
