#include "ASTType.hpp"
#include "ASTDecl.hpp"

void TypedefType::print(std::ostream& os) const {
    os << TD->getName();
}

void UnresolvedType::print(std::ostream& os) const {
    if (resolved) {
        os << "<resolved type> ";
        resolved->print(os);
    }
    else {
        os << "<unresolved type> " << II->getName();
    }
}

void ResolvedType::print(std::ostream& os) const {
    os << II->getName(); 
}