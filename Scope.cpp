#include "Scope.hpp"
#include "Parser.hpp"
#include "ASTDecl.hpp"
#include <ranges>

Decl* Scope::lookupLocal(IdentifierInfo* II) const {
    for (Decl* D : decls())
    {
        if (auto* ND = dyn_cast<NamedDecl>(D))
        {
            if (ND->getIdentifier() == II)
            {
                return D;
            }
        }
    }
    return nullptr;
}