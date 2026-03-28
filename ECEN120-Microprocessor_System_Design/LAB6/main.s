;******************** (C) Andrew Wolfe *******************************************
; @file    main_hw_proto.s
; @author  Andrew Wolfe
; @date    August 18, 2019
; @note
;           This code is for the book "Embedded Systems with ARM Cortex-M 
;           Microcontrollers in Assembly Language and C, Yifeng Zhu, 
;           ISBN-13: 978-0982692639, ISBN-10: 0982692633 as used at Santa Clara University
;*******************************************************************************



	INCLUDE core_cm4_constants.s		; Load Constant Definitions
	INCLUDE stm32l476xx_constants.s   
	INCLUDE jstick.h
	INCLUDE leds.h
	INCLUDE rgb60_redact.h
	
	

	
			AREA    main, CODE, READONLY
			EXPORT	__main				
			ENTRY			
				
__main	PROC
			bl spisw_init
loop		ldr r0, =0x00000000
			bl spi32
			ldr r9, =60
cond 		ldr r0, =0xe40000ff
			bl spi32
			bl delay
			sub r9, #1
			cmp r9, #0
			bne cond
			ldr r0, =0x00000000
			bl spi32
			bl clear
			b loop		

		ENDP
			
clear PROC
			ldr r9, =60
			ldr r0, =0x00000000
			push {lr}
			bl spi32
cond2		ldr r0, =0xe00f0000
			bl spi32
			bl delay
			subs r9, #1
			bne cond2
			ldr r0, =0x00000000
			bl spi32
			pop {lr}
			bx lr
	ENDP
		
delay PROC
		ldr r0, =0x4000
loop1	sub r0, #1
		cmp r0, #0
		bne loop1
		bx lr
		ENDP
	END