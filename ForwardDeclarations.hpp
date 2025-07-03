#pragma once


// Forward declarations

//Preprocessor
class Preprocessor;

class IdentifierInfo;
class IdentifierTable;

//Lexer
class Lexer;

struct Token;
class Decl;

//Parser
class Parser;

class Scope;

// Sema
class Sema;


struct LookupResult;

//AST

// Types
class Type;

struct BuiltinType;
struct ResolvedType;
struct UnresolvedType;

// Statements
class Stmt;

struct Expr;

struct IntLiteralExpr;
struct DoubleLiteralExpr;

struct DeclRefExpr;
struct UnresolvedNameExpr;

struct PlusExpr;
struct MultiplyExpr;

//Declarations
class Decl;

struct NamedDecl;
class DeclContext;

struct VarDecl;
struct TypedefDecl;

