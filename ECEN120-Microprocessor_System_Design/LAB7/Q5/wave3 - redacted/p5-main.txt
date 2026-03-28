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
	INCLUDE timer.h
	INCLUDE leds.h
	INCLUDE	dac.h
	INCLUDE	adc.h
	IMPORT	sintbl	
	IMPORT	sawtbl		
	
sample_freq	equ	20000		;20KHz sampling rate
sample_per	equ	1000000/sample_freq			
test_freq	equ	600
	
			AREA    main, CODE, READONLY
			EXPORT	__main				
			ENTRY			
				
__main	PROC
		ldr		r0,=test_freq	
		bl		calc_phaseinc		;compute the phase increment value (phaseinc)
		ldr		r2,=phaseinc
		str		r0,[r2]				;store the phase increment value in memory
		
		bl		dac_init			;initialize dac
		bl		tim2_init			;initialize timer interrupt
		ldr		r0,=sample_per		;set output rate to 20KHz
		bl		tim2_freq
		
		bl 		adc_init
		
loop	bl 		adc_ch6
		bl 		adc_read
		ldr r1, =gain
		str r0, [r1]
		
		bl 		adc_ch5
		bl 		adc_read
		
		bl 		calc_phaseinc
		ldr		r2, =phaseinc
		str 	r0, [r2]
		b loop



endless	b		endless		
		ENDP
			
			
TIM2_IRQHandler PROC
		EXPORT	TIM2_IRQHandler
		push	{lr}
		ldr		r0,=phase			;get a pointer to the current phase
		ldr		r1,=sintbl			;Get pointer to waveform table | change to sawtbl for saw table values
		bl		get_tblval
		bl		dac_set
		ldr		r1,=phaseinc		;load phase increment
		ldr		r0,=phase			;reload last phase value
		bl		update_phase
		pop		{lr}
		ldr		r2,=(TIM2_BASE+TIM_SR)	;reset pending interrupt for TIM2		
		mov		r1,#~TIM_SR_UIF
		str		r1,[r2]
		dsb
		bx		lr
		ENDP
			
calc_phaseinc	PROC
					; To calculate the phaseinc, take the new frequency (w)/sampling freq.(w0) * 1024
					; to avoid precision issues - we will keep phase in 16ths then divide at the last minute
					; w arrives in r0; phase increment returned in r0
					; works from about 2Hz to sampling freq./2
					; Assumes a wave table size of 1024 and a phase iterator scaled up by 16

;Put your code here.
		LDR r1, =sample_freq
		LSL r0, #4
		LSL r0, #10
		UDIV r0, r0, r1

		bx		lr
		ENDP

update_phase	PROC
					;recieves a pointer to phase in r0 and a pointer to phaseinc in r1
					;adds phaseinc to phase
;Put your code here.
		LDR r2, [r0]
		LDR r3, [r1]
		ADD r3, r2
		BIC r3, #0x4000
		STR r3, [r0]
		bx		lr
		ENDP
		
get_tblval		PROC
					;recieves a pointer to phase in r0 and a pointer to a wave table in r1
					;Assume the wave table is 1024 entries; 16-bits each
					;Assume the phase value is in 16ths.
					;Return the sample in r0
				
;Put your code here.
		LDR r2, [r0]
		LSR r2, #4
		LSL r2, #1
		ADD r1, r2
		LDRH r0, [r1]
		
		LDR r8, =gain
		LDR r9, [r8]
		MUL r0, r9
		LSR r0, #12
		bx		lr
		ENDP
			
			
			ALIGN						
			AREA    myData, DATA, READWRITE
			
			ALIGN			
phase		dcd		0					;maintain in 16ths.
phaseinc	dcd		160	
gain 		dcd 	0

	END
