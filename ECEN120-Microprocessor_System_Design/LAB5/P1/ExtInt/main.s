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
	
	

	
			AREA    main, CODE, READONLY
			EXPORT	__main				
			ENTRY			
				
__main	PROC
		ldr		r0,=RCC_AHB2ENR_GPIOEEN	
		bl		portclock_en				; enable port E clock		
		ldr		r0,=RCC_AHB2ENR_GPIOBEN	
		bl		portclock_en				; enable port B clock


		ldr		r0,=GPIOE_BASE
		ldr		r1,=GPIO_MODER_MODER8_0
		bl		port_bit_pushpull			;set port b.8 to push pull
		
		ldr		r0,=GPIOB_BASE
		ldr		r1,=GPIO_MODER_MODER2_0
		bl		port_bit_pushpull			;set port b.2 to push pull
		
		bl		porta_init			;initialize port A for this program
		bl		exti0_init			;initialize exti0 interrupt
		bl		exti3_init			;initialize exti3 interrupt
		
	
		

endless	b		endless		
		ENDP
			
			
EXTI0_IRQHandler PROC
		EXPORT	EXTI0_IRQHandler
		push	{lr}
		bl		green_tog
		pop		{lr}
		ldr		r2,=(EXTI_BASE+EXTI_PR1)	;reset pending interrupt for EXTI0		
		mov		r1,#EXTI_PR1_PIF0
		str		r1,[r2]
		dsb
		bx		lr
		ENDP

EXTI3_IRQHandler PROC
		EXPORT	EXTI3_IRQHandler
		push	{lr}
		bl		red_tog
		pop		{lr}
		ldr		r2,=(EXTI_BASE+EXTI_PR1)	;reset pending interrupt for EXTI3		
		mov		r1,#EXTI_PR1_PIF3
		str		r1,[r2]
		dsb
		bx		lr
		ENDP
			
			
			
			ALIGN						
			AREA    myData, DATA, READWRITE
			
			ALIGN			
	

	END
