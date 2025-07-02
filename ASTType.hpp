#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Basic.hpp"
// Forward declaration
struct Type;
struct ASTContext;

// === Base class for all types ===
struct Type {
    enum class TypeClass { Builtin, Unresolved };
    TypeClass typeClass;
    explicit Type(TypeClass tc) : typeClass(tc) {}
    virtual ~Type() = default;

    // Base class classof - trivial, always true if pointer not null
    static bool classof(const Type* t) {
        return t != nullptr;
    }

    virtual void print(std::ostream& os) const = 0;

};

// === Concrete Type nodes ===
struct BuiltinType : Type {
    enum BuiltinKind { Int, Double };
    BuiltinKind kind;
    explicit BuiltinType(BuiltinKind k) : Type(TypeClass::Builtin), kind(k) {}

    static bool classof(const Type* e) {
        return e->typeClass == TypeClass::Builtin;
    }


    void print(std::ostream& os) const override {
        switch (kind) {
        case BuiltinKind::Int: os << "int"; break;
        case BuiltinKind::Double: os << "double"; break;
        }
    }

};

struct UnresolvedType : Type {
    std::string name;
    // Once resolved, this points to the actual Type (or nullptr if unresolved)
    Type* resolved = nullptr;

    explicit UnresolvedType(std::string n)
        : Type(TypeClass::Unresolved), name(std::move(n)) {}

    static bool classof(const Type* e) {
        return e->typeClass == TypeClass::Unresolved;
    }

    void print(std::ostream& os) const override {
        if (resolved) {
            os << "<resolved> ";
            resolved->print(os);
        }
        else {
            os << "<unresolved> " << name;
        }
    }
};