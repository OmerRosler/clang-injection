#pragma once
#include "ASTType.hpp"
#include "ASTExpr.hpp"

#include "ASTContext.hpp"
#include "Preprocessor.hpp"
// === Phase 2 Resolver ===
struct Phase2Resolver {
public:
    // simple type table mapping names to Expr* (pretend these are fully resolved Exprs)
    std::unordered_map<std::string, Type*> typeTable;
    std::unordered_map<std::string, Expr*> exprTable;

    void resolveType(Type& type) {
        switch (type.typeClass) {
        case Type::TypeClass::Builtin:
            // nothing to resolve
            break;
        case Type::TypeClass::Unresolved: {
            auto& ut = static_cast<UnresolvedType&>(type);
            if (auto it = typeTable.find(ut.name); it != typeTable.end()) {
                ut.resolved = it->second;
            }
            else {
                std::cerr << "Unresolved type: " << ut.name << "\n";
            }
            break;
        }
        }
    }

    //void resolve(Expr* e) {
    //    if (!e) return;

    //    if (isa<UnresolvedTypeExpr>(e)) {
    //        auto* ut = cast<UnresolvedTypeExpr>(e);
    //        auto it = typeTable.find(ut->name);
    //        if (it != typeTable.end()) {
    //            ut->resolved = it->second;
    //        }
    //        else {
    //            std::cerr << "Unresolved type: " << ut->name << "\n";
    //        }
    //    }
    //    // extend here for recursive walk if your Exprs had children
    //}
};


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
    //DeclContext* CurContext;         // E.g. current function, class, etc.

public:
    explicit Sema(ASTContext& c) : Context(c) {}

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

    UnknownNameExpr* ActOnUnknownName(const std::string& name) {
        return Context.create<UnknownNameExpr>(name);
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