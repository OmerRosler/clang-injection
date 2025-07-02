#pragma once
#include <utility>
#include <typeinfo>

// Forward declarations

//AST

// Types
class Type;

struct BuiltinType;
struct ResolvedType;
struct UnresolvedType;

// Statements
class Stmt;

struct Expr;

struct IntLiteralExpr;
struct DoubleLiteralExpr;

struct DeclRefExpr;
struct UnresolvedNameExpr;

struct PlusExpr;
struct MultiplyExpr;

//Declarations
class Decl;

struct NamedDecl;

struct VarDecl;
struct TypedefDecl;


//Compilation phases
class Preprocessor;
class Lexer;
class Parser;
class Sema;

//static polymorphism helpers

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