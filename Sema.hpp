#pragma once
#include "ASTType.hpp"
#include "ASTExpr.hpp"
// === Phase 2 Resolver ===
struct Phase2Resolver {
public:
    // simple type table mapping names to Expr* (pretend these are fully resolved Exprs)
    std::unordered_map<std::string, Expr*> typeTable;

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

    void resolve(Expr* e) {
        if (!e) return;

        if (isa<UnresolvedTypeExpr>(e)) {
            auto* ut = cast<UnresolvedTypeExpr>(e);
            auto it = typeTable.find(ut->name);
            if (it != typeTable.end()) {
                ut->resolved = it->second;
            }
            else {
                std::cerr << "Unresolved type: " << ut->name << "\n";
            }
        }
        // extend here for recursive walk if your Exprs had children
    }
    }
};