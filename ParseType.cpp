#include "Parser.hpp"
#include "Sema.hpp"
Type* Parser::parseType() {
    if (Tok.Kind == TokenKind::IntKeyword || Tok.Kind == TokenKind::DoubleKeyword) {
        std::string name = Tok.Text;
        ConsumeToken();
        return Actions.ActOnBuiltinType(name);
    }
    else if (Tok.Kind == TokenKind::Identifier)
    {
        std::string name = Tok.Text;
        ConsumeToken();
        return Actions.ActOnUnresolvedType(name);
    }
}