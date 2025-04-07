import sys
import os
program_filepath = sys.argv[1]

print("[CMD] Parsing")

# Tokenize program
program_lines = []
with open(program_filepath, 'r') as program_file:
    program_lines = [line.strip() for line in program_file.readlines()]

program = []
for line in program_lines:
    parts = line.split(" ")
    opcode = parts[0]

    if opcode == "":
        continue

    program.append(opcode)

    if opcode == "PUSH":
        number = int(parts[1])
        program.append(number)
    elif opcode == "PRINT":
        string_literal = ' '.join(parts[1:])[1:-1]
        program.append(string_literal)
    elif opcode in ("JUMP.EQ.0", "JUMP.GT.0"):
        label = parts[1]
        program.append(label)

# Bookkeep string literals
string_literals = []
for ip in range(len(program)):
    if program[ip] == "PRINT":
        string_literal = program[ip+1]
        program[ip+1] = len(string_literals)
        string_literals.append(string_literal)

# Compile to assembly
asm_filepath = program_filepath[:-4] + ".asm"
obj_filepath = program_filepath[:-4] + ".obj"
exe_filepath = program_filepath[:-4] + ".exe"
out = open(asm_filepath, 'w')

out.write("""; -- header --
bits 64
default rel
""")

out.write("""; -- variables --
section .bss
read_number resq 1 ; 64-bits integer = 8 bytes 
""")

out.write("""; -- constants --
section .data
read_format db "%lld", 0 ; format for scanf (64-bit signed integer)
print_format db "%lld", 10, 0 ; format for printf (64-bit signed integer + newline)
""")
for i, string_literal in enumerate(string_literals):
    out.write(f"string_literal_{i} db \"{string_literal}\", 0\n")

out.write("""; -- Entry Point --
section .text
global main
extern printf
extern scanf

main:
	PUSH rbp
	MOV rbp, rsp
	SUB rsp, 32
""")

ip = 0
while ip < len(program):
    opcode = program[ip]
    ip += 1

    if opcode.endswith(":"):
        out.write(f"; -- Label --\n{opcode}\n")
    elif opcode == "PUSH":
        number = program[ip]
        ip += 1
        out.write(f"; -- PUSH --\n\tPUSH {number}\n")
    elif opcode == "POP":
        out.write(f"; -- POP --\n\tPOP rax\n")
    elif opcode == "ADD":
        out.write("""; -- ADD --
	POP rbx
	POP rax
	ADD rax, rbx
	PUSH rax
""")
    elif opcode == "SUB":
        out.write("""; -- SUB --
	POP rbx
	POP rax
	SUB rax, rbx
	PUSH rax
""")
    elif opcode == "MOD":
        out.write("""; -- MOD --
	POP rbx
	POP rax
	XOR rdx, rdx
	DIV rbx
	PUSH rdx
""")
    elif opcode == "PRINT":
        string_literal_index = program[ip]
        ip += 1
        out.write(f"; -- PRINT --\n\tLEA rcx, string_literal_{string_literal_index}\n\tXOR eax, eax\n\tCALL printf\n")
    elif opcode == "PRINT.NUM":
        out.write("""; -- PRINT.NUM --
    POP rax
	MOV rdx, rax
	LEA rcx, print_format
	XOR rax, rax
	CALL printf
""")
    elif opcode == "READ":
        out.write("""; -- READ --
	LEA rcx, read_format
	LEA rdx, read_number
	XOR eax, eax
	CALL scanf
	PUSH qword [read_number]
""")
    elif opcode == "JUMP.EQ.0":
        label = program[ip]
        ip += 1
        out.write(f"; -- JUMP.EQ.0 --\n\tCMP qword [rsp], 0\n\tJE {label}\n")
    elif opcode == "JUMP.GT.0":
        label = program[ip]
        ip += 1
        out.write(f"; -- JUMP.GT.0 --\n\tCMP qword [rsp], 0\n\tJG {label}\n")
    elif opcode == "HALT":
        out.write("; -- HALT --\n\tJMP EXIT_LABEL\n")

out.write("""EXIT_LABEL:
	MOV rsp, rbp
	POP rbp
	MOV rax, 0
	RET
""")

out.close()

print("[CMD] Assembling")
os.system(f"nasm -f elf64 -o {obj_filepath} {asm_filepath}")
print("[CMD] Linking")
os.system(f"gcc -nostartfiles -o {exe_filepath} {obj_filepath} -lkernel32 -lmsvcrt")
print("[CMD] Running")
os.system(f"./{exe_filepath}")
