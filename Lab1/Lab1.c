
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){

	// line reader
	char* line = NULL;
	size_t length = 0;
	ssize_t read;

	// tokenized command
	char* command[10];


	// Text file of dungeon
	FILE* filename;
	char* lines[100];		


	// loaded file or not
	int activate = 0;

	// current spot
	char* current;
	int index;

	while(1){
		// prompt user to type
		printf("$ ");		
		fflush(stdout);


		// get first command
		read = getline(&line, &length, stdin);


		// break if EOF
		if (read < 0)
			break;

		// remove newline from input
		if (line[read-1] == '\n')
			line[read-1] = '\0';

		// tokenize the line
		char* token = strtok(line," ");

		if (token == NULL){}
		else{
			int i = 0;
			while (token != NULL){
				command[i] = token;
				token = strtok(NULL, " ");
				i++;
			}
			command[i] = NULL;

			////printf("First: %s\n",command[0]);
			////printf("Second: %s\n",command[1]);
			

			// quit command
			if (strcmp(command[0],"quit") == 0){
				exit(0);
			}


			// load dungeon
			if (activate == 0 && strcmp(command[0],"loaddungeon")==0 && (filename = fopen(command[1],"r")) != NULL){
				
				activate = 1;		

				// count the number of lines in the file
				int number_of_lines = 0;
				int character = 0;

				while(!feof(filename)){
					character = fgetc(filename);
					if(character == '\n')
						number_of_lines += 1;
				}
				fclose(filename);
				//printf("number of lines %d\n", number_of_lines);

				

				// create array of dungeon
				char* array[number_of_lines][3];
				filename = fopen(command[1],"r");
				char linez[100];	
				int outer = 0;

				while ( (fgets(linez, sizeof(linez), filename)) != NULL) {
					// get each line from file
					token = strtok(linez, "\n");
					//printf("\ntoken == %s\n", token);

					// split each line by $ delimiter
					int inner = 0;
					char* token2 = strtok(token, "$");
					//printf("token2 == %s\n\n", token2);

					while (token2 != NULL){
						if (token2){
							//printf("[%d][%d]\n", outer, inner);
							array[outer][inner] = strdup(token2);
							//printf("continuing token2: %s\n", token2);
							token2 = strtok(NULL, "$");
							inner++;
						}
					}	
					while(token){
						//printf("End token: %s\n",token);
						token = strtok(NULL, "\n");
						outer++;
					}
				}
				fclose(filename);
				
				// set current to first line
				current = array[0][0];
	
				// print first line of dungeon
				printf("%s\n", array[0][1]);

				// int a = 0;
				// int b = 0;
				// for (a = 0; a < 4; a++){
				// 	for (b=0; b<3; b++){
				// 		printf("array[%d][%d]: %s\n", a,b,array[a][b]);
				// 	}
				// }


				// remove any trailingi whitespaces
				int f = 0;
				for(f = 0; f < number_of_lines; f++){
					char* str = array[f][0];
					str[strlen(str)-1] = '\0';
				} 

				char* directions;
				char* cardinal[4];
				int x = 0, j = 0;
				char* copy;

				// loop through north east west south 
				while(1){

					printf("$ ");		
					fflush(stdout);


					// get first command
					read = getline(&line, &length, stdin);

					// remove newline from input
					if (line[read-1] == '\n')
						line[read-1] = '\0';

					// tokenize the line
					char* token = strtok(line," ");

					int i = 0;
					while (token != NULL){
						command[i] = token;
						token = strtok(NULL, " ");
						i++;
					}
					command[i] = NULL;
		

					// quit command
					if (strcmp(command[0],"quit") == 0){
						exit(0);
					}
				

					// make room number equal to index in array
					for (j = 0; j < number_of_lines; j++){
						if (strcmp(current, array[j][0]) == 0){
							index = j;
						}		
					}
					//printf("index: %d\n", index);


					// get string of possible moves
					copy = strdup(array[index][2]);
					directions = strtok(copy, " ");
					//printf("%s\n",directions);
														
					x = 0;
					while (directions != NULL){
						cardinal[x++] = strdup(directions);
						directions = strtok(NULL, " ");
					}	


					// once file is loaded, take these commands
					if (strcmp(command[0],"north") == 0){
						
						// can't go if -1						
						if ( strcmp(cardinal[0],"-1") == 0) { 
							printf("You can't go there\n");
						}

						// check new index number and go there after printing out message
						else {
							current = cardinal[0];
							for (j = 0; j < number_of_lines; j++){
								if (strcmp(current, array[j][0]) == 0){
									index = j;
								}		
							}
							printf("%s\n",array[index][1]);
						}
					
					}


					else if (strcmp(command[0],"south") == 0){
						
					 	if (strcmp(cardinal[1],"-1") == 0) {
		 					printf("You can't go there\n");
		 				}
						else{
							current = cardinal[1];
							for (j = 0; j < number_of_lines; j++){
								if (strcmp(current, array[j][0]) == 0){
									index = j;
								}		
							}
							printf("%s\n", array[index][1]);
						}
					}

					else if (strcmp(command[0],"east") == 0){

						if (strcmp(cardinal[2],"-1") == 0){
		 					printf("You can't go there\n");	
						}

						else{
							current = cardinal[2];
							for (j = 0; j < number_of_lines; j++){
								if (strcmp(current, array[j][0]) == 0){
									index = j;
								}		
							}
							printf("%s\n",array[index][1]);
						}

					}


					else if (strcmp(command[0],"west") == 0){
					
						if (strcmp(cardinal[3],"-1") == 0){	
		 					printf("You can't go there\n");
		 				}

						else{
							current = cardinal[3];
							for (j = 0; j < number_of_lines; j++){
								if (strcmp(current, array[j][0]) == 0){
									index = j;
								}		
							}
							printf("%s\n",array[index][1]);
						} 
					}

					// error handling
					else {
					 	printf("Commands: north, south, east, west, or quit\n");
					}
		
				}
				continue;
			}
			// error handling
			else if (activate == 0){ 
				printf("Enter \"loaddungeon + your file name\" please\n");
			}

		}
	}
	return 0;
}






