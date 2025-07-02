#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Parser.hpp"
#include "Sema.hpp"
// === Example Usage ===

int main() {
    ASTContext ctx;
    auto lexer = std::make_unique<Lexer>(std::vector<Token>{
        {TokenKind::IntLiteral, "42"},
        { TokenKind::Plus, "+" },
        { TokenKind::DoubleLiteral, "3.14" },
        { TokenKind::Star, "*" },
        { TokenKind::Identifier, "foo" },
        { TokenKind::EndOfFile, "" }
    });
    Preprocessor pp(std::move(lexer), ctx.getIdentifierTable());
    Sema sema(ctx);
    Parser parser(pp, sema);

    Expr* expr = parser.parseExpr();
    expr->print(std::cout); std::cout << "\n";

    auto typeLexer = std::make_unique<Lexer>(std::vector<Token>{
        {TokenKind::IntKeyword, "int"},
        {TokenKind::Identifier, "T" }});

    Preprocessor typePP(std::move(typeLexer), ctx.getIdentifierTable());
    Parser typeParser(typePP, sema);
    Type* type1 = typeParser.parseType();
    Type* type2 = typeParser.parseType();
    type1->print(std::cout); std::cout << "\n";
    type2->print(std::cout); std::cout << "\n";

    return 0;

}
