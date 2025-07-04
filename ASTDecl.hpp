#pragma once
#include <string>
#include <iostream>
#include "Basic.hpp"
#include "IdentifierTable.hpp"
#include "DeclContext.hpp"
// Base Decl class with Kind enum for static polymorphism RTTI
class Decl {
public:
    enum class Kind {
        Var,
        Typedef,
        Function,
        // ... other decl kinds can go here
    };

    Kind K;

    explicit Decl(Kind k) : K(k) {}

    Kind getKind() const { return K; }

    virtual ~Decl() = default;

    // Pretty print base
    virtual void print(std::ostream& os) const = 0;

    // Static polymorphism helpers:
    static bool classof(const Decl* D) { return true; } // base class matches all
};

// NamedDecl derives from Decl, adds a name
struct NamedDecl : public Decl {
    IdentifierInfo* Id;

    NamedDecl(Kind k, IdentifierInfo* id) : Decl(k), Id(id) {}

    IdentifierInfo* getIdentifier() const { return Id; }
    const std::string& getName() const { return Id->getName(); }

    static bool classof(const Decl* D) {
        return D->getKind() == Kind::Var || D->getKind() == Kind::Typedef;
    }
};

// VarDecl class
struct VarDecl : public NamedDecl {
    Type* VarType;
    Expr* InitExpr;  // pointer to AST Expr node (initializer), nullable

    VarDecl(IdentifierInfo* id, Type* type, Expr* init = nullptr)
        : NamedDecl(Kind::Var, id), VarType(type), InitExpr(init) {}

    static bool classof(const Decl* D) {
        return D->getKind() == Kind::Var;
    }

    Type* getType() const { return VarType; }
    Expr* getInit() const { return InitExpr; }

    void print(std::ostream& os) const override;
};

// TypedefDecl class
struct TypedefDecl : public NamedDecl {
    Type* AliasedType;

    TypedefDecl(IdentifierInfo* id, Type* aliased)
        : NamedDecl(Kind::Typedef, id), AliasedType(aliased) {}

    static bool classof(const Decl* D) {
        return D->getKind() == Kind::Typedef;
    }

    Type* getAliasedType() const { return AliasedType; }

    void print(std::ostream& os) const override;
};

struct FunctionDecl : public NamedDecl, public DeclContext
{
    //TODO
    FunctionDecl(IdentifierInfo* id)
        : NamedDecl(Kind::Function, id) {}

    static bool classof(const Decl* D) {
        return D->getKind() == Kind::Function;
    }

};