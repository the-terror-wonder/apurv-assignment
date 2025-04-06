%macro print_str 1
    push %1
    call printf
    add esp, 4
%endmacro

%macro read_int 1
    push %1
    push input_fmt
    call scanf
    add esp, 8
%endmacro

%macro print_int 1
    push dword %1        ; << FIXED: specify dword size
    push output_fmt
    call printf
    add esp, 8
%endmacro

%macro msgbox_int 1
    push dword %1        ; << FIXED: specify dword size
    push sprintf_fmt
    push msg_text
    call sprintf
    add esp, 12

    push 0
    push msg_title
    push msg_text
    push 0
    call MessageBoxA
%endmacro
