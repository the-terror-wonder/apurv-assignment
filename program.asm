%include "macros.inc"

extern printf
extern scanf
extern sprintf
extern MessageBoxA
extern ExitProcess
extern CreateFileA
extern WriteFile
extern CloseHandle
extern SetFilePointer
global _start

section .data
    input_fmt     db "%d", 0
    output_fmt    db "Result = %d", 10, 0
    sprintf_fmt   db "Result = %d", 0
    msg_title     db "Cube Difference Result", 0
    msg_text      db 64 dup(0)

    promptA       db "Enter value for a: ", 0
    promptB       db "Enter value for b: ", 0

    ; ✅ Fixed backslashes here
    file_name     db "C:\\Users\\akpan\\Desktop\\projects\\Compiler_design_assignment\\sage_compiler\\values.txt", 0
    format_vals   db "a = %d, b = %d, result = %d", 10, 0  ; newline included

section .bss
    a resd 1
    b resd 1
    result resd 1
    buffer resb 128
    hFile resd 1
    written resd 1

section .text
_start:

    ; --- Input ---
    print_str promptA
    read_int a

    print_str promptB
    read_int b

    ; Compute (a - b)(a^2 + ab + b^2)
    mov eax, [a]
    sub eax, [b]
    mov ecx, eax      ; ecx = (a - b)

    mov eax, [a]
    imul eax, [a]
    mov esi, eax      ; esi = a^2

    mov eax, [a]
    imul eax, [b]
    mov edi, eax      ; edi = ab

    mov eax, [b]
    imul eax, [b]     ; eax = b^2

    add esi, edi      ; esi = a^2 + ab
    add esi, eax      ; esi = a^2 + ab + b^2

    mov eax, ecx
    imul eax, esi     ; result = (a - b)(...)
    mov [result], eax

    ; --- Output ---
    print_int [result]
    msgbox_int [result]

    ; --- Format "a = x, b = y, result = z\n" into buffer ---
    push dword [result]
    push dword [b]
    push dword [a]
    push format_vals
    push buffer
    call sprintf
    add esp, 20

    ; --- Open file in append mode ---
    push 0                ; lpSecurityAttributes
    push 2                ; OPEN_ALWAYS
    push 0                ; No template
    push 0                ; No security attrs
    push 1                ; FILE_SHARE_WRITE
    push 0x40000000       ; GENERIC_WRITE
    push file_name
    call CreateFileA
    mov [hFile], eax

    cmp eax, -1
    je file_error         ; If handle is invalid, skip writing

    ; Move file pointer to end
    push 0
    push 0
    push 2                ; FILE_END
    push eax              ; hFile
    call SetFilePointer

    ; --- Calculate length of formatted buffer ---
    call strlen
    push eax              ; string length

    ; Write to file
    push 0
    push written
    push eax              ; length
    push buffer
    push dword [hFile]
    call WriteFile

    ; Close file
    push dword [hFile]
    call CloseHandle

    ; --- Exit ---
    push 0
    call ExitProcess

file_error:
    print_str msg_title
    push 0
    call ExitProcess

; --- strlen: returns length of null-terminated string in eax ---
strlen:
    push edi
    mov edi, buffer
    xor eax, eax
.str_loop:
    cmp byte [edi + eax], 0
    je .done
    inc eax
    jmp .str_loop
.done:
    pop edi
    ret
