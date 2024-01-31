;Evaldo M. Moreira Junior


%include "asm_io.inc"
;
; initialized data is put in the .data segment
;
segment .data
;
; These labels refer to strings used for output
;


;prompt1 db    "Enter a number: ", 0       ; don't forget null terminator
;vetor	times 5 dd -1

vetor times 16 dd 0
numero dd 8

;
; uninitialized data is put in the .bss segment
;
segment .bss

;number resd   1               ; 32bits reserve a double word

;
; code is put in the .text segment
;
segment .text
        global  _asm_main
_asm_main:

	;mov     eax, prompt1
	;call    print_string
	
	cld 
	mov ecx, 16
	mov edx, 0
	mov bx, [numero]
	
	lp:
	shl bx, 1
	jc carry1
	
	mov dword [vetor + edx], 0
	mov eax, [vetor + edx]
	jmp end_loop
	
	carry1:
	mov dword [vetor + edx], 1
	
	end_loop:
	add edx, 4
	loop lp
	
	mov ecx, 16
	mov edx, 0
	
	printVetor:
		mov eax, [vetor + edx]
		call print_int
		add edx, 4
	
	loop printVetor
		
	
	leave        
	ret	
		
		
	   
		      
       
        
	
