#pragma once
#include <vector>
#include <unordered_map>

#include "Basic.hpp"
enum class ScopeFlags {
    DeclScope = 0x01,
    FunctionScope = 0x02,
    BlockScope = 0x04,
    // ... other flags
};
class Scope {
private:
    Scope* Parent;
    unsigned Flags;
    std::unordered_map<IdentifierInfo*, NamedDecl*> DeclsInScope;

public:
    Scope(Scope* parent, unsigned flags)
        : Parent(parent), Flags(flags) {}

    void AddDecl(IdentifierInfo* II, NamedDecl* D) { DeclsInScope[II] = D; }

    NamedDecl* lookup(IdentifierInfo* II) const;

    Scope* getParent() const { return Parent; }
};
