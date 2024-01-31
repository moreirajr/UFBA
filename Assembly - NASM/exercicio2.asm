;decimal pra binario


%include "asm_io.inc"
;
; initialized data is put in the .data segment
;
segment .data
;
; These labels refer to strings used for output
;


prompt1 db    "Enter a number: ", 0       ; don't forget nul terminator

igual db    " = ", 0

;
; uninitialized data is put in the .bss segment
;
segment .bss

number1 resd   1               ; 32bits reserve a double word



;
; code is put in the .text segment
;
segment .text
        global  _asm_main
_asm_main:

        mov     eax, prompt1
        call    print_string    
        
        call    read_int          ; armazena no eax
		
		mov ecx, 16			;contador do loop
		
		;shl eax, 16
		;mov	bx, eax
		
		mov [number1], eax
		mov bx, [number1]
		
	
		loop1:
			shl bx, 1
			jc loop_carry
			
			mov eax, 0
			call print_int
			jmp loop2
		
		loop_carry:
			mov eax, 1
			call print_int
		
		loop2:
			loop loop1
		      
       
        
        leave        
        ret
