#include "Lexer/Token.hpp"
#include "Lexer/IdentifierInfo.hpp"

bool Token::isKeyword() const {
    return II && (II->isKeyword());
}