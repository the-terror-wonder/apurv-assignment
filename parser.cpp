#include "parser.h"
#include <stdexcept>

static std::vector<Token> toks;
static size_t pos = 0;

static Token peek() { return toks[pos]; }
static Token advance() { return toks[pos++]; }

static std::shared_ptr<ASTNode> parseExpr();

static std::shared_ptr<ASTNode> parsePrimary() {
    auto t = advance();
    if (t.type == TokenType::Number)
        return std::make_shared<NumberNode>(std::stoi(t.text));
    if (t.type == TokenType::Identifier)
        return std::make_shared<VariableNode>(t.text);
    if (t.type == TokenType::LParen) {
        auto expr = parseExpr();
        advance(); // consume RParen
        return expr;
    }
    throw std::runtime_error("Unexpected token: " + t.text);
}

static int precedence(char op) {
    switch (op) {
        case '^': return 3;
        case '*': case '/': return 2;
        case '+': case '-': return 1;
        default: return 0;
    }
}

static std::shared_ptr<ASTNode> parseBinaryOpRHS(int minPrec, std::shared_ptr<ASTNode> lhs) {
    while (true) {
        auto t = peek();
        char op = t.text[0];
        int prec = precedence(op);

        if (prec < minPrec) return lhs;

        advance();
        auto rhs = parsePrimary();

        auto next = peek();
        int nextPrec = precedence(next.text[0]);
        if (prec < nextPrec) {
            rhs = parseBinaryOpRHS(prec + 1, rhs);
        }

        lhs = std::make_shared<BinaryOpNode>(op, lhs, rhs);
    }
}

static std::shared_ptr<ASTNode> parseExpr() {
    auto lhs = parsePrimary();
    return parseBinaryOpRHS(0, lhs);
}

static std::shared_ptr<ASTNode> parseStmt() {
    auto id = advance();
    advance(); // '='
    auto expr = parseExpr();
    advance(); // ';'
    return std::make_shared<AssignmentNode>(id.text, expr);
}

AST parse(const std::vector<Token>& tokens) {
    toks = tokens;
    pos = 0;
    AST program;
    while (peek().type != TokenType::End) {
        program.push_back(parseStmt());
    }
    return program;
}
