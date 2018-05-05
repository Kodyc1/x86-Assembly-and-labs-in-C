

#include <stdio.h>
#include <stdlib.h>



void modifier(int* array){

	array[1] = 2;

}


int main(){

	printf("divide 5 by 2: %d\n", 5/2);
	int array[3] = {0,0,0};

	modifier(array);

	int i = 0;
	for(i; i< 3; i++){
		printf("%d\n",array[i]);
	}

}
