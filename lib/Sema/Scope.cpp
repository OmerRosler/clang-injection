#include "Sema/Scope.hpp"
#include "Parser/Parser.hpp"
#include "AST/ASTDecl.hpp"
#include <ranges>
#include <cassert>

NamedDecl* Scope::lookupLocal(IdentifierInfo* II) const {
    auto it = Locals.find(II);
    if (it != Locals.end())
    {
        return it->second;
    }
    return nullptr;
}

void Scope::addDecl(NamedDecl* NewDecl) {
    Locals[NewDecl->getIdentifier()] = NewDecl;
    std::cout << "  DEBUG: Scope (Addr: " << this << ") added '"
        << NewDecl->getName() << "' to its LocalDeclMap." << std::endl;
}


void Scope::dump() const {
    std::cout << "Scope (Addr: " << this << ", Parent: " << Parent << ") Dump:" << std::endl;
    for (const auto& pair : Locals) {
        std::cout << "    Local: " << pair.first->getName() << " -> (Addr: " << pair.second << ")" << std::endl;
    }
    std::cout << "End Scope Dump." << std::endl;
}