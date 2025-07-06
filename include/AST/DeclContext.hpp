#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Basic.hpp"


enum class DeclContextKind {
    TranslationUnit, // Global scope (top-level)
    Namespace,       // namespace N { ... }
    Record,          // class/struct/union { ... }
    Function,        // function definition body { ... }
    Block,           // C++11 lambda, Objective-C block, captured by a separate Decl (not used yet)
    // ... more kinds as needed
};

class DeclContext {
    using LookupMap = std::unordered_map<IdentifierInfo*, NamedDecl*>;

    DeclContextKind Kind;
    // Fast lookup map for declarations that are visible to everyone
    LookupMap VisibleMembers;
    // Intrusive List of all declarations owned by this Context
    Decl* FirstDeclInContext;
    Decl* LastDeclInContext;
    DeclContext* ParentDC = nullptr; // Semantic parent
    Decl* const DeclContextDecl;

public:
    DeclContext(DeclContextKind K, Decl* parent):
        Kind(K), FirstDeclInContext(nullptr), 
        LastDeclInContext(nullptr), 
        DeclContextDecl(parent){}

    Decl* getSemanticOwner() const {
        return DeclContextDecl;
    }
    DeclContext* getParent() const;

    //TODO: this won't work for multiple inheritence like FunctionDecl
    static bool classof(const DeclContext* D) { return true; } // base class matches all

    DeclContextKind getDeclContextKind() const { return Kind; }

    // --- Private/Protected internal methods for Sema to call ---
    // Sema calls these to register a Decl to this DeclContext's internal map.
    // This method just performs the insertion; it doesn't decide *if* to insert.
    void addDeclInternal(NamedDecl* D);

    // This method is called by Decl::setDeclContext() internally.
    // It links the Decl into this DeclContext's intrusive list.
    void addDeclToContext(Decl* D);

    // Public API for lookup. Queries this DeclContext's map.
    NamedDecl* lookup(IdentifierInfo* Name);

    // Allow Sema to call addDeclInternal
    friend class Sema; // Grants Sema access to private addDeclInternal

protected:

    const char* getDeclContextKindName() const {
        switch (Kind) {
        case DeclContextKind::TranslationUnit: return "TranslationUnit";
        case DeclContextKind::Namespace: return "Namespace";
        case DeclContextKind::Record: return "Record";
        case DeclContextKind::Function: return "Function";
        case DeclContextKind::Block: return "Block";
        }
        return "Unknown";
    }

};
