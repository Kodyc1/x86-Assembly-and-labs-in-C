/*
ICS 51. Lab #3

IMPORTATNT NOTES:

- Noramally, the compiler adds the prologue/epilogue code for the callee. However, when the
__declspec(naked) attribute is used, it is the programmer's responsibility to write the
prologue/epilogue code. In this lab you need to write prologue/epilogue code sections only
for the functions that have been declared with the naked attribute, namely:

+ findMinIndex
+ recFactorial
+ performOperation
+ fibonacci

- You cannot define any local variable in the C code. If you need additional variables in your
assembly code, you have to allocate space for them in the prologue of the function.

- You are not allowed to change anything in this file except for adding your assembly code
between the lines marked "BEGIN YOUR CODE HERE" and "END YOUR CODE HERE".

- Remember to fill in your name, student ID below.

- ONLY submit the lab3.c file for grading.

*/

char *yourName = "Kody Cheung";
char *yourStudentID = "85737824";

/***********************************************************************************************

PART 1: Selection Sort

Selection sort is an in-place comparison sort algorithm that works by dividing the input list
into two parts: the sublist of items already sorted, which is built up from left to right of
the list, and the sublist of items remaining to be sorted that occupy the rest of the list.
Initially, the sorted sublist is empty and the unsorted sublist is the entire input list.
The algorithm proceeds by finding the smallest element in the unsorted sublist, exchanging it
with the leftmost unsorted element (putting it in sorted order), and moving the sublist
boundaries one element to the right. To learn more, read:
https://en.wikipedia.org/wiki/Selection_sort#Example

Our implementation here uses a function called "findMinIndex" to find the index of smallest
element between ith index and jth index of the array. The function "selectionSort" uses
this function to find the smallest number and exchanges it with the leftmost unsorted element
in each iteration. You need to implement the behavior of both functions in x86 assembly.

************************************************************************************************/

__declspec(naked)
int findMinIndex(int integer_array[], int i, int j)
{

	// C code to be converted to x86 assembly
	/*
	int iMin = i;
	// test against elements after i and before j to find the smallest
	for ( i ; i < j; i++) {
	// if this element is less, then it is the new minimum
		if (integer_array[i] < integer_array[iMin]) {s
		// found new minimum; remember its index
			iMin = i;
		}
	}

	return iMin;
	*/

	__asm {
		// BEGIN YOUR CODE HERE

			push ebp
			mov ebp, esp

			push ebx
			push esi
			push edi

			mov ebx, [ebp + 8]	// integer array
			mov esi, [ebp + 12]	// int i
			mov edi, [ebp + 16]	// int j

			mov eax, [ebp + 12]	// int iMin

	FOR:
			cmp esi, edi
			jge ENDLOOP
			mov ecx, dword ptr[ebx + eax * 4]	// ecx = integer_array[iMin]

			//IF:
			cmp dword ptr[ebx + esi * 4], ecx // if (integer_array[i] < ecx
			jge ELSE
				mov eax, esi	// iMin = i
				inc esi
				jmp FOR

			ELSE:
				inc esi
				jmp FOR
	ENDLOOP:
				pop edi
				pop esi
				pop ebx

				mov esp, ebp
				pop ebp


			// END YOUR CODE HERE
			ret
	}

}

void selectionSort(int integer_array[], int array_size)
{

	// C code to be converted to x86 assembly
	/*
	int j;
	int iMin;
	int temp;

	// advance the position through the entire array //
	// (could do j < n-1 because single element is also min element) //
	for (j = 0; j < array_size-1; j++) {

		// find the index of min element in the unsorted a[j .. n-1] //
		iMin = findMinIndex (integer_array, j, array_size);

		if(iMin != j) { // swap values
		temp = integer_array[iMin];
		integer_array[iMin] = integer_array [j];
		integer_array[j] = temp;
		}
	}
	*/

	__asm {
		// BEGIN YOUR CODE HERE

		mov edi, 0		// int j counter

		FOR:
			mov esi, array_size	// array_size
			dec esi				// array_size -1

			cmp edi, esi		// compare j and array_size
			jge ENDLOOP
				push eax
				push ecx
				push edx

				mov ecx, array_size
				mov eax, edi
				mov edx, integer_array

				push ecx	// array_size = int i
				push eax	// j
				push edx	// integer_array

				call findMinIndex	// findMinIndex(edx, eax, ecx)

				add esp, 12

				mov ecx, eax	// ecx = iMin = result

			//IF
				cmp ecx, edi	// if (iMin != j)
				je ELSE
					mov ebx, dword ptr[edx + ecx * 4]	// temp  = integer_array[ecx]

					mov eax, dword ptr[edx + edi * 4]		// eax = integer_array[j]
					mov dword ptr[edx + ecx * 4], eax		// integer_array[ecx] = integer_array[j]

					mov dword ptr[edx + edi * 4], ebx					// integer_array[j] = temp
					jmp END
				ELSE:
					jmp END

		END:
			pop edx
			pop ecx
			pop eax

			inc edi
			jmp FOR
	ENDLOOP:
			

		// END YOUR CODE HERE
	}

}

/***********************************************************************************************

PART 2: Recursive Factorial

Implement two functions that calculates the factorial of an integer not greater than 12.

The 1st function, factorial (unsigned int input, unsigned int *output), is the non
recursive wrapper function which takes the input, sets up the stack, calls the recursive
function and finally puts the result in the location pointed to by the pointer argument called
output.

The 2nd function, recFactorial(unsigned int n), is the recursive function that actually
calculates the factorial.

Given an unsigned integer number n,we define FACTORIAL(n) as:

	FACTORIAL(n) =  1, if n = 0 or n = 1
	n * FACTORIAL(n-1), if n > 1

Tip:

- You need to use IMUL instruction to multiply unsigned integers. Learn how to use it:
http://www.cs.virginia.edu/~evans/cs216/guides/x86.html#instructions

************************************************************************************************/

__declspec(naked)
unsigned int recFactorial(unsigned int n)
{

	// C code to be converted to x86 assembly
	/*
	if (n<=1)
		return 1;
	else
		return n * recFactorial(n-1);
	*/

	__asm {
		// BEGIN YOUR CODE HERE

		push ebp
		mov ebp, esp
		
		push esi
		push edi
		push ebx

		// ebx = n
		mov ebx, [ebp + 8]

	//IF:
		cmp ebx, 1
		ja ELSE
			mov eax, 1
			jmp END

	ELSE:
		// esi = n-1
		mov esi, n
		dec esi
		push esi

		call recFactorial
		add esp, 4

		// n * (n-1)
		imul eax, ebx

		jmp END

	END:
		pop ebx
		pop edi
		pop esi

		mov esp, ebp
		pop ebp

			// END YOUR CODE HERE
			ret
	}
}

void factorial(unsigned int input, unsigned int *output)
{

	// C code to be converted to x86 assembly
	/*
	*output = recFactorial (input);
	*/

	__asm {
		// BEGIN YOUR CODE HERE

		push eax
		push ecx

		// recFactorial(input) parameter
		mov ecx, input
		push ecx	

		call recFactorial

		add esp, 4

		// put into contents of output, not address
		mov edx, output
		mov [edx], eax

		pop ecx
		pop eax

			// END YOUR CODE HERE
	}

}

/***********************************************************************************************

PART 3: Vector Operation

The vectorOperations function gets as parameters 4 arrays of the same length and
the number of elements in those arrays.

For each pair of elements in first_array and second_array, it calls performOperation
and stores the returned result in the same index of output_array.

output_array[index] = performOperation(operation_array[index], first_array[index], second_array[index]).

You also need to implement the body of performOperation function according to the provided
C code.

************************************************************************************************/

__declspec(naked)
int performOperation(int operation, int first, int second)
{

	// C code to be converted to x86 assembly
	/*
	if (operation==0)
	return first + second;
	else if (operation==1)
	return first - second;
	else if (operation==2)
	return (first > second)? first : second;
	else if (operation==3)
	return (first < second)? first : second;
	else
	return first;
	*/

	__asm {
		// BEGIN YOUR CODE HERE

			push ebp
			mov ebp, esp

			push ebx
			push edi
			push esi

			mov esi, [ebp + 8]		// operation
			mov ebx, [ebp + 12]		// first
			mov edi, [ebp + 16]		// second

									//IF:
			cmp esi, 0
			jne ELSEIF1
				mov eax, ebx		// move int first into eax
				add eax, edi		// add first + second
			jmp END

			ELSEIF1 :
			cmp esi, 1
			jne ELSEIF2
				mov eax, ebx
				sub eax, edi
			jmp END

			ELSEIF2 :
			cmp esi, 2
			jne ELSEIF3
				cmp ebx, edi
				jle GREATER
					mov eax, ebx
					jmp END
				GREATER :
					mov eax, edi
			jmp END

			ELSEIF3 :
			cmp esi, 3
			jne ELSE
				cmp ebx, edi
				jae LESS
					mov eax, ebx
					jmp END
				LESS :
					mov eax, edi
			jmp END

		ELSE :
			//return first
			mov eax, ebx
			jmp END

	END :
		pop esi
		pop edi
		pop ebx

		mov esp, ebp
		pop ebp
			// END YOUR CODE HERE
			ret
	}
}

void vectorOperations(int number_of_elements, int *first_array, int *second_array,
	int *operation_array, int *output_array)
{
	// C code to be converted to x86 assembly
	/*
	int i;
	for (i=0; i<number_of_elements; i++)
	{
	output_array[i] = performOperation(operation_array[i], first_array[i], second_array[i]);
	}
	*/

	__asm {
		// BEGIN YOUR CODE HERE

		mov edi, 0

	FOR:
		cmp edi, number_of_elements
		jge END
			push eax
			push ecx
			push edx
			// output_array[ecx] = performOperation(operation_array[ecx], first_array[ecx], second_array[ecx])
			mov eax, second_array
			mov ecx, first_array
			mov edx, operation_array

			push[eax + edi * 4]
			push[ecx + edi * 4]
			push[edx + edi * 4]

			call performOperation

			// get rid of size of params = 4 * 3 ints
			add esp, 12

			// mov return value into output array
			mov esi, output_array
			mov dword ptr[esi + edi * 4], eax

			pop edx
			pop ecx
			pop eax

			inc edi
			jmp FOR

	END :

		// END YOUR CODE HERE
	}
}

/***********************************************************************************************

PART 4: Change Letter Case

You are given an array of characters (a string) that may contain numbers, upper and	lower case
letters, punctuations symbols, etc. Implement a function to convert all uppercase letters to
lowercase and vice versa.
For example "ThisIsTheFinalLabOf51" should be converted to "tHISiStHEfINALlABoF51".
The function has the following argument: An array of charaters and the size of the array. The
array should be changed in place. There is no explicit output for this function.

Tips:

- Note that each element of the arrays is a char (1 byte). You can NOT use a 32-bit register
to read/write from/to a memory location. Instead you should use the 8-bit version of the x86
general-purpose registers.
More info: http://www.cs.virginia.edu/~evans/cs216/guides/x86.html#registers

- The link to the ascii codes is here: http://www.asciitable.com/

************************************************************************************************/

void changeCase(char char_array[], int array_size) {

	__asm {
		// BEGIN YOUR CODE HERE

		mov eax, char_array
		mov ecx, 0			// index for character array

	FOR:
		cmp ecx, array_size
		jge ENDLOOP

		//IF:
			cmp byte ptr[eax + ecx], 0x41			// 41 = A				
			jl ELSEIF
				cmp byte ptr[eax + ecx], 0x5A		// 5A = Z
				jg ELSEIF
					add byte ptr[eax + ecx], 32		// turn in lowercase
			jmp ELSE

		ELSEIF :
			cmp byte ptr[eax + ecx], 0x61			// 61 = a
			jl ELSE
				cmp byte ptr[eax + ecx], 0x7A		// 7A = z
				jg ELSE
					sub byte ptr[eax + ecx], 32		// turn it uppercase
			jmp ELSE

		ELSE :
			//INCREMENT COUNTER
			inc ecx
			jmp FOR
	ENDLOOP :

		// END YOUR CODE HERE
	}
}

/***********************************************************************************************

PART 5: Fibonacci

Implement a recursive function in assembly which returns the Nth number in the fibbonaci
series (1, 1, 2, 3, 5, 8, ...).

************************************************************************************************/

__declspec(naked)
unsigned int fibonacci(unsigned int n) {

	// C code to be converted to x86 assembly
	/*
	if (n < 2)
		return 1;
	else
		return fibonacci(n - 1) + fibonacci(n - 2);
	*/

	__asm {
		// BEGIN YOUR CODE HERE
		push ebp
		mov ebp, esp

		push edi
		push esi

		mov ebx, n

	//IF:
		cmp ebx, 2
		jae ELSE
			//return 1
			mov eax, 1

			pop esi
			pop edi

			jmp END

	ELSE :

		// fibonacci(n - 1) 
		mov esi, n		// n
		dec esi			// n-1
		push esi		// (n-1) parameter

		call fibonacci	// fibonacci(n-1)

		add esp, 4		// delete param from stack
		
		push eax		// push eax onto the stack


		// fibonacci(n - 2)
		mov edi, n		// n
		sub edi, 2		// n-2
		push edi		// (n-2) parameter

		call fibonacci	// fibonacci(n-2)

		add esp, 4		// delete param from stack

		

		mov ecx, eax	// move fibonnacci(n-2) into ecx
		pop eax			// get old eax

		add eax, ecx

			pop esi			// restore the register
			pop edi			// restore register

		jmp END

	END :

		mov esp, ebp
		pop ebp

		// END YOUR CODE HERE
		ret
	}
}