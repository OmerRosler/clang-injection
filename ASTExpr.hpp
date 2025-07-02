#pragma once
#include <iostream> //TODO: Remove, if we add pretty print it would just return a string view
#include "Basic.hpp"
#include "ASTStmt.hpp"

// === Base class for expressions ===
struct Expr : Stmt {
    enum class ExprClass { IntLiteral, DoubleLiteral, PlusExpr, MultiplyExpr, UnknownName
    };
    ExprClass exprClass;
    explicit Expr(ExprClass ec) : Stmt(StmtClass::Expr), exprClass(ec) {}
    virtual ~Expr() = default;

    static bool classof(const Stmt* s) {
        return s && s->stmtClass == StmtClass::Expr;
    }

    virtual void print(std::ostream& os) const = 0;

};

// === Concrete Expr nodes ===
struct IntLiteralExpr : Expr {
    int value;
    explicit IntLiteralExpr(int v) : Expr(ExprClass::IntLiteral), value(v) {}

    static bool classof(const Expr* e) {
        return e->exprClass == ExprClass::IntLiteral;
    }

    void print(std::ostream& os) const override {
        os << value;
    }
};

struct DoubleLiteralExpr : Expr {
    double value;
    explicit DoubleLiteralExpr(double v) : Expr(ExprClass::DoubleLiteral), value(v) {}

    static bool classof(const Expr* e) {
        return e->exprClass == ExprClass::DoubleLiteral;
    }

    void print(std::ostream& os) const override {
        os << value;
    }
};


// === PlusExpr ===
struct PlusExpr : Expr {
    Expr* lhs;
    Expr* rhs;

    PlusExpr(Expr* l, Expr* r) : Expr(ExprClass::PlusExpr), lhs(l), rhs(r) {}

    void print(std::ostream& os) const override {
        os << "(";
        if (lhs) lhs->print(os); else os << "<null>";
        os << " + ";
        if (rhs) rhs->print(os); else os << "<null>";
        os << ")";
    }

    static bool classof(const Expr* e) {
        return e && e->exprClass == ExprClass::PlusExpr;
    }
};

// === MultiplyExpr ===
struct MultiplyExpr : Expr {
    Expr* lhs;
    Expr* rhs;

    MultiplyExpr(Expr* l, Expr* r) : Expr(ExprClass::MultiplyExpr), lhs(l), rhs(r) {}

    void print(std::ostream& os) const override {
        os << "(";
        if (lhs) lhs->print(os); else os << "<null>";
        os << " * ";
        if (rhs) rhs->print(os); else os << "<null>";
        os << ")";
    }

    static bool classof(const Expr* e) {
        return e && e->exprClass == ExprClass::MultiplyExpr;
    }
};

struct DeclRefExpr : Expr {
    NamedDecl* decl;   // pointer to resolved declaration
    explicit DeclRefExpr(NamedDecl* d) : Expr(ExprClass::UnknownName), decl(d) {}
    void print(std::ostream& os) const override;
};

struct UnresolvedNameExpr : Expr {
    std::string name;
    explicit UnresolvedNameExpr(std::string n) : Expr(ExprClass::UnknownName), name(std::move(n)) {}
    void print(std::ostream& os) const override {
        os << "<unresolved> " << name;
    }
};