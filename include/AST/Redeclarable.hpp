#pragma once
#include "Basic.hpp"

// A CRTP base to manage an intrusive list of redeclrations
template <typename T>
class Redeclarable {
protected: // Change to protected to allow Decl subclasses direct access
    T* FirstDeclPtr; // Points to the first declaration in the chain
    T* PreviousDeclPtr; // Points to the previous declaration in the chain
    T* NextDeclPtr;     // Points to the next declaration in the chain

public:
    T* getFirstDecl() const { return FirstDeclPtr; }
    T* getPreviousDecl() const { return PreviousDeclPtr; }
    T* getNextDecl() const { return NextDeclPtr; }

    void setFirstDecl(T* D) { FirstDeclPtr = D; }
    void setPreviousDecl(T* D) { PreviousDeclPtr = D; }
    void setNextDecl(T* D) { NextDeclPtr = D; }

    // Constructor to initialize a new declaration as the start of its own chain
    Redeclarable() :
        FirstDeclPtr(static_cast<T*>(this)), 
        PreviousDeclPtr(nullptr), 
        NextDeclPtr(nullptr) {}

    // Links 'New' declaration into the chain of 'Existing' declaration.
    // 'New' becomes the new last declaration in the chain.
    // Assumes 'Existing' is a valid member of some chain.
    static void linkDeclarations(T* New, T* Existing) {
        // Propagate the FirstDecl from the existing chain to the new declaration.
        New->setFirstDecl(Existing->getFirstDecl());

        // Find the actual last declaration in the chain rooted at Existing->getFirstDecl()
        T* CurrentLastInChain = Existing->getFirstDecl();
        while (CurrentLastInChain && CurrentLastInChain->getNextDecl() != nullptr) {
            CurrentLastInChain = CurrentLastInChain->getNextDecl();
        }

        // CurrentLastInChain is now the last declaration in the chain.
        // Link New after CurrentLastInChain.
        if (CurrentLastInChain) {
            CurrentLastInChain->setNextDecl(New);
            New->setPreviousDecl(CurrentLastInChain);
        }
        else {
            // This case implies Existing was the only declaration and its FirstDecl was itself.
            // Or, more accurately, if Existing was null or not properly linked,
            // but we assert Existing is a valid member here.
            // So, this else branch should ideally not be hit with proper usage.
        }
        New->setNextDecl(nullptr); // New is the new end of the chain.
    }
};