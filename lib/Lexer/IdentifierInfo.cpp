#include "Lexer/IdentifierInfo.hpp"
#include "Lexer/Token.hpp"
#include <ostream> //TODO: Remove, this is for pretty printing

TokenKind IdentifierInfo::getTokenID() const {
    //in real clang unsafe bit casting
    switch (KeywordId) {
    case TokenID::NotKeyword: return TokenKind::identifier;
    case TokenID::kw_int:     return TokenKind::kw_int;
    case TokenID::kw_double:  return TokenKind::kw_double;
    case TokenID::kw_if:     return TokenKind::kw_if;
    case TokenID::kw_else:  return TokenKind::kw_else;
    case TokenID::kw_return:     return TokenKind::kw_return;
    case TokenID::kw_typedef:  return TokenKind::kw_typedef;
        // ...
    default:         return TokenKind::identifier; // fallback
    }
}

void IdentifierInfo::setTokenID(TokenKind K) {
    //in real clang unsafe bit casting
    switch (K) {
    case TokenKind::kw_int:     
        KeywordId = TokenID::kw_int;
        break;
    case TokenKind::kw_double:  
        KeywordId = TokenID::kw_double;
        break;
    case TokenKind::kw_if:     
        KeywordId = TokenID::kw_if;
        break;
    case TokenKind::kw_else:  
        KeywordId = TokenID::kw_else;
        break;
    case TokenKind::kw_return:     
        KeywordId = TokenID::kw_return;
        break;
    case TokenKind::kw_typedef:  
        KeywordId = TokenID::kw_typedef;
        break;
        // ...
    default:  KeywordId = TokenID::NotKeyword; // fallback
    }
}

void IdentifierInfo::print(std::ostream& os) const {
    os << "IdentifierInfo(name=\"" << Name << "\"";
    if (AssociatedDecl) os << ", has Decl)";
    else os << ", no Decl)";
    os << (IsMacro ? ", isMacro" : "");
    os << (isKeyword() ? ", isKeyword" : "");
}