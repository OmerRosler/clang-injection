#pragma once
#include <string>
#include "Basic.hpp"
// === Token ===
enum TokenKind {
    Identifier,
    IntKeyword,
    DoubleKeyword,
    Plus,
    Star,
    IntLiteral,
    DoubleLiteral,
    EndOfFile
};

struct Token {

    TokenKind Kind;
    std::string Text;

    // Clang uses isOneOf and other helpers
    bool is(TokenKind k) const { return Kind == k; }
    bool isOneOf(TokenKind k1, TokenKind k2) const { return Kind == k1 || Kind == k2; }
};
