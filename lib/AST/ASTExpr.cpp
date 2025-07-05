//TODO: This file exists for cyclic dependency, consider fixing it instead
#include "AST/ASTExpr.hpp"
#include "AST/ASTDecl.hpp"

void DeclRefExpr::print(std::ostream& os) const {
    if (decl)
        os << "<ref to " << decl->getName() << ">";
    else
        os << "<null decl>";
}

void UnresolvedLookupExpr::print(std::ostream& os) const {
    os << "<unresolved " << (RequiresADL ? "ADL " : "")
        << (Overloaded ? "overload " : "")
        << Name->getName() << ">";
}