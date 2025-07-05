#include "Scope.hpp"
#include "Parser.hpp"
#include "ASTDecl.hpp"
#include <ranges>

Scope::lookup_result Scope::lookupLocal(IdentifierInfo* II) const {
    lookup_result found = {};
    for (Decl* D : decls())
    {
        if (auto* ND = dyn_cast<NamedDecl>(D))
        {
            if (ND->getIdentifier() == II)
            {
                found.push_back(ND);
            }
        }
    }
    return found;
}

NamedDecl* Scope::lookupFirstLocal(IdentifierInfo* II) const {
    for (Decl* D : decls())
    {
        if (auto* ND = dyn_cast<NamedDecl>(D))
        {
            if (ND->getIdentifier() == II)
            {
                return ND;
            }
        }
    }
    return nullptr;
}

bool Scope::addDecl(NamedDecl* NewDecl) {
    DeclsInScope.push_back(NewDecl); // Add to local map if no conflict
    return true;
}