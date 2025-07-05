#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Basic.hpp"

class DeclContext {
    using LookupMap = std::unordered_map<IdentifierInfo*, NamedDecl*>;

    //Maps identifier string to the* most recent* (head)NamedDecl in its redeclaration chain.
    LookupMap Members;
    DeclContext* ParentDC = nullptr; // Semantic parent

public:
    DeclContext() = default;

    // Add a declaration into this context
    NamedDecl* addDecl(NamedDecl* NewDecl);

    bool lookupDirectMember(IdentifierInfo* II, LookupResult& R) const;
    NamedDecl* lookupDirectMember(IdentifierInfo* II) const;

    const LookupMap& decls() const { return Members; }
};
