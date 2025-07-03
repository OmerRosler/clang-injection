#pragma once
#include "ASTType.hpp"
#include "ASTExpr.hpp"
#include "Token.hpp"


// === Parser ===
class Parser {
private:

    void ConsumeToken();

    IdentifierInfo* getIdentifier() const {
        return Tok.getIdentifierInfo(); // assumes Token has it
    }

    const char* getLiteralText() const {
        return Tok.getLiteralText(); // assumes Token has this
    }

    unsigned getLiteralLength() const {
        return Tok.getLength(); // used for parsing numbers
    }

public:
    Parser(Preprocessor& pp, Sema& s)
        : PP(pp), Actions(s) {
        ConsumeToken();
    }


    int getPrecedence() const {
        if (Tok.is(TokenKind::plus)) return 10;
        if (Tok.is(TokenKind::star)) return 20;
        return -1;
    }

    Expr* parseExpr();

    Expr* parsePrimaryExpr();
    Expr* parseLiteral();
    Expr* parseBinOpRHS(int exprPrec, Expr* lhs);
    ParenExpr* parseParenExpr();


    Type* parseTypeName();

    TypedefDecl* parseTypedef();
    VarDecl* parseVarDecl();
    Decl* parseDeclaration();

protected:
    Preprocessor& PP;
    Sema& Actions;
    Token Tok;
    Scope* CurrentScope;

};
