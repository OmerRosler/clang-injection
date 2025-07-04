#include "Parser.hpp"
#include "Sema.hpp"
#include "Preprocessor.hpp"
#include "ASTDecl.hpp"

void Parser::parseTopLevelDecl() {
    while (Tok.isNot(TokenKind::eof)) {
        parseDeclaration();
    }
}

Decl* Parser::parseDeclaration() {
    //dispatch all known declarations
    // all of these are complete declarations (for now) so they consume the semi-colon
    if (Tok.is(TokenKind::kw_typedef)) {
        return parseTypedef();
    }
    else
    {
        return parseVarDecl();
    }
}
















// Handles: 'typedef int Foo;'
TypedefDecl* Parser::parseTypedef() {
    ConsumeToken(); // eat 'typedef'

    Type* underlyingType = parseTypeName();
    if (!underlyingType) {
        throw std::runtime_error("Error: Expected type after typedef");
    }

    // Expect an identifier (the new typedef name)
    if (Tok.isNot(TokenKind::identifier)) {
        throw std::runtime_error("Error: Expected identifier after typedef type");
    }

    IdentifierInfo* II = Tok.getIdentifierInfo();

    ConsumeToken(); // consume the identifier

    // Expect a semicolon
    if (Tok.isNot(TokenKind::semicolon)) {
        throw std::runtime_error("Error: Expected ';' after typedef declaration");
    }
    ConsumeToken(); // consume ';'

    // Semantic action: create the TypedefDecl
    return Actions.ActOnTypedefDecl(CurrentScope, Actions.GetCurrentDeclContext(),
        underlyingType, II);
}

VarDecl* Parser::parseVarDecl() {
    Type* Ty = parseTypeName();
    if (!Ty) return nullptr;

    if (Tok.isNot(TokenKind::identifier)) {
        throw std::runtime_error("Expected identifier in variable declaration");
    }

    IdentifierInfo* VarName = Tok.getIdentifierInfo();
    ConsumeToken();

    Expr* Init;
    if (Tok.is(TokenKind::semicolon))
    {
        Init = nullptr;
    }
    else if (Tok.is(TokenKind::equal)) {
        //read initializer
        ConsumeToken();
        Init = parseExpr();
    }
    else {
        throw std::runtime_error("Expected '=' in variable declaration");
    }

    // Expect a semicolon
    if (Tok.isNot(TokenKind::semicolon)) {
        throw std::runtime_error("Error: Expected ';' after variable declaration");
    }
    ConsumeToken(); // consume ';'
    
    return Actions.ActOnVarDecl(CurrentScope, Actions.GetCurrentDeclContext(),
        Ty, VarName, Init);
}