#include "Sema/Sema.hpp"
#include "Sema/LookupResult.hpp"
#include "Sema/Scope.hpp"
#include "AST/ASTDecl.hpp"

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

//UnresolvedType* Sema::ActOnUnresolvedType(IdentifierInfo* II) {
//    return Context.create<UnresolvedType>(II);
//}

ParenExpr* Sema::ActOnParenExpr(Expr* SubExpr)
{
    return Context.create<ParenExpr>(SubExpr);
}



Expr* Sema::ActOnIdentifierExpr(Scope* S, DeclContext* OwnerContext, IdentifierInfo* II) {
    // Unqualified Lookup in Scope
    LookupResult Res(*this, II, LookupNameKind::LookupOrdinaryName);
    LookupName(Res, S);
    if (!Res.isSingleResult()) {
        return Context.create<UnresolvedLookupExpr>(II, false, false);
    }

    NamedDecl* ND = Res.getFoundDecl();
    if (!isa<VarDecl>(ND))
    {
        throw std::runtime_error("Expected Identifier of Variable");
    }
    return Context.create<DeclRefExpr>(ND);
}

