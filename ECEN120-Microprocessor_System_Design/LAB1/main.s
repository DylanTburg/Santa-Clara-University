;******************** (C) Andrew Wolfe *******************************************
; @file    mainproto.s
; @author  Andrew Wolfe
; @date    August 18, 2019
; @note
;           This code is for the book "Embedded Systems with ARM Cortex-M 
;           Microcontrollers in Assembly Language and C, Yifeng Zhu, 
;           ISBN-13: 978-0982692639, ISBN-10: 0982692633 as used at Santa Clara University
;*******************************************************************************

	
				AREA main, CODE, READONLY
				EXPORT __main
				ENTRY
__main 		PROC
			LDR r0, =15
			MVN r1, r1
			AND r2, r0, r1
			LDR r8, =result
			LDR r1, =num1
			LDR r2, [r1]
			LDR r3, =num2
			LDR r4, [r3]
			MUL r5, r2, r2
			MUL r6, r4, r4
			ADD r7, r5, r6
			STR r7, [r8]
			;AND r9, r7, #0x0ff00ff0
endless 	B 	endless
			ENDP
			ALIGN
num1 		DCD 0x05
num2 		DCD 0x03
result 		DCD 0x0
	END
