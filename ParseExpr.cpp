#include "Parser.hpp"
#include "Sema.hpp"

Expr* Parser::parsePrimary() {
    if (index >= tokens.size()) throw std::runtime_error("Unexpected EOF");
    const std::string& tok = tokens[index++];

    if (tok == "int") {
        return sema.ActOnIntLiteral(42);
    }
    else if (tok == "double") {
        return sema.ActOnDoubleLiteral(3.14);
    }
    else {
        return sema.actOnUnknownName(tok);
    }
}

Expr* Parser::parseExpr(int minPrec) {
    Expr* lhs = parsePrimary();

    while (index < tokens.size()) {
        const std::string& op = tokens[index];
        int prec = precedence(op);
        if (prec < minPrec) break;

        ++index; // consume operator
        Expr* rhs = parseExpr(prec + 1);

        if (op == "+") {
            lhs = sema.actOnPlusExpr(lhs, rhs);
        }
        else if (op == "*") {
            lhs = sema.actOnMultiplyExpr(lhs, rhs);
        }
        else {
            throw std::runtime_error("Unknown binary operator: " + op);
        }
    }

    return lhs;
}