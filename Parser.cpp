#include "Parser.hpp"
// ------------------ Expression Parser ------------------
std::unique_ptr<ExprNode> parse_expr(TokenStream& ts) {
    std::string tok = ts.peek();
    //TODO: Add ctors
    if (tok == "A") {
        ts.advance();
        auto operand = parse_expr(ts);
        return std::make_unique<ExprNode>(
            std::make_unique<ANode>(std::move(operand)));
    }

    if (tok == "B") {
        ts.advance();
        auto operand = parse_expr(ts);
        return std::make_unique<ExprNode>(
            std::make_unique<BNode>(std::move(operand)));
    }

    // default: unresolved identifier
    ts.advance();
    return std::make_unique<ExprNode>(
        std::make_unique<UnresolvedRef>(tok));
}