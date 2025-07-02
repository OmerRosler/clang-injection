#include "Parser.hpp"
#include "Preprocessor.hpp"

void Parser::ConsumeToken() { PP.Lex(Tok); }