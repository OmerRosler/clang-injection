#pragma once
#include <vector>
#include <string>
#include "Basic.hpp"
enum class LookupResultKind {
    NotFound,
    Found,
    FoundOverloaded,
    Ambiguous  // We'll skip implementing ambiguity handling for now
};

enum class LookupNameKind {
    LookupOrdinaryName,
    LookupTagName,
    LookupMemberName,
    LookupOperatorName,
    LookupLabel,
    LookupNestedNameSpecifierName,
    LookupNamespaceName,
    LookupUsingDeclName,
    LookupObjCProtocolName,
    LookupOMPReductionName,
    LookupOMPMapperName,
    LookupOMPLinearName,
    LookupOMPUseDeclName,
    LookupADL
};
class LookupResult {
private:
    Sema& SemaRef;
    std::vector<NamedDecl*> Decls;
    LookupResultKind Result = LookupResultKind::NotFound;
    IdentifierInfo* Name;
    LookupNameKind LK;

public:
    LookupResult(Sema& SemaRef, 
        IdentifierInfo* Name,
        LookupNameKind LK = LookupNameKind::LookupOrdinaryName) : 
        SemaRef(SemaRef), Name(Name), LK(LK) {}

    IdentifierInfo* getLookupName() const { return Name; }
    LookupNameKind getLookupKind() const { return LK; }

    void addDecl(NamedDecl* D) {
        if (!D) { 
            return; 
        }
        if (Result == LookupResultKind::NotFound) {
            Result = LookupResultKind::Found;
        }
        else { 
            Result = LookupResultKind::FoundOverloaded;
        } // Simplified ambiguity detection
        Decls.push_back(D);
    }
    void clear()
    {
        Decls.clear();
        Result = LookupResultKind::NotFound;
    }
    void addDecl(DeclContext::lookup_data_type overload_set)
    {
        for (NamedDecl* ND : overload_set)
        {
            addDecl(ND);
        }
    }

    [[nodiscard]] bool empty() const { return Decls.empty(); }

    LookupResultKind getResultKind() const { return Result; }

    NamedDecl* getFoundDecl() const {
        return Decls.empty() ? nullptr : Decls.front(); // Simplified
    }

    void setAmbiguous() { Result = LookupResultKind::Ambiguous; }

    bool isAmbiguous() const { return (Result == LookupResultKind::Ambiguous); }
    bool isFound() const { return (Result == LookupResultKind::Found); }
    bool isNotFound() const { return (Result == LookupResultKind::NotFound); }
    bool isOverloaded() const { return (Result == LookupResultKind::FoundOverloaded); }

    bool isSingleResult() const { return (Decls.size() == 1); }

    /*void printDiagnostics() const {
        if (Result == LookupResultKind::NotFound) std::cerr << "error: name not found\n";
        else if (Result == LookupResultKind::Ambiguous) std::cerr << "error: ambiguous name\n";
    }*/

    
};