#pragma once
#include "Basic.hpp"
#include "IdentifierTable.hpp"
#include "Lexer.hpp"

struct Preprocessor {

public:
    Preprocessor(std::unique_ptr<Lexer> lexer, IdentifierTable& tbl)
        : TheLexer(std::move(lexer)), Idents(tbl) {}

    void Lex(Token& Tok) {
        TheLexer->Lex(Tok);
        if (Tok.Kind == TokenKind::Identifier) {
            IdentifierInfo* II = Idents.get(Tok.Text);
            // Update token kind based on keyword info
            if (II->isKeyword()) {
                if (II->getName() == "int")
                    Tok.Kind = TokenKind::IntKeyword;
                else if (II->getName() == "double")
                    Tok.Kind = TokenKind::DoubleKeyword;
                // Add more keywords as needed
            }
        }
    }
private:
    std::unique_ptr<Lexer> TheLexer;
    IdentifierTable& Idents;
};