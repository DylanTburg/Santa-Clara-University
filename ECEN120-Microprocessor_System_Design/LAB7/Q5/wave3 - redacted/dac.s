
	INCLUDE core_cm4_constants.s		; Load Constant Definitions
	INCLUDE stm32l476xx_constants.s 
		
	AREA    main, CODE, READONLY
		
;Interface routines for DAC Channel 2
;Available at pin PA 5

dac_init	PROC
	EXPORT dac_init
		ldr		r2,=(RCC_BASE+RCC_APB1ENR1)		;enable DAC clock  - it seems to be called DAC1 for both
		ldr		r1,[r2]
		orr		r1,#RCC_APB1ENR1_DAC1EN
		str		r1,[r2]
		ldr		r2,=(DAC1_BASE+DAC_CR)			;disable DAC - both channels - req. to set mode
		ldr		r1,[r2]
		bic		r1,#DAC_CR_EN1
		bic		r1,#DAC_CR_EN2
		str		r1,[r2]
		ldr		r2,=(DAC1_BASE+DAC_MCR)			;Set DAC channel 2 mode to ext. pin with buffer (000) (default)
		ldr		r1,[r2]
		bic		r1,#0x70000
		str		r1,[r2]
		ldr		r2,=(DAC1_BASE+DAC_CR)			;Set the trigger mode and enable triggers and enable the DAC channel
		ldr		r1,[r2]
		orr		r1,#DAC_CR_TSEL2
		orr		r1,#DAC_CR_TEN2
		orr		r1,#DAC_CR_EN2
		str		r1,[r2]	
		ldr		r2,=(RCC_BASE+RCC_AHB2ENR)		;Turn on port A clock (bit 0)
		ldr		r1,[r2]
		orr		r1,#RCC_AHB2ENR_GPIOAEN
		str		r1,[r2]
		ldr		r2,=(GPIOA_BASE+GPIO_MODER)		;program bits 10-11 in GPIOA_MODER for pin 5 analog mode (11)
		ldr		r1,[r2]
		orr		r1,#(0x3 << 10)
		str		r1,[r2]	
		bx		lr
		ENDP
						
dac_set		PROC								;12-bit right-aligned value in r0
		EXPORT dac_set
		ldr		r2,=(DAC1_BASE+DAC_SR)			;check to make sure the DAC buffer is not full
ds_1	ldr		r1,[r2]
		tst		r1,#DAC_SR_BWST2				;if bit is 1 (busy), Z flag will be clear
		bne		ds_1
		ldr		r2,=(DAC_BASE+DAC_DHR12R2)		;write data to 12-bit right-aligned register
		str		r0,[r2]
		ldr		r2,=(DAC_BASE+DAC_SWTRIGR)		;Trigger DAC write
		mov		r1,#DAC_SWTRIGR_SWTRIG2
		str		r1,[r2]
		bx		lr
		ENDP
		
		
		END