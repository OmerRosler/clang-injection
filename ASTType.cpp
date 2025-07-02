#include "ASTType.hpp"
#include "ASTDecl.hpp"

void TypedefType::print(std::ostream& os) const {
    os << TD->getName();
}