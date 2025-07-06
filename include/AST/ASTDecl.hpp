#pragma once
#include <string>
#include <iostream>
#include "Basic.hpp"
#include "Preprocessor/IdentifierTable.hpp"
#include "Lexer/IdentifierInfo.hpp"
#include "AST/DeclContext.hpp"
// Base Decl class with Kind enum for static polymorphism RTTI
class Decl {

public:
    enum class Kind {
        Var,
        Typedef,
        Function,
        Namespace,
        Record,
        Enum,
        CXXMethod,
        // ... other decl kinds can go here
    };

    static bool classof(const Decl* D) { return true; } // base class matches all

    explicit Decl(Kind k, DeclContext* DC) : 
        PrevInContextPtr(nullptr), K(k), OwningDC(nullptr), IsInvalidDecl(false) 
    {
        setOwningDeclContext(DC);
    }

    // Intrusive list setters/getters (for DeclContext internal use)
    Decl* getNextInContext() const { return NextInContextPtr; }
    void setNextInContext(Decl* D) { NextInContextPtr = D; }
    Decl* getPrevInContext() const { return PrevInContextPtr; }
    void setPrevInContext(Decl* D) { PrevInContextPtr = D; }


    Kind getKind() const { return K; }
    DeclContext* getOwningDeclContext() const { return OwningDC; }
    void setOwningDeclContext(DeclContext* DC);

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

    // Pointers for intrusive list in DeclContext
    Decl* NextInContextPtr; // Pointer to the next Decl in the OwningDC's list
    Decl* PrevInContextPtr; // Pointer to the previous Decl in the OwningDC's list
    Kind K;
    DeclContext* OwningDC = nullptr;
    bool IsInvalidDecl;


};

// NamedDecl derives from Decl, adds a name
struct NamedDecl : public Decl {
    IdentifierInfo* Id;


    NamedDecl(Kind k, DeclContext* DC, IdentifierInfo* id) : Decl(k, DC), Id(id) {}

    static bool classof(const Decl* D) {
        //TODO: Fix this
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
    FunctionDecl(IdentifierInfo* id, DeclContext* OwnerDC)
        : NamedDecl(Kind::Function, OwnerDC, id), 
        DeclContext(DeclContextKind::Function, static_cast<Decl*>(this)) {}

    static bool classof(const Decl* D) {
        return D->getKind() == Kind::Function;
    }
    /*static bool classof(const DeclContext* D) {
        return D->getDeclContextKind() == DeclContextKind::Function;
    }*/

    void print(std::ostream& os) const override
    {
        os << "In function ";
    }

};

struct NamespaceDecl : public NamedDecl, public DeclContext
{
    //TODO
    NamespaceDecl(IdentifierInfo* id, DeclContext* DC)
        : NamedDecl(Kind::Namespace, DC, id), 
        DeclContext(DeclContextKind::Namespace, static_cast<Decl*>(this)) {}

    static bool classof(const Decl* D) {
        return D->getKind() == Kind::Namespace;
    }

    void print(std::ostream& os) const override
    {
        os << "In namespace ";
    }

};