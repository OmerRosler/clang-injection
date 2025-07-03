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

    std::string expr_source_code = "int foo = 3; int x = 42+3.14*foo; typedef int A; A y;";
    Preprocessor pp(expr_source_code); //lexer is created implictly for now
    Parser parser(pp, sema);
    std::array<Decl*, 4> decls_AST{};
    
    decls_AST[0]= parser.parseVarDecl();
    decls_AST[1]= parser.parseVarDecl();
    decls_AST[2]= parser.parseTypedef();
    decls_AST[3]= parser.parseVarDecl();

    for (auto* decl : decls_AST)
    {
        decl->print(std::cout); std::cout << "\n";
    }

    return 0;

}
