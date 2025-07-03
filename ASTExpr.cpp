//TODO: This file exists for cyclic dependency, consider fixing it instead
#include "ASTExpr.hpp"
#include "ASTDecl.hpp"

void DeclRefExpr::print(std::ostream& os) const {
    if (decl)
        os << "<ref to " << decl->getName() << ">";
    else
        os << "<null decl>";
}

void UnresolvedNameExpr::print(std::ostream& os) const {
    os << "<unknown> " << Ident->getName();
}