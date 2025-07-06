#include "Parser/Parser.hpp"
#include "Sema/Sema.hpp"
Type* Parser::parseTypeName() {
    Type* Ty = nullptr;

    switch (Tok.getKind()) {
    case TokenKind::kw_int:
        Ty = Actions.ActOnBuiltinType(BuiltinType::Int);
        break;

    case TokenKind::kw_double:
        Ty = Actions.ActOnBuiltinType(BuiltinType::Double);
        break;

    case TokenKind::identifier: {
        IdentifierInfo* II = getIdentifier();
        Ty = Actions.ActOnIdentifierType(getCurrentScope(),
            Actions.GetCurrentDeclContext(), II); // could be a typedef, for example
        break;
    }

    default:
        // In Clang this would trigger a diagnostic
        return nullptr;
    }

    ConsumeToken(); // eat the token regardless
    return Ty;

}