
	
;******************** (C) Andrew Wolfe *******************************************
; @file    HW4 Problem 3
; @author  Andrew Wolfe
; @date    Aug. 13, 2023
;*******************************************************************************

	
			AREA    main, CODE, READONLY
			EXPORT	__main				
			ENTRY			
				
__main PROC
			ldr r0,=data
			ldr r1, =eodata
			ldr r2, =(output-data)
			mov r3, #0xf0000000
			mov r6, #0
loop1 		ldr r4, [r0]
			ands r5, r3, #0x80000000
			addne r6,r6,r4
			strne r6, [r0, r2]
			lsl r3, #1
			add r0,#4
			cmp r0, r1
			bne loop1
endless 	b endless
data dcd 9,3,-1,0, 8, -5, 0, 11, 0x33
eodata
output dcd 0,0,0,0,0,0,0,0,0
eooutput
END