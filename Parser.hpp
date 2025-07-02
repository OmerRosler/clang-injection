#pragma once
#include "ASTType.hpp"
#include "ASTExpr.hpp"


// === Parser ===
class Parser {
private:

    // Helper: peek at current token without consuming
    std::string peekToken() const {
        if (index >= tokens.size()) return "";
        return tokens[index];
    }

    // Helper: consume and return current token
    std::string consumeToken() {
        if (index >= tokens.size()) throw std::runtime_error("Unexpected EOF");
        return tokens[index++];
    }

    // Check if token is binary operator (extend as needed)
    bool isBinaryOperator(const std::string& tok) const {
        return tok == "+";
    }

public:
    Parser(std::vector<std::string> toks, Sema& s)
        : tokens(std::move(toks)), sema(s) {}


    int precedence(const std::string& op) const {
        if (op == "*") return 20;
        if (op == "+") return 10;
        return -1;
    }

    Expr* parsePrimary();

    Expr* parseExpr(int minPrec = 0);

    Type* parseType();

protected:
    Sema& sema;

    std::vector<std::string> tokens;
    size_t index = 0;
};
