    .intel_syntax noprefix
    
    .text
    .global _start

_start:
    	push	ebp         #Salva a base da pilha
	mov	ebp,esp     #Move a base para o topo
	push	ebx         #Salva os registradores de índice e base
	push	edi
	push	esi
    
    	mov	edi,[ebp+8]	#Endereço da string
	mov	esi,edi		#Move o endereço para ESI
	xor	ecx,ecx		#Zera ECX

find_end:
 	inc 	ecx
  	cmp 	byte [esi + ecx], 0x00; #Verifica se o caractere é nulo
  	jne 	find_end;
  
print: #Serve, basicamente, para printar a msg de tamanho len
    	mov     edx, ecx
    	mov     ecx, [edi]
    	mov     ebx, 1
    	mov     eax, 4
    	int     0x80
    
    	mov     ebx, eax  #Move código de erro ou sucesso para ebx (que é retornado_
    	mov     eax, 1
    	int     0x80
    
    	pop	    esi
	pop	    edi
	pop	    ebx
	pop	    ebp
	ret
