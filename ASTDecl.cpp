#include "ASTDecl.hpp"
#include "ASTExpr.hpp"
#include "ASTType.hpp"

void VarDecl::print(std::ostream& os) const {
    os << "VarDecl: " << getName() << " : ";
    if (VarType) VarType->print(os);
    else os << "<null type>";
    if (InitExpr) {
        os << " = ";
        InitExpr->print(os);
    }
    os << "\n";
}

void TypedefDecl::print(std::ostream& os) const {
    os << "TypedefDecl: " << getName() << " = ";
    if (AliasedType) AliasedType->print(os);
    else os << "<null type>";
    os << "\n";
}