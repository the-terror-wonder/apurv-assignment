#include "asmgen.h"
#include <iostream>
#include <unordered_map>

static std::unordered_map<std::string, int> variables;

static void generateExpr(std::shared_ptr<ASTNode> node) {
    if (node->type == NodeType::Number) {
        auto n = static_cast<NumberNode*>(node.get());
        std::cout << "  PUSH " << n->value << "\n";
    } else if (node->type == NodeType::Variable) {
        auto v = static_cast<VariableNode*>(node.get());
        std::cout << "  LOAD " << v->name << "\n";
    } else if (node->type == NodeType::BinaryOp) {
        auto b = static_cast<BinaryOpNode*>(node.get());
        generateExpr(b->left);
        generateExpr(b->right);
        switch (b->op) {
            case '+': std::cout << "  ADD\n"; break;
            case '-': std::cout << "  SUB\n"; break;
            case '*': std::cout << "  MUL\n"; break;
            case '/': std::cout << "  DIV\n"; break;
            case '^': std::cout << "  POW\n"; break;
        }
    }
}

void generateAssembly(const AST& ast) {
    for (auto& stmt : ast) {
        if (stmt->type == NodeType::Assignment) {
            auto assign = static_cast<AssignmentNode*>(stmt.get());
            generateExpr(assign->expr);
            std::cout << "  STORE " << assign->name << "\n";
        }
    }
}
