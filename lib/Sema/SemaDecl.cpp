#include "Sema/Sema.hpp"
#include "Sema/Scope.hpp"
#include "AST/ASTDecl.hpp"
#include "Parser/Parser.hpp"
#include "Sema/LookupResult.hpp"
#include <cassert>

Sema::ParseScope::ParseScope(Parser& P,
    unsigned flags,
    Decl* associatedDecl) : P(P), ActualScope(P.getCurrentScope(), flags) {
    P.setCurrentScope(&ActualScope);
    P.getSema().ActOnScopeStart(&ActualScope, associatedDecl); // Pass associatedDecl to Sema
    std::cout << "  [Parser::ParseScope]: Pushed new lexical scope (flags: " << flags << ", ptr: " << P.getCurrentScope() << ")" << std::endl;
}
Sema::ParseScope::~ParseScope() {
    P.getSema().ActOnScopeEnd(&ActualScope); // Pass the Scope object to Sema for proper context popping
    P.setCurrentScope(ActualScope.getParent());// Restore Parser's current scope to its parent
    std::cout << "  [Parser::ParseScope]: Popped lexical scope (ptr: " << &ActualScope << ")" << std::endl;
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

void Sema::ActOnScopeStart(Scope* S, Decl* AssociatedDecl) {
    if (S == nullptr) {
        std::cerr << "Sema: Error: ActOnScopeStart called with nullptr Scope!" << std::endl;
        return;
    }

    std::cout << "  Sema: ActOnScopeStart (Scope ptr: " << S << ", Flags: " << S->getFlags() << ")" << std::endl;

    DeclContext* newDeclContextForScope = nullptr;
    if (S->getFlags() & ScopeFlags::FunctionScope) {
        if (FunctionDecl* FD = dyn_cast<FunctionDecl>(AssociatedDecl)) {
            newDeclContextForScope = FD;
            PushDeclContext(newDeclContextForScope);
            std::cout << "  Sema: DeclContext changed to FunctionDecl: " << FD->getName() << std::endl;
        }
        else {
            std::cerr << "Sema: Warning: FnScope flag set but no FunctionDecl provided for scope start!" << std::endl;
        }
    }
    S->Entity = newDeclContextForScope;
}

void Sema::ActOnScopeEnd(Scope* S) {
    if (S == nullptr) {
        std::cerr << "Sema: Error: ActOnScopeEnd called with nullptr Scope!" << std::endl;
        return;
    }
    std::cout << "  Sema: ActOnScopeEnd (Scope ptr: " << S << ")" << std::endl;

    if (S->Entity != nullptr) {
        PopDeclContext();
        std::cout << "  Sema: DeclContext restored." << std::endl;
    }
}


// Helper to find the previous declaration of a given name in the current context.
NamedDecl* Sema::findPreviousDecl(Scope* S, IdentifierInfo* Name) {
    LookupResult R(*this, Name);
    LookupName(R, S);
    return R.getFoundDecl();
}

// Function to check redeclaration legality. Always returns true (illegal) for this task.
bool Sema::CheckRedeclaration(NamedDecl* NewDecl, NamedDecl* PreviousDecl) {
    return true; // Indicate that this redeclaration is illegal
}



// --- The Core of Name Lookup Registration in Sema ---
    // Takes the NamedDecl to register and the *lexical Scope* where it becomes visible.
void Sema::PushOnScopeChains(NamedDecl* D, Scope* S) {
    //TODO: There is no swithch case in clang, the flow is linear for each type of Decl
    NamedDecl* PrevDecl = findPreviousDecl(S, D->getIdentifier());

    // 2. Perform legality check if a previous declaration was found.
    bool IsRedeclarationIllegal = false;
    if (PrevDecl) {
        IsRedeclarationIllegal = CheckRedeclaration(D, PrevDecl);
    }

    if (IsRedeclarationIllegal) {
        D->setInvalid(true); // Mark the new declaration as invalid
        std::cerr << "Sema: Declining to register illegal redeclaration '" << D->getName() << "'." << std::endl;
        return; // Do not proceed to add to Scope or DeclContext lookup structures
    }

    // If legal (or no previous decl), proceed with registration:

    // 3. Set the intrusive link on the new declaration.
    D->PreviousDeclInContext = PrevDecl;

    
    // Always add to the provided lexical scope for basic visibility.
    if (S) S->addDecl(D);

    // Now, decide if it should *also* be added to the DeclContext's permanent map.
    DeclContext* OwningDC = D->getOwningDeclContext();

    std::cout << "  DEBUG: PushOnScopeChains for '" << D->getName() << "' (DeclKind: " << static_cast<unsigned>(D->getKind()) << ")"
        << " in DeclContext " << OwningDC->getDeclContextKindName() << std::endl;

    switch (OwningDC->getDeclContextKind()) {
    case DeclContextKind::TranslationUnit:
    case DeclContextKind::Namespace:
        OwningDC->addNamedDeclToLookupMap(D);
        break;

    case DeclContextKind::Record:
        OwningDC->addNamedDeclToLookupMap(D);
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
            OwningDC->addNamedDeclToLookupMap(D);
        }
        break;
    default:
        break;
    }
}


VarDecl* Sema::ActOnVarDecl(Type* Ty, IdentifierInfo* II, Expr* Init) {
    DeclContext* new_owner = determine_owner_context(Ty, II, Init);
    // 1. Create the Decl AST node. Sema owns this memory.
    VarDecl* VD = Context.create<VarDecl>(II, new_owner, Ty, Init);
    return VD;
}


TypedefDecl* Sema::ActOnTypedefDecl(Type* Ty, IdentifierInfo* II) {
    DeclContext* new_owner = determine_owner_context(Ty, II);
    TypedefDecl* TD = Context.create<TypedefDecl>(II, new_owner, Ty);
    return TD;
}