#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>

#include "Parser.hpp"
#include "Sema.hpp"
// === Example Usage ===

int main() {
    ASTContext ctx;
    DeclContext dc;

    Sema sema(ctx, &dc);

    std::string expr_source_code = "{int foo = 3; int x = (42+3.14)*foo; typedef int A; A y;}";
    Preprocessor pp(expr_source_code); //lexer is created implictly for now
    Parser parser(pp, sema);

    CompoundStmt* ASTofBlock = parser.parseCompoundStatement();
    ASTofBlock->print(std::cout); std::cout << "\n";
    return 0;

}
