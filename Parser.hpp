#pragma once
#include "ASTType.hpp"
#include "ASTExpr.hpp"
#include "Token.hpp"


// === Parser ===
class Parser {
private:

    void ConsumeToken();

public:
    Parser(Preprocessor& pp, Sema& s)
        : PP(pp), Actions(s) {
        ConsumeToken();
    }


    int getPrecedence() const {
        if (Tok.is(TokenKind::Plus)) return 10;
        if (Tok.is(TokenKind::Star)) return 20;
        return -1;
    }


    Expr* parsePrimaryExpr();
    Expr* ParseLiteral();

    Expr* parseExpr();

    Expr* parseBinOpRHS(int exprPrec, Expr* lhs);

    Type* parseType();

    Type* ParseTypeSpecifier();

    Decl* ParseDeclaration();

protected:
    Preprocessor& PP;
    Sema& Actions;
    Token Tok;

};
