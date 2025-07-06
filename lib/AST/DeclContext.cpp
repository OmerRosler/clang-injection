#include "AST/DeclContext.hpp"
#include "Sema/LookupResult.hpp"
#include "AST/ASTDecl.hpp"
#include <cassert>

DeclContext* DeclContext::getParent() const
{
    if (DeclContextDecl) return DeclContextDecl->getOwningDeclContext();
    throw std::runtime_error("Haven't implemeneted parentless DeclContext yet");
}

// --- Private/Protected internal methods for Sema to call ---
    // Sema calls these to register a Decl to this DeclContext's internal map.
    // This method just performs the insertion; it doesn't decide *if* to insert.
void DeclContext::addDeclInternal(NamedDecl* D) {
    // In a real compiler, this would handle overloads and redeclarations
    // (e.g., adding to a list of functions if multiple exist with the same name).
    // For simplicity, we just insert the latest.
    VisibleMembers[D->getIdentifier()] = D;
    std::cout << "  DEBUG: DeclContext '" <<
        getDeclContextKindName() << "' added '"
        << D->getIdentifier()->getName() << "' to its MemberMap." << std::endl;
}

// This method is called by Decl::setDeclContext() internally.
// It links the Decl into this DeclContext's intrusive list.
void DeclContext::addDeclToContext(Decl* D) {
    //TODO: This is general logic of intrusive list, maybe move it to a separate mixin class
    if (!FirstDeclInContext) {
        FirstDeclInContext = D;
        LastDeclInContext = D;
    }
    else {
        LastDeclInContext->setNextInContext(D);
        D->setPrevInContext(LastDeclInContext);
        LastDeclInContext = D;
    }
}

// Public API for lookup. Queries this DeclContext's map.
NamedDecl* DeclContext::lookup(IdentifierInfo* Name) { // Renamed from lookupMember for clarity
    auto It = VisibleMembers.find(Name);
    if (It != VisibleMembers.end()) {
        return It->second;
    }
    return nullptr;
}