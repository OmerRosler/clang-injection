#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Basic.hpp"
#include "ASTDecl.hpp"


// === StoredDeclsMap ===
using StoredDeclsMap = std::unordered_map<IdentifierInfo*, NamedDecl*>;

// === DeclContext ===
class DeclContext {
    std::unique_ptr<StoredDeclsMap> LookupPtr;

public:
    void addDecl(NamedDecl* D) {
        if (!LookupPtr) {
            LookupPtr = std::make_unique<StoredDeclsMap>();
        }
        (*LookupPtr)[D->getIdentifier()] = D;
    }

    NamedDecl* lookup(IdentifierInfo* II) const {
        if (!LookupPtr) return nullptr;
        auto it = LookupPtr->find(II);
        return it != LookupPtr->end() ? it->second : nullptr;
    }

};