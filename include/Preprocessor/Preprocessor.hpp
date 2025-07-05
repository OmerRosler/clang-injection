#pragma once
#include "Basic.hpp"
#include "Preprocessor/IdentifierTable.hpp"
#include "Lexer/Lexer.hpp"

class Preprocessor {
    friend class Lexer;
public:
    Preprocessor(const char* input, size_t length)
        : Idents(), TheLexer(std::make_unique<Lexer>(*this, input, length)) {}
    Preprocessor(std::string_view sv) : Preprocessor(sv.data(), sv.length()) {}

    void Lex(Token& Result) {
        TheLexer->Lex(Result);

        // Resolve identifiers to keywords
        if (Result.Kind == TokenKind::identifier && Result.II) {
            if (Result.II->isKeyword()) {
                Result.Kind = Result.II->getTokenID(); // Update kind to keyword
            }
        }
    }
private:
    IdentifierTable Idents;
    std::unique_ptr<Lexer> TheLexer;
};