;Alunos:

;Danilo Pires
;Evaldo M. Moreira Junior


%include "asm_io.inc"
;
; initialized data is put in the .data segment
;
segment .data
;
; These labels refer to strings used for output
;


prompt1 db    "Primeira linha: valor de N valor de M. Nas M linhas seguintes: valores S T B", 0       ; don't forget null terminator
;vetor	times 5 dd -1
espaco db " ",0

;vetor_S times 100 dd -1	;pilares S
distancia times 51 dd 999	;iniciado com 999!


;
; uninitialized data is put in the .bss segment
;
segment .bss
               
N_pilares resd 1	; 32bits reserve a double word
M_pontes resd 1
vetor_S resd 100	;pilares S
vetor_B resd 100	;pilares T
vetor_T resd 100	;buracos B


AUX resd 1

inicio resd 1
destino resd 1	;acampamento, com valor n+1
num_caminhos resd 1

;
; code is put in the .text segment
;
segment .text
        global  _asm_main
_asm_main:



	;Importante:
	;admitindo que o programa possui as entradas ordenadas (em relacao ao vetor S), como nos exemplos do trabalho!
	;exemplo:
	;n x1 B
	;n x2 B
	;...
	;n+1 t1 B
	;n+2 t2 B
	;...
	;n+Z tZ B

	;ler valores de N (numero de pilares) e M (numero de pontes)
	call read_int
	mov [N_pilares], eax
	
	mov [destino], eax
	add [destino], dword 2
	
	
	call read_int
	mov [M_pontes], eax
	mov [num_caminhos], eax
	dec dword [num_caminhos]
	
	
	;M linhas: lendo os valores de S (pilares), T (pilares), e B (buracos)
	;existe uma ponte ligando S e T, com B buracos
	;0 e o inicio, e N_pilares+1 o acampamento
	mov edx, 0
	mov ecx, [M_pontes]
	ler_entrada:
			push vetor_B
			push vetor_T
			push vetor_S
					
			mov [AUX], ecx
			mov ecx, 3
			loop_leitura:
					pop esi
					call read_int
					mov [esi + edx], eax
			loop loop_leitura
			
			add edx, 4
			mov ecx, [AUX]
	
	loop ler_entrada
	
	
	;call teste_entrada
	;call testes
	
	call inicializar_caminhos
	;call testes
	
	
	call melhor_caminho
	
	;imprime o vetor distancias:
	;call testes
	
	;no vetor distancia, de tamanho N_pilares+2 (distancia[N_pilares+2]), em cada indice i, tem o melhor caminho, do inicio (indice 0) ao indice i
	;utilizando programacao dinamica
	
	
	;imprimir o melhor caminho do inicio ao acampamento:
	mov edx, 0
	mov eax, 4
	mov ecx, [N_pilares]
	add ecx, dword 1
	mul ecx
		
	mov ecx, eax
	mov eax, [distancia + ecx]
	call print_int
	
		
	
	leave        
ret	


;######################################################################################################################################################
;#######		Funcoes		#######	
;####### passar parametros:
;		push valor1,  push valor2, call funcao, pop ecx(desempilha parametro 2), pop ecx (desempilha parametro 1)
;####### acessando os parametros denro da funcao:
;		mov eax, [esp + 8] ;parametro 1
;		mov ebx, [esp + 4] ; parametro 2
;######################################################################################################################################################

teste_entrada:
	enter 0,0
	
	call print_nl
	call print_nl
	call print_nl
	mov eax, prompt1
	call print_string
	call print_nl
	
	mov eax, [N_pilares]
	call print_int
	mov eax, espaco
	call print_string
	mov eax, [M_pontes]
	call print_int
	call print_nl
	
	mov edx, 0
	mov ecx, [M_pontes]
	print_teste:
		mov eax, [vetor_S + edx]
		call print_int
		mov eax, espaco
		call print_string
		
		mov eax, [vetor_T + edx]
		call print_int
		mov eax, espaco
		call print_string
		
		mov eax, [vetor_B + edx]
		call print_int
		
		call print_nl
		add edx, 4
	
	loop print_teste
	
	call print_nl
	mov eax, [destino]
	call print_int
	
	leave
	ret

;######################################################################################################################################################

testes:
	enter 0,0
	
	
	call print_nl
	
	;teste: imprimir distancias
	
	mov ecx, [destino]
	mov edx, 0
	print_dist:
		mov eax, [distancia + edx]
		call print_int
		mov eax, espaco
		call print_string
		add edx, 4
	loop print_dist
	
		
	leave
	ret
	
;######################################################################################################################################################

inicializar_caminhos:
	enter 0,0
	
	cld

	mov ecx, -1
	
	dist:
	add ecx, dword 1
	
	mov edx, 0
	mov eax, 4
	mul ecx
		
	;eax o indice
	;[vetor_S + eax] o valor no indice eax
	;pegando as distancias ja conhecidas do indice 0 ate um indice i
	
	cmp [vetor_S + eax], dword 0
	je init_dist
	jmp end_init
	
	init_dist:
		mov esi, vetor_B
		add esi, eax
		
		mov [AUX], eax
		push ecx
		mov ecx, [vetor_T + eax]
					
		mov eax, 4
		mul ecx
		
		mov edi, distancia
		add edi, eax
		
		pop ecx
		movsd	;copiar de esi pra edi
	end_init:
	cmp ecx, [num_caminhos]
	jl dist
	
		
	leave
	ret
	
;######################################################################################################################################################

melhor_caminho:
	enter 0,0
		
	cld
		
	mov ecx, -1
	
	melhor_c:
		add ecx, dword 1
		
		mov edx, 0
		mov eax, 4
		mul ecx
		
		;eax = indice do vetor, usando [aux] pra nao perder o valor
		;valor da distancia do inicio ao vetor_T[i] (0 ao valor de v_T[i],    distancia[ vetor_T[i] ] )
		;(codigo em C)
		;if(distancia[ vetor_S[i] ] + vetor_B[i] < distancia[ vetor_T[i] ])
		;			distancia[ vetor_T[i] ] = distancia[ vetor_S[i] ] + vetor_B[i];
		
		mov [AUX], eax
		push ecx
		mov ecx, [vetor_T + eax]
		mov eax, 4
		mul ecx
		mov edi, distancia
		add edi, eax
		
		mov eax, [AUX]
		mov ecx, [vetor_S + eax]
		mov eax, 4
		mul ecx
		mov esi, distancia
		add esi, eax
		
		mov eax, [AUX]
		mov edx, [vetor_B + eax]
		
		add edx, [esi]
		
		cmp edx, [edi]
		jl pegar_melhor_caminho
		jmp fim_m
		
		pegar_melhor_caminho:
			mov [edi], edx	;copiar o melhor caminho
			
		fim_m:
		pop ecx
		cmp ecx, [num_caminhos]
		jl melhor_c
	
	
	leave
	ret
	
;######################################################################################################################################################

