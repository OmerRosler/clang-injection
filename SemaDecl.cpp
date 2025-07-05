#include "Sema.hpp"
#include "Scope.hpp"
#include "ASTDecl.hpp"

VarDecl* Sema::ActOnVarDecl(Scope* S, DeclContext* DC,
    Type* Ty, IdentifierInfo* II, Expr* Init) {

    NamedDecl* Existing = S->lookupFirstLocal(II);
    if (Existing)
    {
        //TODO: Diagnostic of redefinition
        return nullptr;
    }
    auto* VD = Context.create<VarDecl>(II, Ty, Init);
    // Register in current DeclContext
    DC->addDecl(VD);

    // Register in Scope
    S->addDecl(VD);

    return VD;
}


TypedefDecl* Sema::ActOnTypedefDecl(Scope* S, DeclContext* DC, 
    Type* Ty, IdentifierInfo* II) {
    TypedefDecl* TD = Context.create<TypedefDecl>(II, Ty);

    DC->addDecl(TD);
    S->addDecl(TD);
    return TD;
}