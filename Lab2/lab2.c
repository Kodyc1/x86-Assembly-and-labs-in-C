// Kody Cheung 85737824
// ICS 53 Lab 2 Winter Q2017

// gcc -o MySearch lab2.c -lm

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>




void recurse(int search, int* arr, int count, int start, int end){

	int pipefd[2];
	
	pipe(pipefd);

	pid_t pid;
	pid_t pid2;

	if (count == 1){
		//printf("%d is at index %d\n", arr[start], start);
		if (search == arr[start]){

			printf("%d ", start);
			/*
			close(pipefd[0]);
			write(pipefd[1],(void*)& start, sizeof(int));
			close(pipefd[1]);
			*/
			exit(100);	
					
		}

	}

	else {

		int bottom, top;
		if ((count % 2) == 0){
			bottom = count/2;
			top = count/2;
		}
		else{
			bottom = count/2;
			top = bottom + 1;
		}
		//printf("bottom: %d\n", bottom);
		//printf("top: %d\n", top);

		// child1
		pid = fork();
		if (pid == 0){
				//printf("first half: %d %d %d\n", bottom, start, start + bottom);
				recurse(search, arr, bottom, start, start + bottom);
				exit(200);
		}

		// child2
		else {
			pid2 = fork();
			if (pid2 == 0){
				//printf("second half: %d %d %d\n", top, start + bottom, start + bottom + top);
				recurse(search, arr, top, start + bottom, start + bottom + top);
				exit(200);
			}
		}
			
		waitpid(pid, 0, 0);
             	waitpid(pid2, 0, 0);

	}	

	
	
}


int main(int argc, char** argv){

	pid_t pid;

	// $ MySearch arr_file.txt 6
	if (argc != 3){
		printf("Error: need 3 arguments\n");
		exit(1);
	}

	FILE* file;
	int array[10];

	char* filename = argv[1];
	
	int find_int = atoi(argv[2]);

	// printf("%s\n%d\n",filename, find);

	// error checking
	int count = 0;
	int i = 0;
	char c;

	// make sure file exists
	if(file = fopen(filename, "r")){
		
		// make sure number of integers is below 10
		while (fscanf(file, "%d", &array) > 0){
			count++;	
		}
		// make sure only integers are in file
		while (fscanf(file, "%c", &c) > 0){
			//printf("%c",c);	
			printf("Error: integers and whitespace only\n");
			exit(1);
		}
		fclose(file);	
	}
	else{
		printf("Error: file not found\n");
		exit(1);
	}

	if (count == 0){
		printf("Error: empty file\n");
		return -1;
	}

	if (count > 10){
		printf("Error: Max 10 numbers\n");
		return -1;
	}
	// printf("%d\n",count);

	// start finding number in file
	file = fopen(filename,"r");

	for(i = 0; i < count; i++){
		fscanf(file, "%d", &array[i]);
		//	printf("%d\n", array[i]);
	}	

	fclose(file);

	int val = 0;
       	for (i = 0; i < count; i++){
       		if ( find_int == array[i] )
			val++;
       	}					

	int pipefd[2];
 
	// pass integer to find and array with numbers
	recurse(find_int, array, count, 0, count);
			
	if (val == 0)
		printf("%d",-1);

/*	
	int value;
	pipe(pipefd);

	close(pipefd[1]);
	read(pipefd[0], (void*)&value, sizeof(int));
	close(pipefd[0]);
	
	printf("value = %d", value);
*/
	printf("\n");

	exit(1);

	return 0;
}
