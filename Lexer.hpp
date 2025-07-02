#pragma once
#include <vector>
#include "Token.hpp"

// === Lexer ===
// Simplified internal Lexer, only exposes minimal interface internally
class Lexer {
    std::vector<Token> Tokens;
    size_t CurIdx = 0;

public:
    explicit Lexer(const std::vector<Token>& tokens) : Tokens(tokens) {}

    void Lex(Token& result) {
        result = (CurIdx < Tokens.size()) ? Tokens[CurIdx] : eofToken;
        if (CurIdx < Tokens.size()) ++CurIdx;
    }

private:
    static Token eofToken;
};