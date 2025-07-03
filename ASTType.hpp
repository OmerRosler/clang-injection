#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Basic.hpp"

// === Base class for all types ===
class Type {
public:
    enum class TypeClass { Builtin, Typedef, Resolved, Unresolved };

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
    IdentifierInfo* II;
    // Once resolved, this points to the actual Type (or nullptr if unresolved)
    Type* resolved = nullptr;
    DeclContext* OwnerContext = nullptr; // TODO: Handle this properly

    explicit UnresolvedType(IdentifierInfo* II, DeclContext* OwnerContext)
        : Type(TypeClass::Unresolved), II(II), OwnerContext(OwnerContext) {}

    static bool classof(const Type* e) {
        return e->typeClass == TypeClass::Unresolved;
    }

    void print(std::ostream& os) const override;
};


struct TypedefType : Type {
    TypedefDecl* TD;
    explicit TypedefType(TypedefDecl* td) : Type(TypeClass::Typedef), TD(td) {}
    void print(std::ostream& os) const override;
};


struct ResolvedType : Type {
    IdentifierInfo* II;
    explicit ResolvedType(IdentifierInfo* II) : Type(TypeClass::Resolved), II(II) {}
    void print(std::ostream& os) const override;
};