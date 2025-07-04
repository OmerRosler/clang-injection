#include "DeclContext.hpp"
#include "LookupResult.hpp"
#include "ASTDecl.hpp"

void DeclContext::lookup(IdentifierInfo* II, LookupResult& R) const {
    if (!II) return;
    auto it = Lookup.find(II);
    if (it != Lookup.end()) {
        for (auto* D : it->second)
            R.addDecl(D);
    }
}

// Add a declaration into this context
void DeclContext::addDecl(NamedDecl* D) {
    if (!D) return;
    Decls.push_back(D);
    Lookup[D->getIdentifier()].push_back(D);
}