#include "Sema/Sema.hpp"
#include "Sema/LookupResult.hpp"
#include "AST/ASTDecl.hpp"

bool Sema::isTypeName(IdentifierInfo* II, Scope* S) {
    LookupResult R(*this, II, LookupNameKind::LookupOrdinaryName);
    LookupName(R, S);
    if (!R.empty()) {
        if (auto* TD = dyn_cast<TypedefDecl>(R.getFoundDecl()))
            return true;
    }
    return false;
}


Type* Sema::ActOnIdentifierType(Scope* S, DeclContext* OwnerContext, IdentifierInfo* II) {
    // Lookup in Scope
    LookupResult Res(*this, II);
    bool found = LookupName(Res, S);
    auto* ND = Res.getFoundDecl();
    if (found) {
        // For simplicity, assume it's always a TypedefDecl
        if (auto* TD = dyn_cast<TypedefDecl>(ND)) {
            return Context.getTypeDeclType(TD); // returns TypedefType*
        }
    }

    return Context.create<UnresolvedType>(II, OwnerContext);
}
