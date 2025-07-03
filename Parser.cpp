#include "Parser.hpp"
#include "Preprocessor.hpp"

void Parser::ConsumeToken() { PP.Lex(Tok); }

void Parser::ExpectAndConsume(TokenKind Expected) {
    if (Tok.is(Expected))
        ConsumeToken();
    else
        throw std::runtime_error("Expected different token");
}