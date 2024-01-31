;
; To create executable:
; Using djgpp:
; nasm -f coff first.asm
; gcc -o first first.o driver.c asm_io.o
;
; Using Linux and gcc:
; nasm -f elf first.asm
; gcc -o first first.o driver.c asm_io.o
;
; Using Borland C/C++
; nasm -f obj first.asm
; bcc32 first.obj driver.c asm_io.obj
;
; Using MS C/C++
; nasm -f win32 first.asm
; cl first.obj driver.c asm_io.obj
;
; Using Open Watcom
; nasm -f obj first.asm
; wcl386 first.obj driver.c asm_io.obj
%include "asm_io.inc"
;
; initialized data is put in the .data segment
;
segment .data
;
; These labels refer to strings used for output
;
prompt1 db    "Enter a first number: ", 0       ; don't forget nul terminator
prompt2 db    "Enter a second number: ", 0
soma db    " + ", 0
subtracao db    " - ", 0
multiplicacao db    " * ", 0
divisao db    " / ", 0
resto db    " % ", 0
igual db    " = ", 0

;
; uninitialized data is put in the .bss segment
;
segment .bss

number1 resd   1               ; 32bits reserve a double word
number2 resd   1               ; 32bits reserve a double word  


;
; code is put in the .text segment
;
segment .text
        global  _asm_main
_asm_main:

        mov     eax, prompt1
        call    print_string        
        
        call    read_int          ; armazena no eax
        mov     [number1], eax    ; number1 = eax
        
        mov     eax, prompt2
        call    print_string
        
        call    read_int
        mov     [number2], eax
        
        ; ==== soma ===
        mov     ebx, [number1] ; ebx = number1
        add     ebx, [number2] ; ebx = ebx + number1
        
        mov     eax, [number1]
        call    print_int
        
        mov     eax, soma
        call    print_string
        
        mov     eax, [number2]
        call    print_int
        
        mov     eax, igual
        call    print_string
        
        mov     eax, ebx
        call    print_int
        call    print_nl
        
        ; === subtracao ===
        
        mov     ebx, [number1] ; ebx = number1
        sub     ebx, [number2] ; ebx = ebx - number1
        
        mov     eax, [number1]
        call    print_int
        
        mov     eax, subtracao
        call    print_string
        
        mov     eax, [number2]
        call    print_int
        
        mov     eax, igual
        call    print_string
        
        mov     eax, ebx
        call    print_int
        call    print_nl
        
        ; === multiplicacao ===
        
        mov ebx, [number1]
        imul ebx, [number2]
        
        mov     eax, [number1]
        call    print_int
        
        mov     eax, multiplicacao
        call    print_string
        
        mov     eax, [number2]
        call    print_int
        
        mov     eax, igual
        call    print_string
        
        mov     eax, ebx
        call    print_int
        call    print_nl
        
        mov     eax, [number1]
        cdq ; o dividendo eh edx:eax, entao eh necessario estender o sinal
        mov ebx, [number2]
        idiv ebx ; o resultado esta em eax e o resto esta em edx
        mov ebx, eax
        
        mov     eax, [number1]
        call    print_int
        
        mov     eax, divisao
        call    print_string
        
        mov     eax, [number2]
        call    print_int
        
        mov     eax, igual
        call    print_string
        
        mov     eax, ebx
        call    print_int
        call    print_nl
        
        mov     ebx, edx
                mov     eax, [number1]
        call    print_int
        
        mov     eax, resto
        call    print_string
        
        mov     eax, [number2]
        call    print_int
        
        mov     eax, igual
        call    print_string
        
        mov     eax, ebx
        call    print_int
        call    print_nl
        
        leave        
        ret
