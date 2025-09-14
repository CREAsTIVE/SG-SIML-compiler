#include "./Node.h"
#include "../Lexer/Lexer.h"
#include <memory>
#include <utility>
#include <optional>
#include <variant>

Expected<std::unique_ptr<SIML::NodeObject>, SIML::ParseError> parse_next_object(SIML::Lexer& lexer) noexcept;

Expected<std::unique_ptr<SIML::Node>, SIML::ParseError> parse_next_node(SIML::Lexer& lexer) noexcept {
    auto token = lexer.peek();
    if (!token) {return Unexpected(SIML::ParseError("Expected value", lexer));}
    
    if (*token == SIML::TokenType::BLOCK_CLOSE | 
        *token == SIML::TokenType::TEXT_BLOCK_CLOSE |
        *token == SIML::TokenType::EXPR_END
    ) {
        // TODO: Error: unexcpected token
    } else if (*token == SIML::TokenType::BLOCK_OPEN) {
        return expect(parse_next_object(lexer));
    } else if (*token == SIML::TokenType::IDENT) { // ident <value> OR ident;
        auto ident = lexer.get_next_ident();
        auto next = lexer.peek();
        if (next && *next != SIML::TokenType::EXPR_END) {
            auto component = std::make_unique<SIML::NodeComponent>(SIML::NodeComponent {});
            component->name = ident;
            auto value = expect(parse_next_node(lexer));
            component->value = std::move(value);
            return std::move(component);
        } else { // next exists AND it ;
            lexer.consume_next(); // consume ;

            auto ident_node = std::make_unique<SIML::NodeIdent>(SIML::NodeIdent {});
            ident_node->ident = ident;
            return std::move(ident_node);
        }
    } else if (*token == SIML::TokenType::STRING) { // "..."
        auto node = std::make_unique<SIML::NodeString>(SIML::NodeString {});
        node->unescaped_value = expect(lexer.get_next_string());
        
        auto token = lexer.peek();
        if (token && *token == SIML::TokenType::IDENT) {
            node->tag = lexer.get_next_ident();
        }

        // REQUIRED ;
        token = lexer.peek(); lexer.consume_next();
        if (token != SIML::TokenType::EXPR_END) {
            return Unexpected(SIML::ParseError("';' expected", lexer));
        }
        
        return std::move(node);
    } else if (*token == SIML::TokenType::NUMBER | *token == SIML::TokenType::DOT) { // X.X | .X | X.
        auto node = std::make_unique<SIML::NodeNumber>(SIML::NodeNumber {});
        auto token = lexer.peek();

        if (token == SIML::TokenType::NUMBER) {
            node->integer_part = lexer.get_next_number();
        }

        token = lexer.peek();
        if (token && *token == SIML::TokenType::DOT) {
            node->float_part = std::make_optional<std::optional<std::string_view>>(std::nullopt);
            
            lexer.consume_next();
            token = lexer.peek();

            if (token == SIML::TokenType::NUMBER) {
                node->float_part = lexer.get_next_number();
            }
        }

        token = lexer.peek();
        if (token && *token == SIML::TokenType::IDENT) {
            node->tag = lexer.get_next_ident();
        }

        // REQUIRED ;
        token = lexer.peek(); lexer.consume_next();
        if (token != SIML::TokenType::EXPR_END) {
            return Unexpected(SIML::ParseError("';' expected", lexer));
        }

        return std::move(node);
    }
}

Expected<Unit, SIML::ParseError> parse_object_element(SIML::Lexer& lexer, SIML::NodeObject& obj, SIML::TokenType token) noexcept {
    if (token == SIML::TokenType::DOT) { // Parse property
        // EDGE CASE; .<ident> vs .<number>
        // If it is .<number> return back and parse as value
        // (THIS IS UGLY)
        auto cursor = lexer.m_source.m_pointer;
        
        lexer.consume_next();

        if (lexer.peek() == SIML::TokenType::NUMBER) {
            lexer.m_source.m_pointer = cursor;

            

            auto val = expect(parse_next_node(lexer));
            obj.positionalProperties.push_back(std::move(val));
            return Unit {};
        }

        // EDGE CASE END

        if (lexer.peek() != SIML::TokenType::IDENT) {
            // TODO: error: unexcpected token
        }

        auto ident = lexer.get_next_ident();

        // Optional ":"
        if (lexer.peek() == SIML::TokenType::DOUBLE_DOT) {lexer.consume_next();}

        auto value = expect(parse_next_node(lexer));

        obj.namedProperties[ident] = std::move(value);
        return Unit{};
    }  

    // Parse value
    auto val = expect(parse_next_node(lexer));
    obj.positionalProperties.push_back(std::move(val));
}

Expected<std::unique_ptr<SIML::NodeObject>, SIML::ParseError> parse_next_object(SIML::Lexer& lexer) noexcept {
    
    // TODO: assert in debug mode that it is, in fact TokenType::BLOCK_OPEN
    
    lexer.consume_next();
    auto obj = std::make_unique<SIML::NodeObject>(SIML::NodeObject {});

    while (auto token = lexer.peek()) {
        if (*token == SIML::TokenType::BLOCK_CLOSE) { // End of object
            lexer.consume_next();
            return std::move(obj);
        }
        
        parse_object_element(lexer, *obj, *token);
    }
    return Unexpected(SIML::ParseError("'}' expected, EOF found", lexer));
}

Expected<std::unique_ptr<SIML::NodeObject>, SIML::ParseError> SIML::NodeObject::parse_as_global_node(SIML::Lexer& lexer) noexcept {
    auto base_node = std::make_unique<SIML::NodeObject>(SIML::NodeObject {});

    while (auto token = lexer.peek()) {
        expect(parse_object_element(lexer, *base_node, *token));
    }

    return base_node;
}