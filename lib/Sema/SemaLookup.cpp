#include "Sema/Sema.hpp"
#include "Sema/LookupResult.hpp"
#include "Sema/Scope.hpp"
#include "AST/ASTDecl.hpp"
// --- Unqualified Name Lookup ---
    // This simulates Sema's lookup process for an unqualified name.
bool Sema::LookupName(LookupResult& R, Scope* S, bool AllowBuiltinCreation) {
    IdentifierInfo* Name = R.getLookupName();
    std::cout << "\nDEBUG: Looking up unqualified name '" << Name->getName() << "'" << std::endl;

    // Phase 1: Lexical Scope Search (walking up the Scope chain)
    while (S) {
        if (NamedDecl* Found = S->lookupLocal(Name)) {
            R.addDecl(Found);
            std::cout << "  DEBUG: Found '" << Name->getName() << "' in lexical Scope (Addr: " << S << ")" << std::endl;
            return true; // For simplicity, we stop at the first lexical match.
            // Real Clang handles shadowing (hiding outer decls) more precisely.
        }
        S = S->getParent();
    }
    std::cout << "  DEBUG: Not found in lexical scopes." << std::endl;

    // Phase 2: DeclContext Member/Namespace Search (walking up the DeclContext chain)
    // This is simplified: in Clang, this involves more complex rules (e.g., ADL, using directives).
    // We'll search the current semantic context and then its semantic parents (if applicable).
    DeclContext* CurrentSemanticContext = CurrentDeclContext;
    while (CurrentSemanticContext) {
        // Check if the current semantic context has this name directly in its MemberMap.
        if (NamedDecl* Found = CurrentSemanticContext->lookup(Name)) {
            std::cout << "  DEBUG: Found '" << Name->getName() << "' in DeclContext (Kind: "
                << CurrentSemanticContext->getDeclContextKindName() << ", Addr: " << CurrentSemanticContext << ")" << std::endl;
            R.addDecl(Found);
            return true;
        }

        // Move up the semantic parent chain if required (this is a simplification for now)
        CurrentSemanticContext = CurrentSemanticContext->getParent();
    }

    std::cout << "  DEBUG: Name '" << Name->getName() << "' not found in any visible context." << std::endl;
    return false; // Not found
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

