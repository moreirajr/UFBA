%include "asm_io.inc"

segment .data

segment .bss


segment .text
        global  asm_main
        
asm_main:

    call read_int
    
    mov edx, eax
    mov eax, 0
    push edx
    call fib
    pop ecx
    
    call print_int
    call print_nl
    
        
leave
ret 

fib:
     enter 0,0
;    push ebp
;    mov ebp, esp

    mov ebx, [ebp+8]

    
    
    cmp ebx, 0
    je fim
    cmp ebx, 1
    je fib_um
    
    dec ebx ; n-1
    push ebx
    call fib
    pop ebx ; eh preciso desempilhar no ebx para a conservar o valor dele pra proxima chamada abaixo 
    
    
    dec ebx ; n-2
    
    push ebx
    call fib
    pop ecx
    
    jmp fim
    
    fib_um:
        inc eax
        
    fim:
    leave
    ; mov esp, ebp
    ; pop ebp
ret