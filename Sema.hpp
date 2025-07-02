#pragma once
#include "ASTType.hpp"
#include "ASTExpr.hpp"
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


// === Simulated ASTContext Allocator ===
class ASTContext {
    std::list<void*> allocations;

public:
    template<typename T, typename... Args>
    T* create(Args&&... args) {
        void* mem = std::malloc(sizeof(T));
        T* obj = new (mem) T(std::forward<Args>(args)...);
        allocations.push_back(mem);
        return obj;
    }

    ~ASTContext() {
        for (void* ptr : allocations) std::free(ptr); // No destructor calls (matches clang style)
    }
};

class Sema {
    ASTContext& ctx;

public:
    explicit Sema(ASTContext& c) : ctx(c) {}

    PlusExpr* actOnPlusExpr(Expr* lhs, Expr* rhs) {
        // Perform semantic checks (omitted here)
        return ctx.create<PlusExpr>(lhs, rhs);
    }

    MultiplyExpr* actOnMultiplyExpr(Expr* lhs, Expr* rhs) {
        return ctx.create<MultiplyExpr>(lhs, rhs);
    }

    IntLiteralExpr* ActOnIntLiteral(int value) {
        return ctx.create<IntLiteralExpr>(value);
    }

    DoubleLiteralExpr* ActOnDoubleLiteral(double value) {
        return ctx.create<DoubleLiteralExpr>(value);
    }

    UnknownNameExpr* actOnUnknownName(const std::string& name) {
        return ctx.create<UnknownNameExpr>(name);
    }

    BuiltinType* actOnBuiltinType(BuiltinType::BuiltinKind kind) {
        return ctx.create<BuiltinType>(kind);
    }

    UnresolvedType* actOnUnresolvedType(const std::string n) {
        return ctx.create<UnresolvedType>(n);
    }
};