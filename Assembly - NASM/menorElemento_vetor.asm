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

vetor dd 7, 8, 5, 6, 4, 10, 30, 20

;
; uninitialized data is put in the .bss segment
;
segment .bss

;number resd   1               ; 32bits reserve a double word
menor resd 1
aux resd 1

;
; code is put in the .text segment
;
segment .text
        global  _asm_main
_asm_main:

	;mov     eax, prompt1
	;call    print_string
	
	cld
	mov esi, vetor
	mov eax, [esi]
	mov ecx, 8
		
	
	lp:
		cmp eax, [esi]
		jg troca_menor
		add esi, 4
		jmp fim_lp
		
		troca_menor:
		lodsd
		fim_lp:
	loop lp
	
	mov [menor], eax
	call print_nl
	call print_int
	
	leave        
	ret	
		
		
	   
		      
       
        
	
