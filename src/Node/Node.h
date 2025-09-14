#ifndef SIML_NODE_H
#define SIML_NODE_H

#include "../Lexer/Lexer.h"

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
    };

    struct NodeString : Node {
        NodeString() noexcept : Node(NodeType::STRING) {}

        std::string_view unescaped_value;
        std::optional<std::string_view> tag;
    };

    struct NodeNumber : Node {
        NodeNumber() noexcept : Node(NodeType::NUMBER) {}
        
        std::optional<std::string_view> integer_part;
        std::optional<std::optional<std::string_view>> float_part;
        std::optional<std::string_view> tag;
    };

    struct NodeObject : Node {
        NodeObject() noexcept : Node(NodeType::OBJECT) {}

        std::unordered_map<std::string_view, std::unique_ptr<Node>> namedProperties;
        std::vector<std::unique_ptr<Node>> positionalProperties;

        static std::unique_ptr<NodeObject> parse_as_global_node(Lexer& lexer) noexcept;
    };

    struct NodeComponent : Node {
        NodeComponent() noexcept : Node(NodeType::COMPONENT) {}

        std::string_view name;
        std::unique_ptr<Node> value;
    };

    struct NodeIdent : Node {
        NodeIdent() noexcept : Node(NodeType::IDENT) {}

        std::string_view ident;
    };
}

#endif