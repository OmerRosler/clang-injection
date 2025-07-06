#pragma once
#include "AST/ASTType.hpp"
#include "AST/ASTExpr.hpp"
#include "Lexer/Token.hpp"


// === Parser ===
class Parser {
private:

    void ConsumeToken();
    void ExpectAndConsume(TokenKind Expected);

    IdentifierInfo* getIdentifier() const {
        return Tok.getIdentifierInfo(); // assumes Token has it
    }

    const char* getLiteralText() const {
        return Tok.getLiteralText(); // assumes Token has this
    }

    unsigned getLiteralLength() const {
        return Tok.getLength(); // used for parsing numbers
    }


    bool isDeclarationStatement()
    {
        //TODO: This is a hack! needs to change significantly
        if (Tok.isKeyword())
            return true;
        // won't work for A x = 3; where A is known typename
        return false;
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

    void parseTopLevelDecl();


    CompoundStmt* parseCompoundStatement();
    Stmt* parseStatement();
    Stmt* parseDeclarationOrStatement(Scope* S);

protected:
    Preprocessor& PP;
    Sema& Actions;
    Token Tok;

};
