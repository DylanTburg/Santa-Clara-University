

			AREA main, CODE, READONLY
			EXPORT __main
			ENTRY

__main PROC
		LDR     r0, =p
		LDR     r1, =eol1       ; Load p from memory 
		MOV		r2, #0
		BL      sum             ; Call the sum
endless B endless            ; Endless loop to halt execution

	ENDP

sum PROC
		CMP     r0, r1      
		BEQ     base0
		LDR 	r2, [r0]
		ADD 	r3, r2
		ADD 	r0, #4
		PUSH    {lr}		
		BL sum
		POP		{lr}
		BX		lr
base0   BX      lr

	ENDP

p DCD 0x1, 0x2, 0x3, 0x4, 0x5, 0x6
eol1
END