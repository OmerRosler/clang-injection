#include "Sema.hpp"
#include "ASTStmt.hpp"

DeclStmt* Sema::ActOnDeclStmt(Decl* D) {
    return Context.create<DeclStmt>(D);
}

CompoundStmt* Sema::ActOnCompoundStmt(std::vector<Stmt*> stmts) {
    return Context.create<CompoundStmt>(std::move(stmts));
}