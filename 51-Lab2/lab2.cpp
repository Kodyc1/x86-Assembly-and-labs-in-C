// Remember to fill in your name and student ID below.
char *yourName = "Kody Cheung";
char *yourStudentID = "85737824";


// Part1) This function takes in a square sized grayscale image and applies thresholding on each pixel.
// Width and height of the image are equal to "dim".
void imageThresholding(unsigned char* image, int dim) {
	
	__asm {
		push ebx;
		push edi;
		push esi;

// YOUR CODE STARTS HERE
	/* 
	while loop (condition that iterates through image pixels by dimensions)
		if (pixel < 0x80){
			pixel = 0x00	 
		}
		else if (pixel >= 0x80){
			pixel = 0xFF	
		}
		jmp while
	*/	

		// element_size_in_bytes = 1
		// offset = row_idx * num_of_columns + col_idx
		// element_addr = base_addr + 1 * offset

		// element_addr = base_addr + (row_idx * num_of_columns + col_idx)

		mov ebx, dim	// dimension

		mov ecx, 0		// counter initialization for row

		mov esi, image		// image array

FOR:
		cmp ecx, dim		// check row counter
			jae END
			mov edi, 0		// counter initialization for column

		FOR2:
			cmp edi, dim	// check column counter
				jae END2

			// offset
			mov eax, ecx	// row index
			mul dim			// times number of columns
			add eax, edi	// add column index

			//comparison 
			
			cmp byte ptr [esi + eax], 0x80	// compare to see if less than 0x80 so BLACK
				jae WHITE
				mov byte ptr [esi + eax], 0x00
				inc edi 
				jmp FOR2
			WHITE:
				mov byte ptr [esi + eax], 0xFF
				inc edi		// increment column
				jmp FOR2
		
		END2:
			inc ecx			// increment row
			jmp FOR

END:


// YOUR CODE ENDS HERE
		
		pop esi;
		pop edi;
		pop ebx;
	}
}


// Part2) This function rotates a given square sized color image 90 degrees clockwise.
// Width and height of the image are equal to "dim".
// Four variables are defined to simplify your implementation.
// Note that you cannot define additional variables.
void imageRotation(unsigned int* image, int dim) {
	
	unsigned int i0, i90, i180, i270;

	__asm {
		push ebx;
		push edi;
		push esi;

// YOUR CODE STARTS HERE


	//mov ebx, image		//image

	//mov edi, 4

	//mov i0, edi			//PIXEL 1

	////add ebx, i0			//ADDRESS OF PIXEL 1

	//mov i90, 4			//PIXEL 2

	//add ebx, i0

	//add ebx, i90

	//mov ecx, [ebx]

	//mov ebx, image

	//mov[ebx], ecx


		// Iterate over the red triangle (row by row starting from top left)
		
		mov ebx, dim		// dimension
		dec ebx				// start at dim-1
		mov ecx, 0			// ROW COUNTER
		
	FOR :
		
		cmp ecx, ebx			// check row counter
			jae END
			mov edi, ecx		// COLUMN COUNTER
			
			FOR2 :
			
				cmp edi, ebx	// check column counter
				jae END2
					
				// offset in EAX FOR i0 = index + (row * dim) + column)
				mov eax, ecx	// row index
				mul dim			// times number of columns
				add eax, edi	// add column index
				shl eax, 2		// multiply offset by 4 for color pixels

				//mov esi, image	// image
				//mov dword ptr[esi + eax * 4], 0x00

				mov i0, eax			// contains only offset of i0

				// offset in EAX FOR i90 = index + (dim(1 + column - row) - row - 1 - column)) 
				mov eax, 1 
				add eax, edi
				sub eax, ecx
				mul dim
				sub eax, ecx
				sub eax, 1
				sub eax, edi

				shl eax, 2

				add eax, i0			// add to i0 offset
 
				mov i90, eax		// contains only offset of i90

				// offset in EAX for i180 = index + (dim(dim - row - row) - column - column - 1 )

				mov eax, dim
				sub eax, ecx
				sub eax, ecx
				mul dim 

				sub eax, edi
				sub eax, edi
				dec eax

				shl eax, 2

				add eax, i0

				mov i180, eax

				// offset in EAX for i270 = index + (dim 

				mov eax, dim
				sub eax, 1
				sub eax, ecx
				sub eax, edi
				mul dim

				sub eax, edi
				add eax, ecx

				shl eax, 2

				add eax, i0

				mov i270, eax


				// i0 -> i90 -> i180 -> i270 -> i0

				mov esi, image	// space of i0
				add esi, i0
				mov eax, [esi]  // SAVE I0 EAX = I0

				mov edx, image	// image space of i270
				add edx, i270	

				mov edx, [edx]  // content of i270

				mov [esi], edx  // put i270 into i0


				sub esi, i0
				add esi, i90	// esi is now space of i90

				mov edx, [esi]	// edx is now content of i90

				mov [esi], eax	// move i0 into i90 space

				sub esi, i90	
				add esi, i180	// esi is now space of i180
				mov eax, [esi]	// eax is now contents of i180
				
				mov [esi], edx	// mov i90 into i180
				
				sub esi, i180
				add esi, i270	// esi is now space i270

				mov [esi], eax	// mov i180 into i270



				//mov eax, [esi]	// put i90 into register
				//sub esi, i90
				//mov [esi], eax  // put i90 into i0


				inc edi			// increment column
				jmp FOR2

			END2 :
				dec ebx
				inc ecx			// increment row

				jmp FOR

	END :


			// compute index of pixel p0, find the corresponding memory address and store it in i0
			 
				// mov i0, 

		     // compute index of pixel p90, find the corresponding memory address and store it in i90
			
				// mov i90, //index

		     // compute index of pixel p180, find the corresponding memory address and store it in i180
			
				// mov i180, //index

		     // compute index of pixel p270, find the corresponding memory address and store it in i270
			
				// mov i270, //index

		     // rotate pixel values: p0 -> p90 -> p180 -> p270 -> p0


			
			 


// YOUR CODE ENDS HERE
	    
		pop esi;
	    pop edi;
	    pop ebx;
	}
	
}

