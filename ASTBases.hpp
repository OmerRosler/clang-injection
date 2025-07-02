#pragma once
#include "Basic.hpp"


// === AST Base Classes ===

struct Type {
    enum Kind { Builtin, Unresolved } kind;
    Type(Kind k) : kind(k) {}
    virtual ~Type() = default;
    virtual void print() const = 0;

    Kind getKind() const { return kind; }
};

struct Expr {
    enum Kind { IntLit, Var, Add } kind;
    Expr(Kind k) : kind(k) {}
    virtual ~Expr() = default;
    virtual void print() const = 0;

    Kind getKind() const { return kind; }
};