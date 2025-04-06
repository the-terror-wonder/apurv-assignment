#pragma once
#include "ast.h"
#include "tokenizer.h"

AST parse(const std::vector<Token>& tokens);
