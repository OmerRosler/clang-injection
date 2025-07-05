#include "Sema.hpp"
#include "LookupResult.hpp"
#include "Scope.hpp"
#include "ASTDecl.hpp"

// hiding is hard
bool Sema::LookupName(LookupResult& R, Scope* S, bool AllowBuiltinCreation) {
    if (R.getLookupKind() == LookupNameKind::LookupADL)
        return LookupADL(R, S);

    //TODO: See bug #12. The logic is wrong, we won't fix it here
    IdentifierInfo* Name = R.getLookupName();
    // Try scope-based lookup
    for (Scope* Cur = S; Cur; Cur = Cur->getParent()) {
        //look at locals
        for (auto&[LocalName, ND] : Cur->decls())
        {
            if (ND->getIdentifier() == Name)
            {
                //found non function, it hides all, we stop everything
                if (!isa<FunctionDecl>(ND))
                {
                    // if already found functions, remove them
                    //TODO: If R is not empty here, there is an error, we report it when we add diagnostics
                    R.clear();
                    R.addDecl(ND);
                    return true;
                }
                R.addDecl(ND);
            }
        }
        if (!R.empty())
        {
            //found overload set in this scope, it hides the DeclContext so we break
            break;
        }

        //lookup at the entity associated with the scope if exists (say other members of a class)
        if (DeclContext* DC = Cur->getEntity()) {
            //add results from semantic owner of the scope
            for (auto& [LocalName, ND] : DC->decls())
            {
                if (ND->getIdentifier() == Name)
                {
                    //found non function, it hides all, we stop everything
                    if (!isa<FunctionDecl>(ND))
                    {
                        // if already found functions, remove them
                        //TODO: If R is not empty here, there is an error, we report it when we add diagnostics
                        R.clear();
                        R.addDecl(ND);
                        return true;
                    }
                    R.addDecl(ND);
                }
            }
        }
    }

    // No match found
    return !R.empty();
}

// === Lookup in a specific DeclContext ===
bool Sema::LookupQualifiedName(LookupResult& R, DeclContext* DC) {
    //TODO: Fix this when we get to qualified lookup
    return DC->lookupDirectMember(R.getLookupName(), R);
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

