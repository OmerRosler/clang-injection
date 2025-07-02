#include "Parser.hpp"
#include "Sema.hpp"
Type* Parser::parseType() {
    // For simplicity, assume a single token type name (identifier or builtin keyword)
    if (Tok.Kind == TokenKind::Identifier ||
        Tok.Kind == TokenKind::IntKeyword ||
        Tok.Kind == TokenKind::DoubleKeyword)
    {
        std::string name = Tok.Text;
        ConsumeToken();  // consume the token

        // Delegate to Sema for semantic resolution and node creation
        return Actions.ActOnTypeName(name);
    }
    else {
        throw std::runtime_error("Expected type name");
    }
}