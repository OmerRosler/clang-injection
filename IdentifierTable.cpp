#include "IdentifierTable.hpp"

IdentifierTable::IdentifierTable() {
    // Insert all keywords here
    addKeyword("int", TokenKind::kw_int);
    addKeyword("double", TokenKind::kw_double);
    addKeyword("if", TokenKind::kw_if);
    addKeyword("else", TokenKind::kw_else);
    addKeyword("return", TokenKind::kw_return);
    addKeyword("typedef", TokenKind::kw_typedef);
    // ... and so on for all keywords you want to support
}
// Lookup or create IdentifierInfo for a given name
IdentifierInfo* IdentifierTable::get(std::string_view name) {
    auto it = Table.find(name);
    if (it != Table.end())
        return it->second.get();

    // Insert new IdentifierInfo for non-keywords
    auto II = std::make_unique<IdentifierInfo>(std::string(name));
    IdentifierInfo* ptr = II.get();
    Table.emplace(std::string(name), std::move(II));
    return ptr;
}

void IdentifierTable::addKeyword(const char* name, TokenKind kind) {
    auto II = std::make_unique<IdentifierInfo>(name);
    II->setTokenID(kind);
    Table[name] = std::move(II);

}