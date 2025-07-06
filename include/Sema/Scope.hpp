#pragma once
#include <map>
#include <ranges>

#include "Basic.hpp"
enum ScopeFlags {
    FunctionScope = 1 << 0,
    CompoundScope = 1 << 1,
    NamespaceScope = 1 << 2,
    ClassScope = 1 << 3,
};
class Scope {
private:
    Scope* Parent;
    unsigned Flags;
    //TODO: Handle unnamed declarations
    using decl_storage = std::map<IdentifierInfo*, NamedDecl*>;
    decl_storage Locals;
    DeclContext* Entity;

    friend class Sema;

public:

    using decl_range = const decl_storage&;

    Scope(Scope* Parent, unsigned Flags)
        : Parent(Parent), Flags(Flags), Entity(nullptr) {}

    Scope* getParent() const { return Parent; }
    void setParent(Scope* P) { Parent = P; }
    unsigned getFlags() const { return Flags; }

    void addDecl(NamedDecl* NewDecl);
    NamedDecl* lookupLocal(IdentifierInfo* II) const;

    decl_range decls() const { return Locals; }

    void dump() const;

};
