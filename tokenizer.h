#pragma once
#include <string>
#include <vector>

enum class TokenType {
    Identifier, Number,
    Plus, Minus, Star, Slash, Caret,
    Equals, LParen, RParen, Semicolon,
    End
};

struct Token {
    TokenType type;
    std::string text;
};

std::vector<Token> tokenize(const std::string& src);
