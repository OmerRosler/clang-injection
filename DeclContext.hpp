#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Basic.hpp"

class DeclContext {
    using DeclList = std::vector<NamedDecl*>;
    using LookupMap = std::unordered_map<IdentifierInfo*, DeclList>;

    DeclList Decls;
    LookupMap Lookup;

public:
    DeclContext() = default;

    // Add a declaration into this context
    void addDecl(NamedDecl* D);

    void lookup(IdentifierInfo* II, LookupResult& R) const;

    // Iterator over all declarations in this context
    auto decls_begin() const { return Decls.begin(); }
    auto decls_end() const { return Decls.end(); }
    auto decls() const { return std::make_pair(decls_begin(), decls_end()); }
};
