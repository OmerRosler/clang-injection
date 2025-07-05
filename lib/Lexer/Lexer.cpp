#include "Lexer/Lexer.hpp"
#include "Preprocessor/Preprocessor.hpp"

void Lexer::SkipWhitespace() {
    while (BufferPtr < BufferEnd &&
        (*BufferPtr == ' ' ||
            *BufferPtr == '\t' ||
            *BufferPtr == '\n' ||
            *BufferPtr == '\r'))
        ++BufferPtr;
}

IdentifierTable& Lexer::getIdents()
{
    return PP.Idents;
}

const IdentifierTable& Lexer::getIdents() const
{
    return PP.Idents;
}

bool Lexer::LexPunctuation(char c, Token& Tok)
{
    bool is_punctuation = true;
    //if we add && or something, their case needs to update this
    unsigned int punc_length = 1;
    switch (c) {
    case '+':
        Tok.Kind = TokenKind::plus;
        break;
    case '*':
        Tok.Kind = TokenKind::star;
        break;
    case '=':
        Tok.Kind = TokenKind::equal;
        break;
    case ';':
        Tok.Kind = TokenKind::semicolon;
        break;
    case '(':
        Tok.Kind = TokenKind::l_paren;
        break;
    case ')':
        Tok.Kind = TokenKind::r_paren;
        break;
    case '{':
        Tok.Kind = TokenKind::l_brace;
        break;
    case '}':
        Tok.Kind = TokenKind::r_brace;
        break;
    default:
        Tok.Kind = TokenKind::unknown;
        is_punctuation = false;
    }
    Tok.textPtr = BufferPtr;
    Tok.length = punc_length;
    Tok.II = nullptr;
    return is_punctuation;
}
void Lexer::Lex(Token& Tok) {
    SkipWhitespace();

    //eof
    if (BufferPtr >= BufferEnd) {
        Tok.setKind(TokenKind::eof);
        Tok.textPtr = BufferPtr;
        Tok.length = 0;
        return;
    }
    char c = *BufferPtr;

    //identifiers, keywords
    if (isAlpha(c) || c == '_') {
        const char* start = BufferPtr;
        while (BufferPtr < BufferEnd && (isAlnum(*BufferPtr) || *BufferPtr == '_'))
            ++BufferPtr;

        size_t len = BufferPtr - start;
        std::string_view name(start, len);
        IdentifierInfo* II = getIdents().get(name);

        Tok.Kind = II->getTokenID();
        Tok.II = II;
        Tok.textPtr = start;
        Tok.length = unsigned(len);
        return;
    }

    //numeric values
    if (isDigit(c)) {
        const char* start = BufferPtr;
        bool hasDot = false;

        while (BufferPtr < BufferEnd && (isDigit(*BufferPtr) || *BufferPtr == '.')) {
            if (*BufferPtr == '.') {
                if (hasDot) break;
                hasDot = true;
            }
            ++BufferPtr;
        }

        size_t len = BufferPtr - start;
        Tok.Kind = TokenKind::numeric_constant;
        Tok.textPtr = start;
        Tok.length = unsigned(len);
        Tok.II = nullptr;
        return;
    }

    //punctuations
    (void)LexPunctuation(c, Tok); //HACK: the defaults match so we don't check the result

    ++BufferPtr;
}