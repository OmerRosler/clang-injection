#pragma once
#include "ASTType.hpp"
#include "ASTExpr.hpp"

#include "ASTContext.hpp"
#include "Preprocessor.hpp"
#include "DeclContext.hpp"

class Sema {
    // === Core State ===
    ASTContext& Context;               // Owns the AST nodes
    //DiagnosticsEngine& Diags;         // For errors/warnings
    // SourceManager& SourceMgr;         // For tracking locations
    //Preprocessor& PP;                 // Token info
    //IdentifierTable& Identifiers;     // All known identifiers
    //DeclarationNameTable& DeclarationNames; // All known decl names
    //Builtin::Context BuiltinInfo;     // Info about builtin functions/types

    // === Scope and Lookup ===
    //Scope* CurScope = nullptr;
    //std::vector<DeclContext*> ContextStack;

    // === Symbol Tables ===
    DeclContext* CurrentDeclContext;         // E.g. current function, class, etc.


    // Lookup name in current DeclContext
    NamedDecl* lookupName(const std::string& name) {
        return CurrentDeclContext->lookup(name);
    }

    Type* getTypeName(const std::string& name) {
        // First check if builtin
        if (name == "int" || name == "double") return ActOnBuiltinType(name);

        // Lookup in DeclContext
        NamedDecl* decl = CurrentDeclContext->lookup(name);
        if (isa<TypedefDecl>(decl)) {
            TypedefDecl* TD = cast<TypedefDecl>(decl);
            Type* Ty = Context.getTypeDeclType(TD);
        }
        else {
            // Error or unresolved
        }
        if (decl) {
            // For toy model, return resolved type with the name of decl
            return Context.create<ResolvedType>(decl->getName());
        }
        else {
            // Unknown type - create unresolved placeholder
            return Context.create<UnresolvedType>(name);
        }
    }
public:
    explicit Sema(ASTContext& c, DeclContext* CurDeclContext) : Context(c), CurrentDeclContext(CurDeclContext) {}

    IdentifierInfo* getIdentifierInfo(const std::string& name) {
        return Context.getIdentifierTable().get(name);
    }

    Type* getTypeForIdentifier(IdentifierInfo* id) {
        return getTypeName(id->getName()); // You already have getTypeName
    }
    Type* ActOnTypeName(const std::string& name) {
        // First check if builtin
        if (name == "int" || name == "double") return ActOnBuiltinType(name);

        // Lookup in DeclContext
        NamedDecl* decl = CurrentDeclContext->lookup(name);
        if (decl) {
            // For toy model, return resolved type with the name of decl
            return Context.create<ResolvedType>(decl->getName());
        }
        else {
            // Unknown type - create unresolved placeholder
            return Context.create<UnresolvedType>(name);
        }
    }

    Expr* ActOnIdentifier(const std::string& name) {
        NamedDecl* decl = lookupName(name); // lookup inside Sema
        if (decl)
            return Context.create<DeclRefExpr>(decl);
        else
            return Context.create<UnresolvedNameExpr>(name);
    }


    Decl* ActOnVarDecl(Type* type, IdentifierInfo* id, Expr* init = nullptr) {
        VarDecl* VD = Context.create<VarDecl>(id, type, init);
        CurrentDeclContext->addDecl(VD);
        return VD;
    }

    Decl* ActOnTypedefDecl(Type* type, IdentifierInfo* id) {
        TypedefDecl* TD = Context.create<TypedefDecl>(id, type);
        CurrentDeclContext->addDecl(TD);
        return TD;
    }

    PlusExpr* ActOnPlusExpr(Expr* lhs, Expr* rhs) {
        // Perform semantic checks (omitted here)
        return Context.create<PlusExpr>(lhs, rhs);
    }

    MultiplyExpr* ActOnMultiplyExpr(Expr* lhs, Expr* rhs) {
        return Context.create<MultiplyExpr>(lhs, rhs);
    }

    IntLiteralExpr* ActOnIntLiteral(int value) {
        return Context.create<IntLiteralExpr>(value);
    }

    DoubleLiteralExpr* ActOnDoubleLiteral(double value) {
        return Context.create<DoubleLiteralExpr>(value);
    }

    BuiltinType* ActOnBuiltinType(const std::string& name) {
        if (name == "int") 
            return Context.create<BuiltinType>(BuiltinType::BuiltinKind::Int);
        if (name == "double") 
            return Context.create<BuiltinType>(BuiltinType::BuiltinKind::Double);
        throw std::runtime_error("Unknown type: " + name);
    }

    UnresolvedType* ActOnUnresolvedType(const std::string n) {
        return Context.create<UnresolvedType>(n);
    }
};