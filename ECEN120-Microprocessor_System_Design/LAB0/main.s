;******************** (C) Andrew Wolfe *******************************************
; @file    mainproto.s
; @author  Andrew Wolfe
; @date    August 18, 2019
; @note
;           This code is for the book "Embedded Systems with ARM Cortex-M 
;           Microcontrollers in Assembly Language and C, Yifeng Zhu, 
;           ISBN-13: 978-0982692639, ISBN-10: 0982692633 as used at Santa Clara University
;*******************************************************************************

	
			AREA    main, CODE, READONLY
			EXPORT	__main				
			ENTRY			
				
__main	PROC
	
;********************************************************************************

;LDR r0, =0x13
;LDR r1, =0x07
;ADD r2, r0, r1
;EOR r3, r0, r1
;endless B endless

; question 1 code ^^


;EOR r0, r0, r0
;MOV r2, r0
;ADD r1, r0, #1
;MOV r3, r1
;count ADD r2, r2, r1
;ADD r3, r3, r3
;B count
;endless B endless

;question 2 code ^^^^
;
		EOR r0, r0, r0
		MOV r2, r0
		ADD r1, r0, #1
		MOV r3, r1
count 	ADD r2, r2, r1
		ROR r3, #31
		B count
endless	B		endless
;********************************************************************************


		ENDP
					
		ALIGN			

;********************************************************************************
; Put Your Data Here
;********************************************************************************
	END
