#include "../src/Node/Node.h"
#include "../src/Lexer/Lexer.h"
#include "../src/Lexer/Source.h"
#include <iostream> // For std::cout, std::cerr
#include <fstream>  // For std::ifstream
#include <sstream>  // For std::stringstream
#include <string>   // For std::string
#include <iostream>
#include <string_view>

// Yep, i used deepseek to write tests, lol

std::string readFileIntoString(const std::string& filePath) {
    std::ifstream fileStream(filePath);

    if (!fileStream.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return ""; // Return an empty string on error
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf(); // Read the entire file buffer into the stringstream
    fileStream.close(); // Close the file stream

    return buffer.str(); // Return the content as a std::string
}

void cout_all_tokens(SIML::Lexer& lexer);

int main() {
    std::string str = readFileIntoString("tests/main.siml");
    auto source = SIML::Source(str);
    auto lexer = SIML::Lexer(source);


    auto node = SIML::NodeObject::parseAsGlobalNode(lexer);
    if (node.hasError()) {
        std::cout << "Error: " << node.error().m_message;
        return 0;
    }
    node->write(std::cout, 2);

    // cout_all_tokens(lexer);

    return 0;
}

void cout_all_tokens(SIML::Lexer& lexer) {
    while (auto token = lexer.peek()) {
        std::string_view test;
        switch (*token) {
            case SIML::TokenType::DOT: 
                std::cout << "." << std::endl;
                lexer.consume_next();
                break;
            case SIML::TokenType::BLOCK_CLOSE:
                std::cout << "}" << std::endl;
                lexer.consume_next();
                break;
            case SIML::TokenType::IDENT:
                test = lexer.getNextIdent();
                std::cout << test << std::endl;
                break;
            case SIML::TokenType::STRING:
                std::cout << lexer.getNextString().value() << std::endl;
                break;
            case SIML::TokenType::NUMBER:
                std::cout << lexer.getNextNumber() << std::endl;
                break;
            case SIML::TokenType::BLOCK_OPEN:
                std::cout << "{" << std::endl;
                lexer.consume_next();
                break;
            case SIML::TokenType::EXPR_END:
                std::cout << ";" << std::endl;
                lexer.consume_next();
                break;
            case SIML::TokenType::TEXT_BLOCK_OPEN:
                std::cout << "[" << std::endl;
                lexer.consume_next();
                break;
            case SIML::TokenType::TEXT_BLOCK_CLOSE:
                std::cout << "]" << std::endl;
                lexer.consume_next();
                break;
            case SIML::TokenType::DOUBLE_DOT:
                std::cout << ":" << std::endl;
                lexer.consume_next();
                break;
        }
    }
}