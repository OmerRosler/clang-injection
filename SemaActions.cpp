#include "Sema.hpp"

//Type* Sema::ActOnTypeName(IdentifierInfo* II) {
//    // First check if builtin
//    auto name = II->getName();
//    if (II->isKeyword())
//    {
//        if (name == "int" || name == "double")
//        {
//            return ActOnBuiltinType(II);
//        }
//        else
//        {
//            throw std::runtime_error("Expected a type, got keyword");
//        }
//    }
//    // Lookup in DeclContext
//    NamedDecl* decl = CurrentDeclContext->lookup(II);
//    if (decl) {
//        // For toy model, return resolved type with the name of decl
//        return Context.create<ResolvedType>(II);
//    }
//    else {
//        // Unknown type - create unresolved placeholder
//        return Context.create<UnresolvedType>(II);
//    }
//}

Expr* Sema::ActOnIdentifier(IdentifierInfo* II) {
    NamedDecl* decl = lookupName(II); // lookup inside Sema
    if (decl)
        return Context.create<DeclRefExpr>(decl);
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