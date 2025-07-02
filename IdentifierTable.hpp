#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Basic.hpp"
#include "Token.hpp"
// == = IdentifierInfo == =
class IdentifierInfo {
    std::string Name;
    bool IsKeyword = false;

public:
    explicit IdentifierInfo(std::string name) : Name(std::move(name)) {}

    const std::string& getName() const { return Name; }
    bool isKeyword() const { return IsKeyword; }
    void setKeyword(bool val) { IsKeyword = val; }
};

// === IdentifierTable ===
class IdentifierTable {
    // Owns all IdentifierInfo instances
    std::unordered_map<std::string, std::unique_ptr<IdentifierInfo>> Table;

public:
    IdentifierTable() {
        // Preload keywords
        auto intId = std::make_unique<IdentifierInfo>("int");
        intId->setKeyword(true);
        Table["int"] = std::move(intId);

        auto doubleId = std::make_unique<IdentifierInfo>("double");
        doubleId->setKeyword(true);
        Table["double"] = std::move(doubleId);
    }

    // Returns existing or creates new IdentifierInfo for Name
    IdentifierInfo* get(const std::string& Name) {
        auto it = Table.find(Name);
        if (it != Table.end())
            return it->second.get();

        // Not found, create new IdentifierInfo (not keyword)
        auto II = std::make_unique<IdentifierInfo>(Name);
        IdentifierInfo* ptr = II.get();
        Table[Name] = std::move(II);
        return ptr;
    }
};
