#include "AST/ASTDecl.hpp"
#include "AST/ASTExpr.hpp"
#include "AST/ASTType.hpp"

void Decl::setOwningDeclContext(DeclContext* DC) {
    OwningDC = DC;
    if (DC) {
        DC->addDeclToContext(this); // Tell the DeclContext to add this Decl
    }
}


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
