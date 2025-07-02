#pragma once
#include <utility>
#include <typeinfo>

// Forward declarations
struct Type;
struct BuiltinType;
struct UnresolvedType;

struct Expr;
struct IntLiteral;
struct VarExpr;
struct AddExpr;

struct Lexer;
struct Preprocessor;
struct Parser;
struct Sema;

// isa implementation
template<typename To, typename From>
bool isa(const From* f) {
    return To::classof(f);
}

// cast implementation (unsafe, assumes isa == true)
template<typename To, typename From>
To* cast(From* f) {
    if (!isa<To>(f)) throw std::bad_cast();
    return static_cast<To*>(f);
}

// dyn_cast implementation (returns nullptr if not correct type)
template<typename To, typename From>
To* dyn_cast(From* f) {
    return isa<To>(f) ? static_cast<To*>(f) : nullptr;
}