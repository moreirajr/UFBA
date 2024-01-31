%include "asm_io.inc"

segment .data
	espaco	db	' '	
segment .bss
	vetor		resd		8
	aux		resd		1
	jafoi		resd		1
	loopbubble	resd 		1
	aux_imp		resd		1

	i		resd	1
	j		resd	1
	k		resd	1
	tam		resd 	1
	vetor8		resd	8
	vetore4		resd	4
	vetord4		resd	4
	vetoree2	resd	2
	vetored2	resd	2
	vetorde2	resd	2
	vetordd2	resd	2
	vetoreee1	resd	1
	vetoreed1	resd	1
	vetorede1	resd	1
	vetoredd1	resd	1
	vetordee1	resd	1
	vetorded1	resd	1
	vetordde1	resd	1
	vetorddd1	resd	1
	tamp		resd	1
	tamordenado	resd	1
segment .text
	global asm_main
asm_main:
	enter 	0,0
	
	mov	ecx, 8
	mov	edi, vetor
	cld
inserir:
	call	read_int
	stosd
	loop	inserir
	
escolha:
	xor	eax, eax
	call	read_char
	cmp	eax, "b"
	je	bubbleSort
	cmp	eax, "m"
	je	mergesoort
	jmp	escolha


printprimeira:
	mov	ecx, 8
	mov	esi, vetor
	cld
	imprimeinicio:
	lodsd	
	call	print_int
	mov	eax, espaco
	call	print_string	
	loop	imprimeinicio
	call	print_nl
	ret

	

bubbleSort:
	call	printprimeira
	mov	[jafoi], dword 0
	mov	[loopbubble], dword 8
	loopsort:
	sub	[loopbubble], dword 1
	jmp	passear
	continuapassear:
	cmp	[loopbubble], dword 0
	jne	loopsort
	jmp	fim



troca:
	mov	ebx, [i]
	mov	[j], ebx
	mov	edi, vetor
	mov	eax, [edi + ebx]
	add	ebx, 4
	mov	edx, [edi + ebx]
	mov	[edi + ebx], eax
	sub	ebx, 4
	mov	[edi + ebx], edx
	jmp	foitrocado

passear:
	mov	esi, vetor
	mov	[i], dword 0
	proximo:
	mov	ebx, 28
	sub	ebx, [jafoi]
	cmp	ebx, [i]
	jg	continua
	jmp	acaboupassear
	continua:
	mov	edx, [i]
	mov	ebx, [esi + edx]
	add	edx, 4
	cmp	ebx, [esi + edx]
	jg	troca
	jmp	naofoitrocado
	foitrocado:
	jmp	imprimir
	acabouimprimir:
	naofoitrocado:
	add	[i], dword 4
	jmp 	proximo
	acaboupassear:
	add	[jafoi], dword 4
	jmp	continuapassear


imprimir:
	mov	ecx, 8
	mov	[aux_imp], dword 0
	push	ebx
	print:
	cmp	[aux_imp], dword 28
	jg	sairimprimir
	mov	ebx, [aux_imp]
	mov	eax, [esi + ebx]
	call	print_int
	mov	eax, espaco
	call	print_string
	add	[aux_imp], dword  4
	jmp	print
	sairimprimir:
	call	print_nl
	pop	ebx
	jmp	acabouimprimir
	




















; MERGESORT


mergesoort:
	call	printprimeira
	mov	esi, vetor
	mov	ebx, 0
	mov	edx, [esi + ebx]
	mov	[vetoreee1], edx
	add	ebx, dword 4
	mov	esi, vetor
	mov	edx, [esi + ebx]
	mov	[vetoreed1], edx
	add	ebx, dword 4
	mov	esi, vetor
	mov	edx, [esi + ebx]
	mov	[vetorede1], edx
	add	ebx, dword 4
	mov	esi, vetor
	mov	edx, [esi + ebx]
	mov	[vetoredd1], edx
	add	ebx, dword 4
	mov	esi, vetor
	mov	edx, [esi + ebx]
	mov	[vetordee1], edx
	add	ebx, dword 4
	mov	esi, vetor
	mov	edx, [esi + ebx]
	mov	[vetorded1], edx
	add	ebx, dword 4
	mov	esi, vetor
	mov	edx, [esi + ebx]
	mov	[vetordde1], edx
	add	ebx, dword 4
	mov	esi, vetor
	mov	edx, [esi + ebx]
	mov	[vetorddd1], edx
	
	;PASSO 1
	mov	esi, vetoreee1
	mov	edx, vetoreed1
	mov	edi, vetoree2
	mov	[tam], dword 0
	mov	[tamordenado], dword 4
	call	intercala
	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetoree2
	call	printavetor

	
	mov	[i], dword 8
	mov	[tamp], dword 6
	mov	esi, vetor
	call	printavetor
	call	print_nl

	;PASSO 2


	mov	esi, vetorede1
	mov	edx, vetoredd1
	mov	edi, vetored2
	mov	[tam], dword 0
	mov	[tamordenado], dword 4
	call	intercala
	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetoree2
	call	printavetor
	
	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetored2
	call	printavetor

	
	mov	[i], dword 16
	mov	[tamp], dword 4
	mov	esi, vetor
	call	printavetor
	call	print_nl
	;PASSO 3

	mov	esi, vetordee1
	mov	edx, vetorded1
	mov	edi, vetorde2
	mov	[tam], dword 0
	mov	[tamordenado], dword 4
	call	intercala

	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetoree2
	call	printavetor
	
	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetored2
	call	printavetor

	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetorde2
	call	printavetor
	
	mov	[i], dword 24
	mov	[tamp], dword 2
	mov	esi, vetor
	call	printavetor
	call	print_nl

	;PASSO 4
	
	mov	esi, vetordde1
	mov	edx, vetorddd1
	mov	edi, vetordd2
	mov	[tam], dword 0
	mov	[tamordenado], dword 4
	call	intercala

	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetoree2
	call	printavetor
	
	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetored2
	call	printavetor

	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetorde2
	call	printavetor
	
	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetordd2
	call	printavetor
	call	print_nl

	;PASSO 5

	mov	esi, vetoree2
	mov	edx, vetored2
	mov	edi, vetore4
	mov	[tam], dword 4
	mov	[tamordenado], dword 16
	call	intercala

	mov	[i], dword 0
	mov	[tamp], dword 4
	mov	esi, vetore4
	call	printavetor

	
	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetorde2
	call	printavetor

	mov	[i], dword 0
	mov	[tamp], dword 2
	mov	esi, vetordd2
	call	printavetor
	call	print_nl

	;PASSO 6
	mov	esi, vetorde2
	mov	edx, vetordd2
	mov	edi, vetord4
	mov	[tam], dword 4
	mov	[tamordenado], dword 16
	call	intercala

	mov	[i], dword 0
	mov	[tamp], dword 4
	mov	esi, vetore4
	call	printavetor
	
	
	mov	[i], dword 0
	mov	[tamp], dword 4
	mov	esi, vetord4
	call	printavetor


	call	print_nl

	;PASSO 7
	mov	esi, vetore4
	mov	edx, vetord4
	mov	edi, vetor8
	mov	[tam], dword 12
	mov	[tamordenado], dword 32
	call	intercala

	mov	[i], dword 0
	mov	[tamp], dword 8
	mov	esi, vetor8
	call	printavetor
	call	print_nl

	jmp	fim


	
	
	
intercala:
	mov	[i], dword 0
	mov	[j], dword 0
	mov	[k], dword 0
	for:
	mov	ecx, [k]
	cmp	ecx, [tamordenado]
	jg	fimintercala
	mov	ebx, [tam]
	cmp	ebx, [i]
	jl	entraj
	cmp	ebx, [j]
	jl	entrai
	mov	ebx, [i]
	mov	eax, [esi + ebx]
	mov	ebx, [j]
	cmp	eax, [edx + ebx]
	jl	entrai
	jmp	entraj


	entrai:
	mov	ebx, [i]
	mov	ecx, [k]
	mov	eax, [esi + ebx]
	mov	[edi + ecx], eax
	add	[i], dword 4
	add	[k], dword 4
	jmp 	for

	entraj:
	mov	ebx, [j]
	mov	ecx, [k]
	mov	eax, [edx + ebx]
	mov	[edi + ecx], eax
	add	[j], dword 4
	add	[k], dword 4
	jmp 	for
	fimintercala:
	ret




printavetor:
	mov	ecx, [tamp]
	add	esi, [i]
	cld
	printM:
	lodsd
	call	print_int
	mov	eax, espaco
	call	print_string
	loop	printM
	ret

	
	
fim:	
	call	print_nl
	leave
	ret



























