#include "Sema/Sema.hpp"
#include "Sema/Scope.hpp"
#include "AST/ASTDecl.hpp"
#include <cassert>
void Sema::PushScope(unsigned flags) {
    // Allocate a new Scope, setting its parent to the current CurScope.
    Scope* newScope = Context.create<Scope>(CurScope, flags);
    CurScope = newScope; // Update CurScope to point to the new innermost scope
}

// Pops the current lexical scope from the stack.
void Sema::PopScope() {
    if (!CurScope->getParent()) {
        // Should not pop the outermost global scope through this method typically.
        std::cerr << "ERROR: Attempted to pop the global lexical scope!" << std::endl;
        return;
    }
    // Update CurScope to point to the parent scope.
    // The popped Scope object itself is now "inactive" but not deleted here.
    CurScope = CurScope->getParent();
}

void Sema::PushDeclContext(DeclContext* DC) {
    CurrentDeclContext = DC;
    std::cout << "  DEBUG: Current DeclContext changed to " << DC->getDeclContextKindName() << std::endl;
}

void Sema::PopDeclContext() {
    // In a real Clang, this would involve traversing up the semantic parent chain
    // of the current DeclContext. Our simplified DeclContext doesn't expose a getParent().
    // For example, if CurContext is a RecordDecl, its parent would be the DeclContext
    // that owns that RecordDecl (likely a NamespaceDecl or TranslationUnitDecl).
    // For now, we'll just go back to the global semantic context as a fallback.
    // This is a major simplification compared to real Clang's management of CurContext.
    CurrentDeclContext = CurrentDeclContext->getParent(); // Fallback to global for demo purposes
    std::cout << "  DEBUG: Current DeclContext reverted to " << CurrentDeclContext->getDeclContextKindName() << std::endl;
}
// --- The Core of Name Lookup Registration in Sema ---
    // Takes the NamedDecl to register and the *lexical Scope* where it becomes visible.
void Sema::PushOnScopeChains(NamedDecl* D, Scope* S) {
    // Always add to the provided lexical scope for basic visibility.
    S->addDecl(D);

    // Now, decide if it should *also* be added to the DeclContext's permanent map.
    DeclContext* OwningDC = D->getOwningDeclContext();

    std::cout << "  DEBUG: PushOnScopeChains for '" << D->getName() << "' (DeclKind: " << static_cast<unsigned>(D->getKind()) << ")"
        << " in DeclContext " << OwningDC->getDeclContextKindName() << std::endl;

    switch (OwningDC->getDeclContextKind()) {
    case DeclContextKind::TranslationUnit:
    case DeclContextKind::Namespace:
        OwningDC->addDeclInternal(D);
        break;

    case DeclContextKind::Record:
        OwningDC->addDeclInternal(D);
        break;

    case DeclContextKind::Function:
    case DeclContextKind::Block:
        // For declarations within a function or block:
        // - Local variables (VarDecl), Local Typedefs:
        //   These are *only* visible lexically (via the Scope stack).
        //   They are *NOT* added to the Function/Block DeclContext's map.
        if (D->getKind() == Decl::Kind::Var || D->getKind() == Decl::Kind::Typedef) {
            std::cout << "  DEBUG: (Local) NOT adding '" << D->getName() << "' to Func/Block DeclContext map." << std::endl;
        }
        // - Nested types (classes/enums/functions) inside a function:
        //   These *are* considered discoverable within the function's semantic context.
        else if (D->getKind() == Decl::Kind::Record ||
            D->getKind() == Decl::Kind::Enum ||
            D->getKind() == Decl::Kind::Function ||
            D->getKind() == Decl::Kind::CXXMethod) {
            OwningDC->addDeclInternal(D);
        }
        break;
    default:
        break;
    }
}


VarDecl* Sema::ActOnVarDecl(Scope* S,
    Type* Ty, IdentifierInfo* II, Expr* Init) {
    DeclContext* new_owner = determine_owner_context(Ty, II, Init);
    // 1. Create the Decl AST node. Sema owns this memory.
    VarDecl* VD = Context.create<VarDecl>(II, new_owner, Ty, Init);
    PushOnScopeChains(VD, S);
    return VD;
}


TypedefDecl* Sema::ActOnTypedefDecl(Scope* S, 
    Type* Ty, IdentifierInfo* II) {
    DeclContext* new_owner = determine_owner_context(Ty, II);
    TypedefDecl* TD = Context.create<TypedefDecl>(II, new_owner, Ty);
    PushOnScopeChains(TD, S);
    return TD;
}