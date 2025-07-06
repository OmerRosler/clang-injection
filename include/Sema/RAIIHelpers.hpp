#pragma once
#include "Sema/Sema.hpp"
#include "Sema/Scope.hpp"
#include "AST/DeclContext.hpp"

class Sema::ParseScope {
    Sema& S; // Reference to the Sema instance
public:
    ParseScope(Sema& sema, unsigned flags = ScopeFlags::CompoundScope) : S(sema) {
        S.PushScope(flags); // Call Sema's method to push a new Scope
        std::cout << "  [Sema::ParseScope]: Pushed new lexical scope (flags: " << flags << ")" << std::endl;
    }
    ~ParseScope() {
        S.PopScope(); // Call Sema's method to pop the Scope
        std::cout << "  [Sema::ParseScope]: Popped lexical scope" << std::endl;
    }
    Scope* getScope() const
    {
        return S.CurScope;
    }
    ParseScope(const ParseScope&) = delete; // Disable copy/move
    ParseScope& operator=(const ParseScope&) = delete;
};