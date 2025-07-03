#include "Parser.hpp"
#include "Sema.hpp"

Expr* Parser::ParseLiteral() {
    if (Tok.isNot(TokenKind::numeric_constant)) {
        // In real clang, error handling here
        return nullptr;
    }

    const char* text = getLiteralText();
    unsigned len = getLiteralLength();

    Expr* result = nullptr;

    // Crude parsing logic just for demo
    if (std::strchr(text, '.')) {
        double value = std::strtod(text, nullptr);
        result = Actions.ActOnDoubleLiteral(value);
    }
    else {
        int value = std::strtol(text, nullptr, 10);
        result = Actions.ActOnIntLiteral(value);
    }

    ConsumeToken();
    return result;
}

Expr* Parser::parsePrimaryExpr() {
    if (Tok.is(TokenKind::numeric_constant)) {
        return ParseLiteral();
    }
    else if (Tok.is(TokenKind::identifier)) {
        IdentifierInfo* II = Tok.getIdentifierInfo();
        ConsumeToken();
        return Actions.ActOnIdentifier(II);
    }
    else if (Tok.is(TokenKind::l_paran))
    {
        ConsumeToken(); // eat '('
        Expr* SubExpr = parseExpr();
        if (Tok.isNot(TokenKind::r_paran))
            throw std::runtime_error("Expected ')'");
        ConsumeToken(); // eat ')'
        return SubExpr;
    }
    else {
        throw std::runtime_error("Unexpected token in primary expression");
    }
}

Expr* Parser::parseBinOpRHS(int exprPrec, Expr* lhs) {
    while (true) {
        int tokPrec = getPrecedence();
        if (tokPrec < exprPrec)
            return lhs;

        Token op = Tok;
        ConsumeToken(); //op is + or *

        Expr* rhs = parsePrimaryExpr();
        if (!rhs) return nullptr;

        int nextPrec = getPrecedence();
        if (tokPrec < nextPrec) {
            rhs = parseBinOpRHS(tokPrec + 1, rhs);
            if (!rhs) return nullptr;
        }

        if (op.is(TokenKind::plus))
            lhs = Actions.ActOnPlusExpr(lhs, rhs);
        else if (op.is(TokenKind::star))
            lhs = Actions.ActOnMultiplyExpr(lhs, rhs);
        else
            throw std::runtime_error("Unknown binary operator");
    }
}

Expr* Parser::parseExpr() {
    Expr* lhs = parsePrimaryExpr();
    if (!lhs) return nullptr;
    return parseBinOpRHS(0, lhs);
}