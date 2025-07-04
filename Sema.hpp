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
    Scope* CurScope = nullptr;
    //std::vector<DeclContext*> ContextStack;

    // === Symbol Tables ===
    DeclContext* CurrentDeclContext;         // E.g. current function, class, etc.

    bool LookupADL(LookupResult& R, Scope* S);

public:
    explicit Sema(ASTContext& c, DeclContext* CurDeclContext) : Context(c), CurrentDeclContext(CurDeclContext) {}
    
    DeclContext* GetCurrentDeclContext() const
    {
        return CurrentDeclContext;
    }

    // lookup
    NamedDecl* LookupSingleName(DeclContext* DC, IdentifierInfo* II);
    //unqualified name lookup (no ADL)
    bool LookupName(LookupResult& R, Scope* S, bool AllowBuiltinCreation = false);
    bool LookupQualifiedName(LookupResult& R, DeclContext* DC);
    bool LookupParsedName(LookupResult& R, Scope* S, /* CXXScopeSpec* SS stubbed */ void* = nullptr);
    bool LookupNameOrErr(LookupResult& R, Scope* S);
    
    // queries
    bool isTypeName(IdentifierInfo* II, Scope* S);

    // declarations actions
    VarDecl* ActOnVarDecl(Scope* S, DeclContext* DC,
        Type* type, IdentifierInfo* II, Expr* init = nullptr);
    TypedefDecl* ActOnTypedefDecl(Scope* S, DeclContext* DC, Type* type, IdentifierInfo* II);

    // expressions actions
    Expr* ActOnIdentifierExpr(Scope* S, DeclContext* DC, IdentifierInfo* II);
    Type* ActOnIdentifierType(Scope* S, DeclContext* DC, IdentifierInfo* II);

    PlusExpr* ActOnPlusExpr(Expr* lhs, Expr* rhs);
    MultiplyExpr* ActOnMultiplyExpr(Expr* lhs, Expr* rhs);
    IntLiteralExpr* ActOnIntLiteral(int value);
    DoubleLiteralExpr* ActOnDoubleLiteral(double value);
    ParenExpr* ActOnParenExpr(Expr* SubExpr);
    BuiltinType* ActOnBuiltinType(BuiltinType::BuiltinKind);
    //UnresolvedType* ActOnUnresolvedType(IdentifierInfo* II);

    //statements actions
    DeclStmt* ActOnDeclStmt(Decl* D);
    CompoundStmt* ActOnCompoundStmt(std::vector<Stmt*> stmts);
};