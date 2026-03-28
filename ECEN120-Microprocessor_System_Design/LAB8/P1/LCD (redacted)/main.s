;******************** (C) Andrew Wolfe *******************************************
; @file    lcd-r.s
; @author  Andrew Wolfe
; @date    Nov. 19, 2019
; @note
;           This code is for the book "Embedded Systems with ARM Cortex-M 
;           Microcontrollers in Assembly Language and C, Yifeng Zhu, 
;           ISBN-13: 978-0982692639, ISBN-10: 0982692633 as used at Santa Clara University
;*******************************************************************************



	INCLUDE core_cm4_constants.s		; Load Constant Definitions
	INCLUDE stm32l476xx_constants.s   

	INCLUDE lcd.h



	
			AREA    main, CODE, READONLY
			EXPORT	__main				
			ENTRY			
				
__main	PROC
	
		bl		lcd_init
endless	bl		lcd_clear

;******************************************
		
		mov r0, #83;S
		bl let2font
		mov r1, #1
		bl lcd_draw
		mov r0, #65;A
		bl let2font
		mov r1, #2
		bl lcd_draw
		mov r0, #78;N
		bl let2font
		mov r1, #3
		bl lcd_draw
		mov r0, #84;T
		bl let2font
		mov r1, #4
		bl lcd_draw
		mov r0, #65;A
		bl let2font
		mov r1, #5
		bl lcd_draw
		
		mov r4, #0x130000;insert some delay. Can't test yet
delay0	subs r4, #1
		bne delay0
		
		bl		lcd_clear
		mov r0, #67;C
		bl let2font
		mov r1, #2
		bl lcd_draw
		mov r0, #76;L
		bl let2font
		mov r1, #3
		bl lcd_draw
		mov r0, #65;A
		bl let2font
		mov r1, #4
		bl lcd_draw
		mov r0, #82;R
		bl let2font
		mov r1, #5
		bl lcd_draw
		mov r0, #65;A
		bl let2font
		mov r1, #6
		bl lcd_draw
		
		mov r4, #0x130000;insert some delay. Can't test yet
delay1	subs r4, #1
		bne delay1
;******************************************
			
		b		endless		
		ENDP
			

			
			ALIGN						
			AREA    myData, DATA, READWRITE
			
			ALIGN			


	END
