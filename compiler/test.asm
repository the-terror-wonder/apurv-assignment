bits 64
default rel
section .text
global main
main:
    mov rax, 60
    xor rdi, rdi
    syscall
