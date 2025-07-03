#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>

#include <string_view>
#include <fstream>
#include <sstream>
#include "Parser.hpp"
#include "Sema.hpp"
// === Example Usage ===
std::string file_to_string(const char* path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << path << '\n';
        return std::string{};
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();  // Read entire file into buffer
    return buffer.str();
}
int main() {


    std::string test_file = file_to_string("test_files/compound_block.cpp");
    Preprocessor pp(test_file.data(), test_file.length()); //lexer is created implictly for now


    ASTContext ctx;
    DeclContext dc;
    Sema sema(ctx, &dc);
    Parser parser(pp, sema);

    CompoundStmt* ASTofBlock = parser.parseCompoundStatement();
    ASTofBlock->print(std::cout); std::cout << "\n";
    return 0;

}
