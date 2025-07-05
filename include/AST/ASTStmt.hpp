#pragma once
#include <ostream>
#include <vector>
#include "Basic.hpp"

// === Base class for all statements and expressions ===
class Stmt {
public:
    enum class StmtClass { Expr, DeclStmt, CompoundStmt, /*...other stmt kinds*/ }; 

    explicit Stmt(StmtClass sc) : stmtClass(sc) {}

    StmtClass stmtClass;

    virtual ~Stmt() = default;

    static bool classof(const Stmt* s) {
        // Always true if input is already a Stmt pointer
        return s != nullptr;
    }

    virtual void print(std::ostream& os) const = 0;
};

class DeclStmt : public Stmt {
private:
    Decl* TheDecl;
public:
    explicit DeclStmt(Decl* TheDecl) : Stmt(StmtClass::DeclStmt), TheDecl(TheDecl) {}
    Decl* getDecl() const { return TheDecl; }

    void print(std::ostream& os) const override;

    static bool classof(const Stmt* e) {
        return e && e->stmtClass == StmtClass::DeclStmt;
    }
};

class CompoundStmt : public Stmt {
private:
    std::vector<Stmt*> body;

public:
    explicit CompoundStmt(std::vector<Stmt*> stmts)
        : Stmt(StmtClass::CompoundStmt), body(std::move(stmts)) {}

    const std::vector<Stmt*>& getBody() const { return body; }

    void print(std::ostream& os) const override;
};