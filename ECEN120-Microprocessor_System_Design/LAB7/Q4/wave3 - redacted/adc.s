
	INCLUDE core_cm4_constants.s		; Load Constant Definitions
	INCLUDE stm32l476xx_constants.s 
		
	AREA    main, CODE, READONLY
		
; Interface routines for ADC1 Channel 6
; Available at pin PA 1

; ADC intitialization derived from Figure 20-12 in Zhu
adc_init	PROC
		EXPORT	adc_init
		ldr		r2,=(RCC_BASE+RCC_CR)		;Turn on HSI clock
		ldr		r1,[r2]
		orr		r1,#RCC_CR_HSION
		str		r1,[r2]
adcw1	ldr		r1,[r2]						;Wait for HSI Ready
		tst		r1,#RCC_CR_HSIRDY
		beq		adcw1
		ldr		r2,=(RCC_BASE+RCC_AHB2ENR)		;Turn on port A clock (bit 0)
		ldr		r1,[r2]
		orr		r1,#RCC_AHB2ENR_GPIOAEN
		str		r1,[r2]
		ldr		r2,=(GPIOA_BASE+GPIO_MODER)		;program bits 2-3 in GPIOA_MODER for pin 1 analog mode (11)
		ldr		r1,[r2]
		orr		r1,#(0x3 << 2)
		str		r1,[r2]				
		ldr		r2,=(GPIOA_BASE+GPIO_ASCR)		;Enable the analog switch ASC1
		ldr		r1,[r2]
		orr		r1,#GPIO_ASCR_EN_1
		str		r1,[r2]
		ldr		r2,=(RCC_BASE+RCC_AHB2ENR)		;Turn on ADC clock
		ldr		r1,[r2]
		orr		r1,#RCC_AHB2ENR_ADCEN
		str		r1,[r2]	
		ldr		r2,=(RCC_BASE+RCC_AHB2RSTR)		;reset ADC
		ldr		r1,[r2]
		orr		r1,#RCC_AHB2RSTR_ADCRST
		str		r1,[r2]
		mov		r0,#10
adcw11	subs	r0,#1
		bne		adcw11
		bic		r1,#RCC_AHB2RSTR_ADCRST
		str		r1,[r2]	
		ldr		r2,=(ADC1_BASE+ADC_CR)		;Disable ADC1
		ldr		r1,[r2]
		bic		r1,#ADC_CR_ADEN
		str		r1,[r2]
		ldr		r2,=(SYSCFG_BASE+SYSCFG_CFGR1)		;Enable analog switch booster
		ldr		r1,[r2]
		orr		r1,#SYSCFG_CFGR1_BOOSTEN
		str		r1,[r2]		
		ldr		r2,=(ADC1_BASE+ADC_CCR)		;Turn on vfrefen;set prescaler to 0000;set ckmode; independent mode
		ldr		r1,[r2]
		orr		r1,#ADC_CCR_VREFEN
		bic		r1,#ADC_CCR_PRESC
		bic		r1,#ADC_CCR_CKMODE
		orr		r1,#ADC_CCR_CKMODE_0
		bic		r1,#ADC_CCR_DUAL
		orr		r1,#6
		str		r1,[r2]
		ldr		r2,=(ADC1_BASE+ADC_CR)		;power up ADC1 and voltage regulator
		ldr		r1,[r2]
		bic		r1,#ADC_CR_DEEPPWD
		str		r1,[r2]
		orr		r1,#ADC_CR_ADVREGEN
		str		r1,[r2]		
		mov		r1,#(28 * 20)					;About 22µs delay at 80MHz
adcw2	subs	r1,#1
		bne		adcw2
		ldr		r2,=(ADC1_BASE+ADC_CFGR)		;set resolution to 12 bits, right aligned
		ldr		r1,[r2]
		bic		r1,#ADC_CFGR_RES
		bic		r1,#ADC_CFGR_ALIGN
		str		r1,[r2]		
		ldr		r2,=(ADC1_BASE+ADC_SQR1)		;set conversion sequence to 1 channel, channel 6
		ldr		r1,[r2]
		bic		r1,#ADC_SQR1_L
		bic		r1,#ADC_SQR1_SQ1
		orr		r1,#(6 << 6)
		str		r1,[r2]			
		ldr		r2,=(ADC1_BASE+ADC_DIFSEL)		;set channel 6 to single-ended
		ldr		r1,[r2]
		bic		r1,#ADC_DIFSEL_DIFSEL_6
		str		r1,[r2]
		ldr		r2,=(ADC1_BASE+ADC_SMPR1)		;set channel 6 sample time to 47 clocks
		ldr		r1,[r2]
		bic		r1,#ADC_SMPR1_SMP6
		orr		r1,#(3<<18)	
		str		r1,[r2]
		ldr		r2,=(ADC1_BASE+ADC_CFGR)		;select discontiguous mode; software trigger
		ldr		r1,[r2]
		bic		r1,#ADC_CFGR_CONT
		bic		r1,#ADC_CFGR_EXTEN
		str		r1,[r2]	
		ldr		r2,=(ADC1_BASE+ADC_CR)		;Enable ADC1
		ldr		r1,[r2]
		orr		r1,#ADC_CR_ADEN
		str		r1,[r2]		
		ldr		r2,=(ADC1_BASE+ADC_ISR)		;wait for ADC ready
adcw3	ldr		r1,[r2]	
		tst		r1,#ADC_ISR_ADRDY
		beq		adcw3
		
		bx		lr
			ENDP


adc_read	PROC		;return ADC channel 6 value in r0
				EXPORT	adc_read	
		ldr r1, =(ADC1_BASE+ADC_CR)
		ldr r2, [r1]
		bic r2, #0x80000000
		bic r2, #0x0000003f
		orr r2, #ADC_CR_ADSTART
		str r2, [r1]
loop	ldr r1, =(ADC1_BASE + ADC_CSR)
		ldr r2, [r1]
		cmp r2, #ADC_CSR_EOC_MST
		bne out
		b loop
out		ldr r1, =(ADC1_BASE+ADC_DR)
		ldr r0, [r1]
		
		bx		lr
			ENDP
			ALIGN
			END
			