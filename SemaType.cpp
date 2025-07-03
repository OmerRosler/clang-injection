#include "Sema.hpp"
#include "LookupResult.hpp"

bool Sema::isTypeName(IdentifierInfo* II, Scope* S) {
    LookupResult R(*this, II, LookupNameKind::LookupOrdinaryName);
    LookupName(R, S);
    if (!R.empty()) {
        if (auto* TD = dyn_cast<TypedefDecl>(R.getFoundDecl()))
            return true;
    }
    return false;
}