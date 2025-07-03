#pragma once
#include <vector>
#include "Basic.hpp"

class Scope {
public:
    enum class ScopeFlags {
        DeclScope = 0x01,
        FunctionScope = 0x02,
        BlockScope = 0x04,
        // ... other flags
    };

private:
    Scope* Parent = nullptr;
    unsigned Flags = 0;
    std::vector<NamedDecl*> DeclsInScope = {};

public:
    Scope(Scope* parent, unsigned flags)
        : Parent(parent), Flags(flags) {}

    void AddDecl(NamedDecl* D) { DeclsInScope.push_back(D); }

    NamedDecl* lookup(IdentifierInfo* II) const;

    Scope* getParent() const { return Parent; }
};
