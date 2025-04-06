@echo off
nasm -f win32 program.asm -o program.obj
GoLink /console program.obj user32.dll kernel32.dll msvcrt.dll
pause
