#include "Sema.hpp"

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
