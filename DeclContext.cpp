#include "DeclContext.hpp"
#include "LookupResult.hpp"
#include "ASTDecl.hpp"
#include <cassert>

bool DeclContext::lookupDirectMember(IdentifierInfo* II, LookupResult& R) const {
    // TODO: Handle different declarations with the same name (overloads)
    assert("Called stub function");
    return false;
}

NamedDecl* DeclContext::lookupDirectMember(IdentifierInfo* II) const {
    if (!II) return nullptr;
    auto it = Members.find(II);
    if (it != Members.end()) {
        return it->second;
    }
    return nullptr;
}


// Add a declaration into this context
NamedDecl* DeclContext::addDecl(NamedDecl* NewDecl) {
    IdentifierInfo* Name = NewDecl->getIdentifier();
    NamedDecl* existing = nullptr;

    auto it = Members.find(Name);
    if (it != Members.end()) {
        existing = it->second; // Found an existing local declaration in *this* Scope
    }

    if (existing) {
        // Link the NewDecl to the existing chain.
        NewDecl->setPreviousDeclInContext(existing);
    }
    else {
        // NewDecl->PreviousDeclInContext is already nullptr.
    }
    Members[Name] = NewDecl; // NewDecl is now the new head for this Scope's list

    return existing; // Return the previous head if any, for Sema to use
}
