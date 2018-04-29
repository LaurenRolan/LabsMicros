    .intel_syntax noprefix
    
    .text
    .global _start

_start:
    mov     edx, offset len
    mov     ecx, offset msg
    mov     ebx, 1
    mov     eax, 4
    int     0x80
    
    xor     ebx, ebx
    mov     eax, 1
    int     0x80
    
    .data
msg:
    .string "Hello, world!\n"
len= . - msg
