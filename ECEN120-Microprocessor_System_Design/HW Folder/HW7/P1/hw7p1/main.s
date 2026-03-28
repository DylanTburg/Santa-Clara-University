;******************** (C) Andrew Wolfe *******************************************
; @file    HW7 Problem 1
; @author  Andrew Wolfe
; @date    2023
;*******************************************************************************

	
			AREA    main, CODE, READONLY
			EXPORT	__main				
			ENTRY			
				
__main	PROC
			ldr r4, =5;test to make sure subroutine preserved register values are indeed preserved
			ldr r5, =5
			ldr r6, =5
			ldr r7, =5
			ldr r8, =5
			ldr r9, =5
			ldr r10, =5
			ldr r11, =5
			ldr		r0, =array	; Pointer to array of string pointers
			ldr		r1, =endofarray			; end of array
			push {r11,r10,r9,r8,r7,r6,r5,r4}
			bl		mysort			; Call sorting routine
			pop {r4,r5,r6,r7,r8,r9,r10,r11}
endless		b		endless

		ENDP

			
					; registers 4-11 must be preserved while in subroutines which is why they are pushed and popped

									
mysort	PROC
			ldr r4, =0
			push{lr}
loop0		bl	compare_and_swap
			ldr r0, =array
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
			mov r6, #0
			ldr r2, [r0]
			add r0, #4
			cmp r0, r1
			beq back     ; check in case we've reached the end
loop1		ldr r3, [r0]
			mov r6, #-4    ;used for offset
			cmp r3, r2
			blt swap
			add r0, #4
			add r8, #1
back		cmp r1, r0
			bne	loop1
			bx lr
swap		mov r7, r2		;swap with offset of r8 times r6(-4)
			mov r2, r3
			mov r3, r7
			mul r9, r8, r6
			str r2, [r0, r9]
			str r3, [r0]
			b back
			ENDP
						
			ALIGN
			AREA mydata, DATA, READONLY

				
array		DCD		9,2,5,1,8,6,7,0,3,4
endofarray
	

		END