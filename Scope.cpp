#include "Scope.hpp"
#include "Parser.hpp"
#include "ASTDecl.hpp"
#include <ranges>
#include <cassert>

NamedDecl* Scope::lookupLocal(IdentifierInfo* II) const {
    auto it = Locals.find(II);
    if (it != Locals.end())
    {
        return it->second;
    }
    return nullptr;
}

NamedDecl* Scope::addDecl(NamedDecl* NewDecl) {
    IdentifierInfo* Name = NewDecl->getIdentifier();
    NamedDecl* existing = nullptr;

    auto it = Locals.find(Name);
    if (it != Locals.end()) {
        existing = it->second; // Found an existing local declaration in *this* Scope
    }

    if (existing) {
        // Link the NewDecl to the existing chain.
        NewDecl->setPreviousDeclInContext(existing);
    }
    else {
        // NewDecl->PreviousDeclInContext is already nullptr.
    }
    Locals[Name] = NewDecl; // NewDecl is now the new head for this Scope's list

    return existing; // Return the previous head if any, for Sema to use
}
