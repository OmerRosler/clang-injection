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
        Namespace,
        // ... other decl kinds can go here
    };

    static bool classof(const Decl* D) { return true; } // base class matches all

    explicit Decl(Kind k, DeclContext* DC) : 
        PreviousDeclInContext(nullptr), K(k), OwningDC(DC) {}

    Decl* getPreviousDeclInContext() const { return PreviousDeclInContext; }
    void setPreviousDeclInContext(Decl* Prev) { PreviousDeclInContext = Prev; }

    Kind getKind() const { return K; }
    DeclContext* getOwningDeclContext() const { return OwningDC; }
    void setOwningDeclContext(DeclContext* DC) { OwningDC = DC; }

    bool isInvalidDecl() const { return IsInvalidDecl; }
    void setInvalid(bool invalid = true) { IsInvalidDecl = invalid; }

    virtual ~Decl() = default;

    // Pretty print base
    virtual void print(std::ostream& os) const = 0;

    // Static polymorphism helpers:

    bool isVarDecl() const { return K == Kind::Var; }
    bool isFunctionDecl() const { return K == Kind::Function; }
    bool isTypedefDecl() const { return K == Kind::Typedef; }
    bool isNamespaceDecl() const { return K == Kind::Namespace; }

protected:

    Decl* PreviousDeclInContext;
    Kind K;
    DeclContext* OwningDC = nullptr;
    bool IsInvalidDecl;


};

// NamedDecl derives from Decl, adds a name
struct NamedDecl : public Decl {
    IdentifierInfo* Id;


    NamedDecl(Kind k, DeclContext* DC, IdentifierInfo* id) : Decl(k, DC), Id(id) {}

    static bool classof(const Decl* D) {
        return D->getKind() == Kind::Var || D->getKind() == Kind::Typedef;
    }
    IdentifierInfo* getIdentifier() const { return Id; }
    const std::string& getName() const { return Id->getName(); }



};

// VarDecl class
struct VarDecl : public NamedDecl {
    Type* VarType;
    Expr* InitExpr;  // pointer to AST Expr node (initializer), nullable

    VarDecl(IdentifierInfo* id, DeclContext* DC, Type* type, Expr* init = nullptr)
        : NamedDecl(Kind::Var, DC, id), VarType(type), InitExpr(init) {}

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

    TypedefDecl(IdentifierInfo* id, DeclContext* DC, Type* aliased)
        : NamedDecl(Kind::Typedef, DC, id), AliasedType(aliased) {}

    static bool classof(const Decl* D) {
        return D->getKind() == Kind::Typedef;
    }

    Type* getAliasedType() const { return AliasedType; }

    void print(std::ostream& os) const override;
};

struct FunctionDecl : public NamedDecl, public DeclContext
{
    //TODO
    FunctionDecl(IdentifierInfo* id, DeclContext* DC)
        : NamedDecl(Kind::Function, DC, id) {}

    static bool classof(const Decl* D) {
        return D->getKind() == Kind::Function;
    }

};

struct NamespaceDecl : public NamedDecl, public DeclContext
{
    //TODO
    NamespaceDecl(IdentifierInfo* id, DeclContext* DC)
        : NamedDecl(Kind::Namespace, DC, id) {}

    static bool classof(const Decl* D) {
        return D->getKind() == Kind::Namespace;
    }

};