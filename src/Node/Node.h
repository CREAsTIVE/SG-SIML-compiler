#ifndef SIML_NODE_H
#define SIML_NODE_H

#include "../Lexer/Lexer.h"
#include "Expected.h"
#include <iostream>
#include <string_view>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace SIML {
    enum class NodeType {
        STRING,
        NUMBER,
        OBJECT,
        COMPONENT,
        IDENT
    };

    struct Node {
        Node(NodeType type) noexcept : node_type(type) {}
        
        NodeType node_type;
        virtual ~Node() = default;
        
        // Virtual write method to output the tree structure
        virtual void write(std::ostream& stream, int indent_level = 0) const = 0;
    };

    struct NodeString : Node {
        NodeString() noexcept : Node(NodeType::STRING) {}

        std::string_view m_unescaped_value;
        std::optional<std::string_view> tag;

        std::string escaped() noexcept;
        
        void write(std::ostream& stream, int indent_level = 0) const override;
    };

    struct NodeNumber : Node {
        NodeNumber() noexcept : Node(NodeType::NUMBER) {}
        
        std::optional<std::string_view> raw_integer_part;
        std::optional<std::optional<std::string_view>> raw_float_part;
        std::optional<std::string_view> tag;

        bool is_float() noexcept;
        int integer_part() noexcept;
        int float_part() noexcept;
        float as_float() noexcept;
        double as_double() noexcept;
        
        void write(std::ostream& stream, int indent_level = 0) const override;
    };

    struct NodeObject : Node {
        NodeObject() noexcept : Node(NodeType::OBJECT) {}

        std::unordered_map<std::string_view, std::unique_ptr<Node>> namedProperties;
        std::vector<std::unique_ptr<Node>> positionalProperties;
        
        void write(std::ostream& stream, int indent_level = 0) const override;
        
        static Expected<std::unique_ptr<NodeObject>, ParseError> parse_as_global_node(Lexer& lexer) noexcept;
    };

    struct NodeComponent : Node {
        NodeComponent() noexcept : Node(NodeType::COMPONENT) {}

        std::string_view name;
        std::unique_ptr<Node> value;
        
        void write(std::ostream& stream, int indent_level = 0) const override;
    };

    struct NodeIdent : Node {
        NodeIdent() noexcept : Node(NodeType::IDENT) {}

        std::string_view ident;
        
        void write(std::ostream& stream, int indent_level = 0) const override;
    };
}

#endif