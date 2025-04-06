#include "asmgen.h"

std::string generateAssembly(const Expr& expr) {
    int tempCount = 0;
    std::string body = expr.generateAsm(tempCount);

    return
        "section .text\n"
        "    global _start\n\n"
        "_start:\n" +
        body +
        "    mov ebx, eax\n"
        "    mov eax, 1\n"
        "    int 0x80\n";
}