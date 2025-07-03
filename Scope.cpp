#include "Scope.hpp"
#include "Parser.hpp"
#include "ASTDecl.hpp"

NamedDecl* Scope::lookup(IdentifierInfo* II) const {
    for (auto* D : DeclsInScope) {
        if (D->getIdentifier() == II)
            return D;
    }
    return nullptr;
}