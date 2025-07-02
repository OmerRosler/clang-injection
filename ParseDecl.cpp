#include "Parser.hpp"
#include "Sema.hpp"
#include "Preprocessor.hpp"

Type* Parser::ParseTypeSpecifier() {
    if (Tok.Kind == TokenKind::Identifier ||
        Tok.Kind == TokenKind::IntKeyword ||
        Tok.Kind == TokenKind::DoubleKeyword) {
        std::string name = Tok.Text;
        ConsumeToken();
        return Actions.ActOnTypeName(name);
    }
    else {
        throw std::runtime_error("Expected a type specifier");
    }
}

Decl* Parser::ParseDeclaration() {
    // Parse type
    Type* type = ParseTypeSpecifier();

    // Expect identifier
    if (Tok.Kind != TokenKind::Identifier)
        throw std::runtime_error("Expected identifier after type");

    IdentifierInfo* id = Actions.getIdentifierInfo(Tok.Text);
    ConsumeToken();

    // typedef or var?
    if (type && type->typeClass == Type::TypeClass::Builtin &&
        static_cast<BuiltinType*>(type)->kind == BuiltinType::BuiltinKind::Int &&
        id->getName() == "typedef") {
        throw std::runtime_error("We don't parse 'typedef int typedef;");
    }

    // Optional '=' for initialization or typedef
    if (Tok.Kind == TokenKind::EndOfFile || Tok.Text == ";") {
        return Actions.ActOnVarDecl(type, id);  // no initializer
    }

    if (Tok.Kind == TokenKind::Plus || Tok.Kind == TokenKind::Star ||
        Tok.Kind == TokenKind::IntKeyword || Tok.Kind == TokenKind::DoubleKeyword ||
        Tok.Kind == TokenKind::Identifier) {
        Expr* init = parseExpr();
        return Actions.ActOnVarDecl(type, id, init);
    }

    throw std::runtime_error("Unexpected token in declaration");
}