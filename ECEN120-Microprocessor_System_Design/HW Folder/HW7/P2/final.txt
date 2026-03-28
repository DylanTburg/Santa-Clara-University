;******************** (C) Andrew Wolfe *******************************************
; @file    HW7 Problem 2
; @author  Andrew Wolfe
; @date    2023
;*******************************************************************************

	
			AREA    main, CODE, READONLY
			EXPORT	__main				
			ENTRY			
				
__main	PROC
			ldr r4, =5 ;demonstrate values are conserved
			ldr r5, =5
			ldr r6, =5
			ldr r7, =5
			ldr r8, =5
			ldr r9, =5
			ldr r10, =5
			ldr r11, =5
			ldr		r0, =strarray	; Pointer to array of string pointers
			ldr		r1, =endofarray
			push{r11,r10,r9,r8,r7,r6,r5,r4}
			bl		mysort			; Call sorting routine
			pop{r4,r5,r6,r7,r8,r9,r10,r11}
endless		b		endless

		ENDP

			
	

									
mysort	PROC
			ldr r4, =0
			push{lr}
loop0		bl	compare_and_swap
			ldr r0, =strarray
			add r4, #4			;increments r0 up to the next value after a full loop
			add r0, r4
			cmp r1, r0
			bne	loop0
			pop{lr}
end			bx		lr
			ENDP

compare_and_swap	PROC
			mov r8, #1     ;counter
			mov r9, #0		;r9 and r6 are used for constants in the switching process
			mov r7, #0
			mov r10, #0
			ldr r5, =0xffffff00
			add r0, #4
			cmp r0, r1
			beq back     ; check in case we've reached the end
			sub r0, #4
			mov r6, #-4    ;used for offset
loop1		b compare
continue	add r0, #4
			add r8, #1
			mul r9, r8, r6
			add r9, #4
			ldr r5, =0xffffff00
			mov r10, #0
			mov r7, #0
back		cmp r1, r0
			bne	loop1
			bx lr
swap		mul r9, r8, r6
			add r9, #4
			ldr r2, [r0, r9]
			ldr r3, [r0]
			mov r7, r2		;swap with offset of r8 times r6(-4)
			mov r2, r3
			mov r3, r7
			mov r7, #0
			str r2, [r0, r9]
			str r3, [r0]
			b back
compare		cmp r8, #1
			beq continue
			ldr r2, [r0, r9]
			ldr r3, [r0]
			add r2, r10
			add r3, r10
			ldr r2, [r2]
			ldr r3, [r3]
			bic r2, r5
			bic r3, r5
			b ridcapitals
return2		cmp r3, r2
			blt swap
			bne continue
			cmp r3, #0 			;case for when we are at the end and r3=r2 (they are the same string)
			beq continue
			ror r5, #24
			add r7, #1
			cmp r7, #4
			beq nextword
			b compare
nextword	add r10, #4 ;if first four letters are the same, move to next 32-bit string (thats what I mean by word). It's not the actual next word in the strarray
			mov r7, #0
			b compare
ridcapitals push {r10} 
			mov r10, #0x5b5b5b5b ;check if r2 is capital
			bic r10, r5
			cmp r2, r10
			blt action1
return0		mov r10, #0x5b5b5b5b ;check if r3 is capital
			bic r10, r5
			cmp r3, r10
			blt action2
return1		pop {r10}
			b return2
action1		mov r10, #0x20202020 ;if r2 is capital convert to lower case
			bic r10, r5
			add r2, r10
			b return0
action2		mov r10, #0x20202020 ;if r3 is capital convert to lower case
			bic r10, r5
			add r3, r10
			b return1
			ENDP
						
			ALIGN
			AREA mydata, DATA, READONLY
			
			AREA mydata, DATA, READONLY
str1 DCB "First string",0
str2 DCB "Second string",0
str3 DCB "So, do I really need a third string",0
str4 DCB "Tetraphobia is the fear of the number 4",0
str5 DCB "A is for apple",0
str6 DCB "Z is called \'zed\' in Canada",0
str7 DCB "M is for middle",0
strarray	DCD	str1, str2, str3, str4, str5, str6, str7
endofarray
		END