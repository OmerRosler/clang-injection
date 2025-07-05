#include "Sema/Sema.hpp"
#include "Sema/Scope.hpp"
#include "AST/ASTDecl.hpp"
#include <cassert>
bool Sema::PushOnScopeAndInsertRedeclaration(NamedDecl* NewDecl, Scope* S, DeclContext* DC) {
    std::string Name = NewDecl->getName();
    bool IsError = false; // Tracks if a semantic error was detected and reported

    // --- 1. Lexical Scope Registration and Initial Semantic Check ---
    // Delegate to Scope to handle its internal map and intrusive list.
    // Scope returns the previously existing declaration in *this lexical scope*.
    NamedDecl* existingInLexicalScope = S->addDecl(NewDecl);

    // Sema now performs semantic validation based on the result from Scope.
    if (existingInLexicalScope) {
        // Rule: Local variables cannot be redeclared within the same lexical scope.
        if (NewDecl->getKind() == Decl::Kind::Var &&
            existingInLexicalScope->getKind() == Decl::Kind::Var) {
            //Diag.Report(Loc, "error: redeclaration of local variable '" + Name + "' in this scope.");
            NewDecl->setInvalid(true);
            IsError = true;
        }
        // TODO: More complex checks for functions (overloading vs. redefinition)
        // in lexical scope can go here if needed. For now, we allow them to stack.
        else {
            std::cout << "Note: '" << Name << "' found existing in lexical scope (potential redecl/overload)." << std::endl;
        }
    }


    // --- 2. Semantic DeclContext Insertion and Comprehensive Semantic Validation ---
    // Determine if this declaration should be added to the DC's Members map.
    // Local variables (VarDecl within a function) are generally NOT added here,
    // as DeclContext::Members is for semantically owned members.
    bool addToDeclContextMembers = false;
    if (NewDecl->getKind() != Decl::Kind::Var) {
        // Functions, classes, namespaces, etc., are always semantically owned.
        addToDeclContextMembers = true;
    }
    else { // It's a VarDecl
        // Only add VarDecls to DeclContext::Members if they are global, namespace,
        // or static members of a class.
        //if (DC->getDeclContextKind() == DeclContext::DCK_TranslationUnit ||
        //    DC->getDeclContextKind() == DeclContext::DCK_Namespace ||
        //    DC->getDeclContextKind() == DeclContext::DCK_Record) {
        //    // For simplicity, assume all VarDecls here are added.
        //    // In real Clang, you'd check storage duration (e.g., 'static', 'extern').
        //    addToDeclContextMembers = true;
        //}
        assert(true, "TODO: Conditional adding statics");
    }

    if (addToDeclContextMembers) {
        // Delegate to DeclContext to handle its internal map and intrusive list.
        // DeclContext returns the previously existing declaration in *this semantic context*.
        NamedDecl* existingInDC = DC->addDecl(NewDecl);

        // Sema now performs comprehensive semantic validation based on the result from DeclContext.
        if (existingInDC) {
            // --- Comprehensive Semantic Redeclaration/Overload Checks ---
            if (NewDecl->getKind() == Decl::Kind::Function && existingInDC->getKind() == Decl::Kind::Function) {
                // This is where Clang performs deep signature matching for overload resolution.
                // For our toy model, just print a note.
                std::cout << "Note: DeclContext member function '" << Name << "' redeclared/overloaded." << std::endl;
            }
            else if (NewDecl->getKind() != existingInDC->getKind()) {
                // Rule: Cannot redeclare a symbol as a different kind.
                //Diag.Report(Loc, "error: redeclaration of '" + Name + "' as a different kind of symbol in DeclContext.");
                NewDecl->setInvalid(true);
                IsError = true;
            }
            else {
                // Rule: Re-definition error for non-overloadable entities of same kind.
                // E.g., int x; int x; at global scope. Or re-defining a non-inline function.
                //Diag.Report(Loc, "error: redefinition of '" + Name + "' in DeclContext.");
                NewDecl->setInvalid(true);
                IsError = true;
            }
        }
    }

    // Return true if the declaration was processed, even if errors were found.
    // False would mean a fundamental failure that prevents its AST node from forming.
    return !IsError;
}

VarDecl* Sema::ActOnVarDecl(Scope* S,
    Type* Ty, IdentifierInfo* II, Expr* Init) {
    DeclContext* new_owner = determine_owner_context(Ty, II, Init);
    // 1. Create the Decl AST node. Sema owns this memory.
    VarDecl* VD = Context.create<VarDecl>(II, new_owner, Ty, Init);

    // 2. Push it onto scopes and perform all validation.
    if (!PushOnScopeAndInsertRedeclaration(VD, S, new_owner)) {
        // The declaration was invalid, VD is already marked as such.
        // Further handling for invalid decls (e.g., skip code generation for it).
        //std::cout << "ActOnVarDecl: Error during declaration of '" << NameInfo->getName() << "'" << std::endl;
        throw std::runtime_error("Redeclaration error");
    }
    return VD;
}


TypedefDecl* Sema::ActOnTypedefDecl(Scope* S, 
    Type* Ty, IdentifierInfo* II) {
    DeclContext* new_owner = determine_owner_context(Ty, II);
    TypedefDecl* TD = Context.create<TypedefDecl>(II, new_owner, Ty);

    // 2. Push it onto scopes and perform all validation.
    if (!PushOnScopeAndInsertRedeclaration(TD, S, new_owner)) {
        // The declaration was invalid, VD is already marked as such.
        // Further handling for invalid decls (e.g., skip code generation for it).
        //std::cout << "ActOnVarDecl: Error during declaration of '" << NameInfo->getName() << "'" << std::endl;
        throw std::runtime_error("Redeclaration error");
    }
    return TD;
}