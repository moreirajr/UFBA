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

;print_teste db "teste",0
;printempilhar db "empilhando",0
;printdesemp db "desempilhando",0



NUM dd 0
;
; uninitialized data is put in the .bss segment
;
segment .bss

;number resd   1               ; 32bits reserve a double word
vetor resb 20

;resultado resd 1
aux resb 1

;
; code is put in the .text segment
;
segment .text
        global  _asm_main
_asm_main:

	mov ecx, 20
	mov edi, vetor
	
	cld
	mov eax, printdesemp
	
	ler_entrada:
		call read_char
		stosb
		;call print_char
		cmp AL, '0'
		je continue
	loop ler_entrada
	
	
	continue:
		
		mov ecx, 20
		mov esi, vetor
		cld
		
		notacao_posfixa:
			lodsb
								
			cmp AL, '0'
			je fim
			
			cmp Al, '+'
			je empilhar
			
						
			cmp AL, '-'
			je empilhar
						
			cmp AL, '*'
			je empilhar
						
			cmp AL, '/'
			je empilhar
			
			cmp Al, '('
			je continue_2
			
			cmp AL, ')'
			je desempilhar
						
			call print_char
			jmp continue_2
						
			
		continue_2: 
		loop notacao_posfixa
		
		empilhar:
			inc dword [NUM]
			movzx eax, al
			push eax
			jmp continue_2
		
		desempilhar:
			dec dword [NUM]
			pop eax
			mov [aux], eax
			mov al, [aux]
			call print_char
			jmp continue_2
	
	
	
	desempilhar_restante:
		dec dword [NUM]
		pop eax
		mov [aux], eax
		mov al, [aux]
		call print_char
	
	fim:
		cmp [NUM], dword 0
		jg desempilhar_restante
	leave        
	ret