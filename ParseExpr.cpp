#include "Parser.hpp"
#include "Sema.hpp"

Expr* Parser::ParseLiteral() {
    std::string text = Tok.Text;
    TokenKind kind = Tok.Kind;
    ConsumeToken();

    if (kind == TokenKind::IntKeyword) {
        int value = std::stoi(text); // simulate clang's token-to-int conversion
        return Actions.ActOnIntLiteral(value);
    }
    else if (kind == TokenKind::DoubleKeyword) {
        double value = std::stod(text);
        return Actions.ActOnDoubleLiteral(value);
    }
    else {
        throw std::runtime_error("Unsupported literal");
    }
}

Expr* Parser::parsePrimaryExpr() {
    if (Tok.Kind == TokenKind::IntLiteral) {
        int val = std::stoi(Tok.Text);
        ConsumeToken();
        return Actions.ActOnIntLiteral(val);
    }
    else if (Tok.Kind == TokenKind::DoubleLiteral) {
        double val = std::stod(Tok.Text);
        ConsumeToken();
        return Actions.ActOnDoubleLiteral(val);
    }
    else if (Tok.Kind == TokenKind::Identifier) {
        std::string name = Tok.Text;
        ConsumeToken();
        return Actions.ActOnIdentifier(name);
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
        ConsumeToken();

        Expr* rhs = parsePrimaryExpr();
        if (!rhs) return nullptr;

        int nextPrec = getPrecedence();
        if (tokPrec < nextPrec) {
            rhs = parseBinOpRHS(tokPrec + 1, rhs);
            if (!rhs) return nullptr;
        }

        if (op.is(TokenKind::Plus))
            lhs = Actions.ActOnPlusExpr(lhs, rhs);
        else if (op.is(TokenKind::Star))
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