#pragma once
#include <string>
#include "Basic.hpp"
enum class TokenKind;
enum class TokenID : unsigned char {
    NotKeyword = 0,
    kw_int = 1,
    kw_double = 2,
    kw_if = 3,
    kw_else = 4,
    kw_return = 5,
    kw_typedef = 6
    // ... other keywords
};

class IdentifierInfo {
    std::string Name;            // The textual name of the identifier
    Decl* AssociatedDecl = nullptr; // Pointer to semantic declaration, initially null

    // Stub for macro-related fields (not implemented)
    bool IsMacro = false;

    TokenID KeywordId = TokenID::NotKeyword;

public:
    explicit IdentifierInfo(std::string name) : Name(std::move(name)) {}

    // Access the name string
    const std::string& getName() const { return Name; }

    // Semantic linkage: get/set the Decl pointer
    Decl* getDecl() const { return AssociatedDecl; }
    void setDecl(Decl* D) { AssociatedDecl = D; }

    // Macro-related stubs
    bool isMacro() const { return IsMacro; }
    void setMacro(bool val) { IsMacro = val; }

    // Keyword handling
    bool isKeyword() const { return KeywordId != TokenID::NotKeyword; }

    void setTokenID(TokenKind K);
    TokenKind getTokenID() const;

    // Debug print
    void print(std::ostream& os) const;
};