#pragma once
#include "Basic.hpp"
#include "IdentifierTable.hpp"
#include "Lexer.hpp"

class Preprocessor {
    std::unique_ptr<Lexer> TheLexer;
    IdentifierTable& Idents;

public:
    Preprocessor(std::unique_ptr<Lexer> lexer, IdentifierTable& tbl)
        : TheLexer(std::move(lexer)), Idents(tbl) {}

    void Lex(Token& Tok) {
        TheLexer->Lex(Tok);
        if (Tok.Kind == TokenKind::Identifier) {
            Tok.Kind = Idents.get(Tok.Text);
        }
    }
};