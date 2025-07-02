#pragma once
#include "ASTType.hpp"
#include "ASTExpr.hpp"

// === Simulated ASTContext Allocator ===
class ASTContext {
    std::list<void*> allocations;

public:
    template<typename T, typename... Args>
    T* create(Args&&... args) {
        void* mem = std::malloc(sizeof(T));
        T* obj = new (mem) T(std::forward<Args>(args)...);
        allocations.push_back(mem);
        return obj;
    }

    ~ASTContext() {
        for (void* ptr : allocations) std::free(ptr); // No destructor calls (matches clang style)
    }
};

// === Parser ===
class Parser {
public:
    std::vector<std::string> tokens;
    // Helper: peek at current token without consuming
    std::string peekToken() const {
        if (index >= tokens.size()) return "";
        return tokens[index];
    }

    // Helper: consume and return current token
    std::string consumeToken() {
        if (index >= tokens.size()) throw std::runtime_error("Unexpected EOF");
        return tokens[index++];
    }

    // Check if token is binary operator (extend as needed)
    bool isBinaryOperator(const std::string& tok) const {
        return tok == "+";
    }
    size_t index = 0;
    ASTContext& ctx;

    Parser(std::vector<std::string> toks, ASTContext& c)
        : tokens(std::move(toks)), ctx(c) {}


    int precedence(const std::string& op) const {
        if (op == "*") return 20;
        if (op == "+") return 10;
        return -1;
    }

    Expr* parsePrimary() {
        if (index >= tokens.size()) throw std::runtime_error("Unexpected EOF");
        const std::string& tok = tokens[index++];

        if (tok == "int") {
            return ctx.create<IntLiteralExpr>(42);
        }
        else if (tok == "double") {
            return ctx.create<DoubleLiteralExpr>(3.14);
        }
        else {
            return ctx.create<UnknownNameExpr>(tok);
        }
    }

    Expr* parseExpr(int minPrec = 0) {
        Expr* lhs = parsePrimary();

        while (index < tokens.size()) {
            const std::string& op = tokens[index];
            int prec = precedence(op);
            if (prec < minPrec) break;

            ++index; // consume operator
            Expr* rhs = parseExpr(prec + 1);

            if (op == "+") {
                lhs = ctx.create<PlusExpr>(lhs, rhs);
            }
            else if (op == "*") {
                lhs = ctx.create<MultiplyExpr>(lhs, rhs);
            }
            else {
                throw std::runtime_error("Unknown binary operator: " + op);
            }
        }

        return lhs;
    }

    Type* parseType() {
        if (index >= tokens.size()) throw std::runtime_error("Unexpected EOF");
        const std::string& tok = tokens[index++];

        if (tok == "int") {
            return ctx.create<BuiltinType>(BuiltinType::BuiltinKind::Int);
        }
        else if (tok == "double") {
            return ctx.create<BuiltinType>(BuiltinType::BuiltinKind::Double);
        }
        else {
            return ctx.create<UnresolvedType>(tok);
        }
}


};
