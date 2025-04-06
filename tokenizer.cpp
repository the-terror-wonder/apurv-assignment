#include "tokenizer.h"
#include <cctype>

std::vector<Token> tokenize(const std::string& src) {
    std::vector<Token> tokens;
    size_t i = 0;
    while (i < src.length()) {
        char c = src[i];
        if (isspace(c)) {
            ++i;
        } else if (isalpha(c)) {
            std::string id;
            while (isalnum(src[i])) id += src[i++];
            tokens.push_back({TokenType::Identifier, id});
        } else if (isdigit(c)) {
            std::string num;
            while (isdigit(src[i])) num += src[i++];
            tokens.push_back({TokenType::Number, num});
        } else {
            switch (c) {
                case '+': tokens.push_back({TokenType::Plus, "+"}); break;
                case '-': tokens.push_back({TokenType::Minus, "-"}); break;
                case '*': tokens.push_back({TokenType::Star, "*"}); break;
                case '/': tokens.push_back({TokenType::Slash, "/"}); break;
                case '^': tokens.push_back({TokenType::Caret, "^"}); break;
                case '=': tokens.push_back({TokenType::Equals, "="}); break;
                case '(': tokens.push_back({TokenType::LParen, "("}); break;
                case ')': tokens.push_back({TokenType::RParen, ")"}); break;
                case ';': tokens.push_back({TokenType::Semicolon, ";"}); break;
                default: break;
            }
            ++i;
        }
    }
    tokens.push_back({TokenType::End, ""});
    return tokens;
}
