#pragma once
#include <string>
#include <memory>
#include <vector>

enum class NodeType { Assignment, BinaryOp, Number, Variable };

struct ASTNode {
    NodeType type;
    virtual ~ASTNode() = default;
};

struct NumberNode : ASTNode {
    int value;
    NumberNode(int v) : value(v) { type = NodeType::Number; }
};

struct VariableNode : ASTNode {
    std::string name;
    VariableNode(std::string n) : name(std::move(n)) { type = NodeType::Variable; }
};

struct BinaryOpNode : ASTNode {
    char op;
    std::shared_ptr<ASTNode> left, right;
    BinaryOpNode(char o, std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r)
        : op(o), left(std::move(l)), right(std::move(r)) { type = NodeType::BinaryOp; }
};

struct AssignmentNode : ASTNode {
    std::string name;
    std::shared_ptr<ASTNode> expr;
    AssignmentNode(std::string n, std::shared_ptr<ASTNode> e)
        : name(std::move(n)), expr(std::move(e)) { type = NodeType::Assignment; }
};

using AST = std::vector<std::shared_ptr<ASTNode>>;
