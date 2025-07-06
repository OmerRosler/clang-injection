#pragma once
#include "AST/ASTType.hpp"
#include "AST/ASTExpr.hpp"
#include "AST/ASTContext.hpp"
#include "AST/DeclContext.hpp"

#include "Preprocessor/Preprocessor.hpp"

class Sema {
    // === Core State ===
    ASTContext& Context;               // Owns the AST nodes
    //Scope* GlobalLexicalScope;
    //DeclContext* GlobalSemanticContext; // Typically a TranslationUnitDecl
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
    static DeclContext* getParentDeclContext(DeclContext* DC);

public:
    class ParseScope;
    class ContextRAII;
    friend class ParseScope;
    friend class ContextRAII;

    explicit Sema(ASTContext& c, DeclContext* CurDeclContext) : Context(c), CurrentDeclContext(CurDeclContext) {}
    
    // TODO: Should we implement a logic here? or is it done by the ASTContext
    virtual ~Sema() = default;
    DeclContext* GetCurrentDeclContext() const
    {
        return CurrentDeclContext;
    }
    

    // --- Scope Management ---
    // Pushes a new lexical scope onto the stack.
    void PushScope(unsigned flags);
    void PopScope();
    Scope* getCurrentScope() const { return CurScope; }

    void PushDeclContext(DeclContext* DC);
    void PopDeclContext();

    void PushOnScopeChains(NamedDecl* D, Scope* S);

    // lookup
    NamedDecl* LookupSingleName(DeclContext* DC, IdentifierInfo* II);
    //unqualified name lookup (no ADL)
    bool LookupName(LookupResult& R, Scope* S, bool AllowBuiltinCreation = false);
    bool LookupName(LookupResult& R, bool AllowBuiltinCreation = false)
    {
        return LookupName(R, CurScope, AllowBuiltinCreation);
    }
    bool LookupQualifiedName(LookupResult& R, DeclContext* DC);
    bool LookupParsedName(LookupResult& R, Scope* S, /* CXXScopeSpec* SS stubbed */ void* = nullptr);
    bool LookupNameOrErr(LookupResult& R, Scope* S);
    
    // queries
    bool isTypeName(IdentifierInfo* II, Scope* S);

    template<typename... Args>
    DeclContext* determine_owner_context(Args&&... args)
    {
        //TODO: Handle complex logic here for out of line definitions
        return GetCurrentDeclContext();
    }

    // declarations actions
    VarDecl* ActOnVarDecl(Scope* S,
        Type* type, IdentifierInfo* II, Expr* init = nullptr);
    TypedefDecl* ActOnTypedefDecl(Scope* S, Type* type, IdentifierInfo* II);

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