#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Basic.hpp"
#include "Token.hpp"

// === IdentifierTable ===
// Simplified, just maps strings to TokenKinds (keywords vs identifiers)
class IdentifierTable {
    std::unordered_map<std::string, TokenKind> Table;

public:
    IdentifierTable() {
        Table["int"] = TokenKind::IntKeyword;
        Table["double"] = TokenKind::DoubleKeyword;
    }

    TokenKind get(const std::string& name) const {
        auto it = Table.find(name);
        if (it != Table.end())
            return it->second;
        return TokenKind::Identifier;
    }
};


// === IdentifierInfo holds info about keywords and identifiers ===
struct IdentifierInfo {
    std::string name;
    bool isKeywordFlag;

    IdentifierInfo(std::string n, bool kw = false) : name(std::move(n)), isKeywordFlag(kw) {}

    bool isKeyword() const { return isKeywordFlag; }
};