#pragma once
#include <vector>
#include <unordered_map>
#include <ranges>

#include "Basic.hpp"
enum class ScopeFlags {
    FunctionScope = 1 << 0,
    CompoundScope = 1 << 1,
    NamespaceScope = 1 << 2,
    ClassScope = 1 << 3,
};
class Scope {
private:
    Scope* Parent;
    unsigned Flags;
    DeclContext* Entity; //owner of the scope if exists
    //TODO: Handle unnamed declarations
    using decl_storage = std::unordered_map<IdentifierInfo*, NamedDecl*>;
    decl_storage Locals;


public:

    using decl_range = const decl_storage&;

    Scope(Scope* Parent, unsigned Flags)
        : Parent(Parent), Entity(nullptr), Flags(Flags) {}

    void setEntity(DeclContext* DC) { Entity = DC; }
    DeclContext* getEntity() const { return Entity; }

    Scope* getParent() const { return Parent; }

    NamedDecl* addDecl(NamedDecl* NewDecl);
    NamedDecl* lookupLocal(IdentifierInfo* II) const;

    decl_range decls() const { return Locals; }


};
