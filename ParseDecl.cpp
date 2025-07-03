#include "Parser.hpp"
#include "Sema.hpp"
#include "Preprocessor.hpp"

// Entry point
Decl* Parser::parseDeclaration() {
    //if (Tok.Kind == TokenKind::Identifier && Tok.Text == "typedef") {
    //    return ParseTypedef();
    //}

    //// Otherwise, parse type first
    //Type* Ty = Actions.getTypeName(*this);
    //if (!Ty) return nullptr;

    //return ParseSingleDeclAfterType(Ty);
    return nullptr;
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