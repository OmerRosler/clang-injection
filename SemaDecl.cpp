#include "Sema.hpp"
#include "Scope.hpp"
#include "ASTDecl.hpp"

VarDecl* Sema::ActOnVarDecl(Scope* S, DeclContext* DC,
    Type* Ty, IdentifierInfo* II, Expr* Init) {
    auto* VD = Context.create<VarDecl>(II, Ty, Init);

    // Register in current DeclContext
    DC->addDecl(VD);

    // Register in Scope
    S->AddDecl(VD);

    return VD;
}


TypedefDecl* Sema::ActOnTypedefDecl(Scope* S, DeclContext* DC, 
    Type* Ty, IdentifierInfo* II) {
    TypedefDecl* TD = Context.create<TypedefDecl>(II, Ty);

    DC->addDecl(TD);
    S->AddDecl(TD);
    return TD;
}