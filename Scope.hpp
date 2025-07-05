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
    using decl_storage = std::vector<Decl*>;
    decl_storage DeclsInScope;

    using lookup_result = std::vector <NamedDecl*>;


public:

    using decl_range = decltype(std::views::reverse(std::declval<const decl_storage&>()));

    Scope(Scope* Parent, unsigned Flags)
        : Parent(Parent), Entity(nullptr), Flags(Flags) {}

    void setEntity(DeclContext* DC) { Entity = DC; }
    DeclContext* getEntity() const { return Entity; }

    Scope* getParent() const { return Parent; }

    bool addDecl(NamedDecl* NewDecl);
    lookup_result lookupLocal(IdentifierInfo* II) const;
    NamedDecl* lookupFirstLocal(IdentifierInfo* II) const;

    decl_range decls() const { return std::views::reverse(DeclsInScope); }


};
