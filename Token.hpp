#pragma once
#include <string>
#include "Basic.hpp"
// === Token ===
enum class TokenKind {
    // Primary tokens
    identifier,
    numeric_constant,

    //punctuations
    plus,
    star,
    equal,
    semicolon,
    l_paren,
    r_paren,
    l_curly,
    r_curly,

    //keywords
    kw_int,
    kw_double,
    kw_if,
    kw_else,
    kw_return,
    kw_typedef,

    //text handling
    eof,

    // Annotation tokens used internally by the parser
    annot_typename,
    annot_primary_expr,
    annot_cxxscope,

    unknown
};
struct Token {
public:
    friend class Lexer;
    friend class Preprocessor;
private:
    TokenKind Kind = TokenKind::unknown;

    // === Clang-style semantic fields ===
    IdentifierInfo* II = nullptr;     // For identifier tokens
    const char* textPtr = nullptr; // For numeric/string literals. This is a pointer into the direct input to the lexer
    unsigned length = 0;

    void* AnnotationValue = nullptr;  // For annot_* tokens

public:
    Token() = default;

    void setKind(TokenKind K) { Kind = K; }
    TokenKind getKind() const { return Kind; }

    void setIdentifierInfo(IdentifierInfo* I) { II = I; }
    IdentifierInfo* getIdentifierInfo() const { return II; }

    void setLiteralData(const char* Data, unsigned Len) {
        textPtr = Data;
        length = Len;
    }
    const char * getLiteralText() const {
        return textPtr;
    }
    unsigned getLength() const {
        return length;
    }

    void setAnnotationValue(void* V) { AnnotationValue = V; }
    void* getAnnotationValue() const { return AnnotationValue; }

    bool is(TokenKind K) const { return Kind == K; }
    bool isNot(TokenKind K) const { return !is(K); }
    bool isAnnotation() const { return Kind >= TokenKind::annot_typename && Kind <= TokenKind::annot_cxxscope; }
};
