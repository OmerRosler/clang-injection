#include "Sema.hpp"
#include "LookupResult.hpp"
#include "Scope.hpp"

// === Main public API ===
bool Sema::LookupName(LookupResult& R, Scope* S, bool AllowBuiltinCreation) {
    if (R.getLookupKind() == LookupNameKind::LookupADL)
        return LookupADL(R, S);

    // Try scope-based lookup
    for (; S; S = S->getParent()) {
        NamedDecl* D = S->lookup(R.getLookupName());
        if (D) {
            R.addDecl(D);
            return true;
        }
    }

    // Fallback to DeclContext
    if (CurrentDeclContext) {
        NamedDecl* D = CurrentDeclContext->lookup(R.getLookupName());
        if (D) {
            R.addDecl(D);
            return true;
        }
    }

    // No match found
    return false;
}

// === Lookup in a specific DeclContext ===
bool Sema::LookupQualifiedName(LookupResult& R, DeclContext* DC) {
    NamedDecl* D = DC->lookup(R.getLookupName());
    if (D) {
        R.addDecl(D);
        return true;
    }
    return false;
}

// === Lookup a possibly qualified name (e.g. A::x)
bool Sema::LookupParsedName(LookupResult& R, Scope* S, /* CXXScopeSpec* SS stubbed */ void*) {
    // Stub: we just use LookupName
    return LookupName(R, S);
}

// === Template-safe version ===
bool Sema::LookupNameOrErr(LookupResult& R, Scope* S) {
    bool found = LookupName(R, S);
    if (!found) {
        // In real Clang, diagnostics would go here
        return false;
    }
    return true;
}

bool Sema::LookupADL(LookupResult& R, Scope* S) {
    // Just fallback to normal lookup for now
    return LookupName(R, S);
}


Expr* Sema::ActOnIdentifierExpr(Scope* S, DeclContext* OwnerContext, IdentifierInfo* II) {
    // Unqualified Lookup in Scope
    LookupResult Res(*this, II, LookupNameKind::LookupOrdinaryName);
    if (!Res.isSingleResult()) {
        return Context.create<UnresolvedLookupExpr>(II, false, false);
    }

    NamedDecl* ND = Res.getFoundDecl();
    if (!isa<VarDecl>(ND))
    {
        throw std::runtime_error("Expected Identifier of Variable");
    }
    return Context.create<DeclRefExpr>(ND, OwnerContext);
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
