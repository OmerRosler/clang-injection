#pragma once
#include "Basic.hpp"

// === Base class for all statements and expressions ===
struct Stmt {
    enum class StmtClass { Expr, /*...other stmt kinds*/ };
    StmtClass stmtClass;
    explicit Stmt(StmtClass sc) : stmtClass(sc) {}
    virtual ~Stmt() = default;

    static bool classof(const Stmt* s) {
        // Always true if input is already a Stmt pointer
        return s != nullptr;
    }

    virtual void print(std::ostream& os) const = 0;
};