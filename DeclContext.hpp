#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Basic.hpp"
// === Decl ===
class NamedDecl {
    std::string Name;

public:
    explicit NamedDecl(std::string N) : Name(std::move(N)) {}
    const std::string& getName() const { return Name; }
    virtual ~NamedDecl() = default;
};

// === StoredDeclsMap ===
using StoredDeclsMap = std::unordered_map<std::string, NamedDecl*>;

// === DeclContext ===
class DeclContext {
    std::unique_ptr<StoredDeclsMap> LookupPtr;

public:
    void addDecl(NamedDecl* D) {
        if (!LookupPtr)
            LookupPtr = std::make_unique<StoredDeclsMap>();
        (*LookupPtr)[D->getName()] = D;
    }

    NamedDecl* lookup(const std::string& name) const {
        if (!LookupPtr) return nullptr;
        auto it = LookupPtr->find(name);
        return it != LookupPtr->end() ? it->second : nullptr;
    }
};