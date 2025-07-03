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
    NamedDecl* lookupName(IdentifierInfo* II) {
        return CurrentDeclContext->lookup(II);
    }

public:
    Type* getTypeName(IdentifierInfo* II) {
        NamedDecl* ND = CurrentDeclContext->lookup(II);
        if (!ND)
            return nullptr; // unknown type

        // Is it actually a typedef?
        auto* TD = dyn_cast<TypedefDecl>(ND);
        if (!TD)
            return nullptr; // not a type declaration

        return Context.getTypeDeclType(TD);
    }

    DeclContext* GetCurrentDeclContext() const
    {
        return CurrentDeclContext;
    }

    explicit Sema(ASTContext& c, DeclContext* CurDeclContext) : Context(c), CurrentDeclContext(CurDeclContext) {}

    /*IdentifierInfo* getIdentifierInfo(const std::string& name) {
        return Context.getIdentifierTable().lookup(name);
    }*/

    Type* getTypeForIdentifier(IdentifierInfo* II) {
        return getTypeName(II); // You already have getTypeName
    }
    //Type* ActOnTypeName(IdentifierInfo* II);

    Expr* ActOnIdentifier(IdentifierInfo* II);


    VarDecl* ActOnVarDecl(Type* type, IdentifierInfo* id, Expr* init = nullptr);
    VarDecl* ActOnVarDecl(Type* type, IdentifierInfo* id, DeclContext* DC);
    VarDecl* ActOnVarDecl(Type* type, IdentifierInfo* id, Expr* init, DeclContext* DC);

    TypedefDecl* ActOnTypedefDecl(Type* type, IdentifierInfo* id, DeclContext* DC);

    PlusExpr* ActOnPlusExpr(Expr* lhs, Expr* rhs);
    MultiplyExpr* ActOnMultiplyExpr(Expr* lhs, Expr* rhs);

    IntLiteralExpr* ActOnIntLiteral(int value);
    DoubleLiteralExpr* ActOnDoubleLiteral(double value);

    ParenExpr* ActOnParenExpr(Expr* SubExpr);




    BuiltinType* ActOnBuiltinType(BuiltinType::BuiltinKind);
    UnresolvedType* ActOnUnresolvedType(IdentifierInfo* II);
};