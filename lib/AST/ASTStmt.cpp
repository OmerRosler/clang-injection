#include "AST/ASTStmt.hpp"
#include "AST/ASTDecl.hpp"

void DeclStmt::print(std::ostream& os) const {
    os << "Statement of the declaration ";
    TheDecl->print(os);
}


void CompoundStmt::print(std::ostream& os) const {
    os << "{\n";
    for (auto* stmt : body) {
        os << "  ";
        if (stmt) stmt->print(os);
        else os << "<null>";
        os << ";\n";
    }
    os << "}";
}