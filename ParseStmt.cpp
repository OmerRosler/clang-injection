#include "Parser.hpp"
#include "ASTStmt.hpp"
#include "Scope.hpp"
#include "Sema.hpp"

Stmt* Parser::parseStatement() {
    if (Tok.is(TokenKind::l_brace)) {
        return parseCompoundStatement();
    }
    else {
        // Expression statement
        Expr * expr = parseExpr();
        ExpectAndConsume(TokenKind::semicolon);
        return expr;
    }
}

Stmt* Parser::parseDeclarationOrStatement(Scope* S) {
    //check for typedef declaration
    if (Tok.is(TokenKind::kw_typedef) || Tok.isKeyword()) {
        Decl* decl = parseDeclaration();
        return Actions.ActOnDeclStmt(decl);
    }
    else if (Tok.is(TokenKind::identifier)) {
        IdentifierInfo* II = Tok.getIdentifierInfo();
        if (Actions.isTypeName(II, S)) {
            return Actions.ActOnDeclStmt(parseDeclaration());
        }
    }
    return parseStatement();
}

CompoundStmt* Parser::parseCompoundStatement() {
	
    ConsumeToken(); // consume '{'

	//TODO: Handle flag interaction with the flags of the current scope 
	// (this is state passing in the parser, finally!)
	// 
	//auto CurScopeFlags = CurrentScope->getFlags();
	//CurScopeFlags |= ScopeFlags::BlockScope;
	unsigned new_scope_flags = static_cast<unsigned>(ScopeFlags::CompoundScope);
    // Enter a new block scope
	Scope CompoundScope(CurrentScope, new_scope_flags);
	CurrentScope = &CompoundScope;

    std::vector<Stmt*> statements;

	// Parse statements...
    while (!Tok.is(TokenKind::r_brace) && Tok.isNot(TokenKind::eof)) {
        auto* stmt = parseDeclarationOrStatement(CurrentScope);
        if (!stmt)
        {
            throw std::runtime_error("Invalid statement");
        }
        statements.push_back(stmt);
    }

	// Exit scope
    ExpectAndConsume(TokenKind::r_brace);
	CurrentScope = CompoundScope.getParent();
    return Actions.ActOnCompoundStmt(std::move(statements));

}
