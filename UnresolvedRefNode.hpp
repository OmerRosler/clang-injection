#pragma once
#include "Node.hpp"

// UnresolvedRef node: a leaf node that is unresolved at phase 1
struct UnresolvedRef : Node {
    std::string name;

    void phase1(TokenStream& ts) override {
        name = ts.peek();
        ts.advance();
        // Nothing else to do at phase 1
    }
    void phase2() override {
        // Here symbol resolution would happen in a real parser
    }
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "UnresolvedRef(" << name << ")\n";
    }
};