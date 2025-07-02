#pragma once
#include "Basic.hpp"

// === Base class for all statements and expressions ===
class Stmt {
public:
    enum class StmtClass { Expr, /*...other stmt kinds*/ }; 

    explicit Stmt(StmtClass sc) : stmtClass(sc) {}

    StmtClass stmtClass;

    virtual ~Stmt() = default;

    static bool classof(const Stmt* s) {
        // Always true if input is already a Stmt pointer
        return s != nullptr;
    }

    virtual void print(std::ostream& os) const = 0;
};