#include "Parser.hpp"
#include "Sema.hpp"

Type* Parser::parseType() {
    if (index >= tokens.size()) throw std::runtime_error("Unexpected EOF");
    const std::string& tok = tokens[index++];

    if (tok == "int") {
        return sema.actOnBuiltinType(BuiltinType::BuiltinKind::Int);
    }
    else if (tok == "double") {
        return sema.actOnBuiltinType(BuiltinType::BuiltinKind::Double);
    }
    else {
        return sema.actOnUnresolvedType(tok);
    }
}