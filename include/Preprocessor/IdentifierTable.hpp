#pragma once
#include <string>
#include <string_view>
#include <map>
#include <memory>
#include <vector>
#include "Basic.hpp"
#include "Lexer/Token.hpp"
#include "Lexer/IdentifierInfo.hpp"
#include "Preprocessor/IdentifierTable.hpp"


// Simplified IdentifierTable class
class IdentifierTable {
    std::map<std::string, std::unique_ptr<IdentifierInfo>, std::less<>> Table;

public:
    IdentifierTable();

    IdentifierInfo* get(std::string_view name);

private:
    void addKeyword(const char* name, TokenKind kind);
};