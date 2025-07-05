#include "DeclContext.hpp"
#include "LookupResult.hpp"
#include "ASTDecl.hpp"

bool DeclContext::lookupDirectMember(IdentifierInfo* II, LookupResult& R) const {
    if (!II) return false;
    auto it = Lookup.find(II);
    if (it != Lookup.end()) {

        R.addDecl(it-> second);
        return true;
    }
    return false;
}

NamedDecl* DeclContext::lookupFirstDirectMember(IdentifierInfo* II) const {
    if (!II) return nullptr;
    auto it = Lookup.find(II);
    if (it != Lookup.end()) {
        return it->second.front();
    }
    return nullptr;
}


DeclContext::lookup_data_type DeclContext::lookupDirectMember(IdentifierInfo* II) const {
    if (!II) return lookup_data_type{};
    auto it = Lookup.find(II);
    if (it != Lookup.end()) {
        return it->second;
    }
    return lookup_data_type{};
}


// Add a declaration into this context
bool DeclContext::addDecl(NamedDecl* NewDecl) {
    if (!NewDecl) return false;
    Lookup[NewDecl->getIdentifier()].push_back(NewDecl);
    return true;
    //IdentifierInfo* DeclName = NewDecl->getIdentifier();

    //NamedDecl* ExistingDecl = lookupDirectMember(NewDecl->getIdentifier());
    //if (ExistingDecl) {
    //    // --- Redeclaration Check Logic (Simplified, similar to Scope::AddDecl) ---
    //    bool existingIsFunc = ExistingDecl->isFunctionDecl();
    //    bool newIsFunc = NewDecl->isFunctionDecl();
    //    bool existingIsTypedef = ExistingDecl->isTypedefDecl();
    //    bool newIsTypedef = NewDecl->isTypedefDecl();
    //    bool existingIsVar = ExistingDecl->isVarDecl();
    //    bool newIsVar = NewDecl->isVarDecl();
    //    bool existingIsNS = ExistingDecl->isNamespaceDecl();
    //    bool newIsNS = NewDecl->isNamespaceDecl();

    //    // Conflict: Different kind of symbol
    //    if ((existingIsFunc != newIsFunc) ||
    //        (existingIsTypedef != newIsTypedef && !existingIsFunc && !newIsFunc) ||
    //        (existingIsVar != newIsVar && !existingIsFunc && !newIsFunc && !existingIsTypedef && !newIsTypedef) ||
    //        (existingIsNS != newIsNS)) { // Namespace specific check
    //        //Diags->Report(NewDecl->getLocation(), DiagnosticsEngine::Error,
    //        //    "redeclaration of '" + DeclName + "' as different kind of symbol within DeclContext");
    //        return false;
    //    }

    //    // Conflict: Same kind redefinition (simplified)
    //    if (NewDecl->getKind() == ExistingDecl->getKind()) {
    //        if (NewDecl->isTypedefDecl()) {
    //            TypedefDecl* NewTD = static_cast<TypedefDecl*>(NewDecl);
    //            TypedefDecl* ExistingTD = static_cast<TypedefDecl*>(ExistingDecl);
    //            if (!(NewTD->getAliasedType() == ExistingTD->getAliasedType())) {
    //                //Diags->Report(NewDecl->getLocation(), DiagnosticsEngine::Error, "redefinition of typedef '" + DeclName + "' with different types in DeclContext");
    //                return false;
    //            }
    //            return true; // Valid re-typedef
    //        }
    //        else if (NewDecl->isVarDecl() || NewDecl->isNamespaceDecl()) {
    //            //Diags->Report(NewDecl->getLocation(), DiagnosticsEngine::Error, "redefinition of " + (NewDecl->isVarDecl() ? "variable" : "namespace") + " '" + DeclName + "' in DeclContext");
    //            return false;
    //        }
    //        else if (NewDecl->isFunctionDecl()) {
    //            // For simplicity, assume redefinition if not an overload.
    //            //Diags->Report(NewDecl->getLocation(), DiagnosticsEngine::Error, "redefinition of function '" + DeclName + "' in DeclContext (simple model)");
    //            return false;
    //        }
    //    }
    //    //Diags->Report(NewDecl->getLocation(), DiagnosticsEngine::Error, "unhandled redeclaration of '" + DeclName + "' in DeclContext");
    //    return false;
    //}

    //Lookup[DeclName].push_back(NewDecl); // Add to members map if no conflict
    //return true;
}
