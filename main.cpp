#include "tokenizer.h"
#include "parser.h"
#include "asmgen.h"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: sage <file.sage>\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return 1;
    }

    std::string source((std::istreambuf_iterator<char>(input)),
                        std::istreambuf_iterator<char>());

    auto tokens = tokenize(source);
    auto ast = parse(tokens);
    generateAssembly(ast);

    return 0;
}
