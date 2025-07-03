#include "Sema.hpp"

NamedDecl* Sema::LookupSingleName(DeclContext* DC, IdentifierInfo* II)
{
    return DC->lookup(II);
}

Type* Sema::ActOnIdentifierType(IdentifierInfo* II) {
    if (NamedDecl* D = LookupSingleName(GetCurrentDeclContext(), II)) {
        if (auto* TD = dyn_cast<TypedefDecl>(D)) {
            return Context.getTypeDeclType(TD); // returns TypedefType*
        }
    }

    return Context.create<UnresolvedType>(II);
}

Expr* Sema::ActOnIdentifierExpr(IdentifierInfo* II) {
    if (NamedDecl* D = LookupSingleName(GetCurrentDeclContext(), II))
        return Context.create<DeclRefExpr>(D);
    else
        return Context.create<UnresolvedNameExpr>(II);
}

VarDecl* Sema::ActOnVarDecl(Type* type, IdentifierInfo* id, Expr* init)
{
    return ActOnVarDecl(type, id, init, GetCurrentDeclContext());
}

VarDecl* Sema::ActOnVarDecl(Type* type, IdentifierInfo* id, DeclContext* DC)
{
    return ActOnVarDecl(type, id, nullptr, DC);
}

VarDecl* Sema::ActOnVarDecl(Type* type, IdentifierInfo* id, Expr* init, DeclContext* DC) {
    VarDecl* VD = Context.create<VarDecl>(id, type, init);
    DC->addDecl(VD);
    return VD;
}

TypedefDecl* Sema::ActOnTypedefDecl(Type* type, IdentifierInfo* id, DeclContext* DC) {
    TypedefDecl* TD = Context.create<TypedefDecl>(id, type);
    DC->addDecl(TD);
    return TD;
}

PlusExpr* Sema::ActOnPlusExpr(Expr* lhs, Expr* rhs) {
    // Perform semantic checks (omitted here)
    return Context.create<PlusExpr>(lhs, rhs);
}

MultiplyExpr* Sema::ActOnMultiplyExpr(Expr* lhs, Expr* rhs) {
    return Context.create<MultiplyExpr>(lhs, rhs);
}

IntLiteralExpr* Sema::ActOnIntLiteral(int value) {
    return Context.create<IntLiteralExpr>(value);
}

DoubleLiteralExpr* Sema::ActOnDoubleLiteral(double value) {
    return Context.create<DoubleLiteralExpr>(value);
}

BuiltinType* Sema::ActOnBuiltinType(BuiltinType::BuiltinKind K) {
    return Context.create<BuiltinType>(K);
}

UnresolvedType* Sema::ActOnUnresolvedType(IdentifierInfo* II) {
    return Context.create<UnresolvedType>(II);
}

ParenExpr* Sema::ActOnParenExpr(Expr* SubExpr)
{
    return Context.create<ParenExpr>(SubExpr);
}