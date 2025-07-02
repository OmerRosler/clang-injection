#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Parser.hpp"
#include "Sema.hpp"
// === Example Usage ===

int main() {
    ASTContext ctx;
    Sema sema(ctx);
    Parser parser({ "int", "+", "double", "*", "foo" }, sema);
    Expr* expr = parser.parseExpr();
    expr->print(std::cout); // prints: (42 + (3.14 * <unknown> foo))
    std::cout << "\n";

    Parser typeParser({ "int" }, sema);
    Type* type = typeParser.parseType();
    type->print(std::cout); // prints: int
    std::cout << "\n";

}
