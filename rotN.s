@ Implement a Caesar cipher (rotation cipher)
@ TAG: assembler_print	
@ TEMPLATE code only, with main fct rotN to be filled in, callable from C
@ Note: isalph subroutine for check, parameterised with base value (#65 for 'A' and #97 for 'Z')
@	
@ > as -o rotN.o rotN.s
@ > as -o showIntCConv.o showIntCConv.s 
@ > ld -o rotN rotN.o showIntCConv.o
@ > ./rotN
@ > echo $?
@ 43
@ -----------------------------------------------------------------------------

.text
.global rotN	
.global ___start
___start: NOP
	LDR  R2, =teststr	@ pointer to input string
	@LDR  R2, =resstr	@ this version does in-place modification, so no separate output ptr

	MOV  R0, R2		@ arg 1: ptr to input
	MOV  R1, #ROT		@ arg 2: rotation value
	BL   rotN		@ result: R0 ... ptr to rotated string
	
exit1:	
	@MOV	 R0, R4		@ load result to output register
	MOV 	 R7, #1		@ load system call code
	SWI 	 0		@ return this value

	
@ -----------------------------------------------------------------------------	
@ based on VERSION 6 of the rot13 exercises, using sub-routines, and handles only
@ upper and lower case characters, printing in Assembler
@ in-place modification of input string
@ R2 = pointer to string	

rotN:				@ Input: R0 = ptr to string ; R1 = key (rotation value)
	@... COMPLETE CODE HERE ...

@branches to isalph to check if the given string contains only alphabet characters or not
	BL isalph

@if the register 4 is 0, then exit the program as the string contains non-alphabetic characters
	CMP R4, #0
	BEQ exit1
@create a pointer in Register 0 to R2, which access the string	
	MOV R0, R2
	B _rotNloop
	@BX  LR			@ Output: R0 = ptr to the rotate string (same as input ptr)
	

_rotNloop:
	@load content of r0 into r3 byte by byte	
	LDRB R3,[R0]
	
	@check if the character receieved by r3 is a null terminator or not
	@if its a null terminator, it means this is the end of string, exit the program
	CMP R3, #0
	BEQ exit1
	
	@check if the recevied character is a space or not, if it is, then skip to the next character as we do not want to shift ascii value of space
	CMP R3, #' '
	BEQ _rotNspace
	
	@we have already checked in the isalph function if the string contains any non-alphabetic characters
	@now check if the recevied character is a Upper-case alphabet or a Lower-case alphabet
	CMP R3, #'Z'
	BLE _rotUpperCase
	BGT _rotLowerCase
	
_rotUpperCase:
	@if the character+key exceeds the 'Z', loop back to 'A' by subtracting 26 from the ascii value
	MOV R6, R3
	ADD R6, R1
	CMP R6, #'Z'
	BLGT _chargreat
	ADD R3, R1
	@store the encrypted character in the string
	STRB R3, [R0], #1
	B _rotNloop

_rotLowerCase:
	@if the character+key exceeds the 'z', loop back to 'A' by subtracting 26 from the ascii value
	MOV R6, R3
	ADD R6, R1
	CMP R6, #'z'
	BLGT _chargreat
	ADD R3, R1
	@store the encrypted character in the string
	STRB R3, [R0], #1
	B _rotNloop

_chargreat:
	@subtracting 26 from the original ascii value of the character
	SUB R3, #26
	BX LR

_rotNspace:
	@if the recieved character is a space, skip to the next character
	ADD R0, #1
	B _rotNloop

isalph:
	@load the content of the string character by character
	LDRB R3,[R0], #1
	
	@check if the character receieved by r3 is a null terminator or not
	@if its a null terminator, it means this is the end of string, end the isalph function
	CMP R3, #0
	BEQ _isalphaEND
	
	@skip to the next character if its a space
	CMP R3, #' '
	BEQ isalph
	
	@if the received character has a ascii value lower than 'A', then its not a alphabet
	CMP R3, #'A'
	BLT _notalph
	
	@if the received character has a ascii value greater than 'z', then its not a alphabet
	CMP R3, #'z'
	BGT _notalph
	
	@move 1 in the register 4 to tell the program it hasn't encountered a non-alphabetic character in the string
	MOV R4, #1
	
	@loops back to itself until it reaches null terminator
	BAL isalph


_isalphaEND:
	@go back to the rotN function
	BX LR

	
_notalph:
	@if any non-alphabetic character has been found, store 0 in register 4 to tell it to the program
	MOV R4, #0
	BX lr


@ -----------------------------------------------------------------------------

.data
.align 4
	
fstr: .asciz "%s\n"
f1str: .asciz "In:  '%s'\n"
f2str: .asciz "Out: '%s'\n"
f3str: .asciz "Exp: '%s'\n"
gstr: .asciz "Expexted: %s\n"

n: .word 0x00
char: .word 0x00
fin: .word 0x00

.equ MAXSIZE, 32
.equ ROT, 13

	
.align 4
@ a simple test string
teststr:	@ 9 words, 43 chars
.asciz "The quick brown fox jumps over the lazy dog"

resultstr:	@ 9 words, 43 chars
	.asciz "Gur dhvpx oebja sbk whzcf bire gur ynml qbt"

resstr:	.rept MAXSIZE
   .word 0 
   .endr
	
wod3_text:	@ 188 words (wc -w)
	.asciz "For the first time in her life, Molly Gibson was to be included among the guests at the Towers. She was much too young to be a visitor at the school, so it was not on that account that she was to go; but it had so happened that one day when Lord Cumnor was on a \"pottering\" expedition, he had met Mr. Gibson, the doctor of the neighbourhood, coming out of the farm-house my lord was entering; and having some small question to ask the surgeon (Lord Cumnor seldom passed any one of his acquaintance without asking a question of some sort—not always attending to the answer; it was his mode of conversation), he accompanied Mr. Gibson to the out-building, to a ring in the wall of which the surgeon's horse was fastened. Molly was there too, sitting square and quiet on her rough little pony, waiting for her father. Her grave eyes opened large and wide at the close neighbourhood and evident advance of \"the earl;\" for to her little imagination the grey-haired, red-faced, somewhat clumsy man, was a cross between an arch-angel and a king."	
