#pragma once
#include "Token.hpp"

// === Lexer ===
// Simplified internal Lexer, only exposes minimal interface internally
class Lexer {
    Preprocessor& PP;

    //original buffer
    const char* BufferStart;
    const char* BufferEnd;

    //current ptr
    const char* BufferPtr;

    //utility methods
    IdentifierTable& getIdents();
    const IdentifierTable& getIdents() const;

    void SkipWhitespace();
    bool LexPunctuation(char c, Token& Tok);

    static bool set_punctuation(char c, TokenKind& Result);
    static bool isAlpha(char c) { return std::isalpha(static_cast<unsigned char>(c)); }
    static bool isDigit(char c) { return std::isdigit(static_cast<unsigned char>(c)); }
    static bool isAlnum(char c) { return std::isalnum(static_cast<unsigned char>(c)); }

public:
    Lexer(Preprocessor& PP, const char* input, size_t length)
        : BufferStart(input), BufferPtr(input), BufferEnd(input + length), PP(PP) {}

    void Lex(Token& Result);
    
};