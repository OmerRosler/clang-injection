#pragma once
#include <list>
#include "Basic.hpp"
#include "IdentifierTable.hpp"

// === Simulated ASTContext Allocator ===
class ASTContext {
    std::list<void*> allocations;

    IdentifierTable Idents;

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

    IdentifierTable& getIdentifierTable() { return Idents; }
};
