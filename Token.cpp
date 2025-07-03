#include "Token.hpp"
#include "IdentifierInfo.hpp"

bool Token::isKeyword() const {
    return II && (II->isKeyword());
}