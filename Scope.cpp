#include "Scope.hpp"
#include "Parser.hpp"
#include "ASTDecl.hpp"

NamedDecl* Scope::lookup(IdentifierInfo* II) const {
    auto find_result = DeclsInScope.find(II);
    if (find_result != DeclsInScope.end())
    {
        return find_result->second;
    }
    return nullptr;
}