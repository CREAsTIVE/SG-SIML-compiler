#include "Node.h"
#include <iomanip>
#include <sstream>

namespace SIML {

namespace {
    std::string create_indent(int level) {
        return std::string(level * 2, ' ');
    }
}

void NodeString::write(std::ostream& stream, int indent_level) const {
    std::string indent = create_indent(indent_level);
    stream << indent << "STRING: \"";
    
    stream << m_unescapedValue << "\"";
    
    if (tag.has_value()) {
        stream << " (tag: " << tag.value() << ")";
    }
    
    stream << "\n";
}

void NodeNumber::write(std::ostream& stream, int indent_level) const {
    std::string indent = create_indent(indent_level);
    stream << indent << "NUMBER: ";
    
    if (m_rawIntegerPart.has_value()) {
        stream << m_rawIntegerPart.value();
    }
    
    if (m_rawFloatPart.has_value() && m_rawFloatPart.value().has_value()) {
        stream << "." << m_rawFloatPart.value().value();
    }
    
    if (m_tag.has_value()) {
        stream << " (tag: " << m_tag.value() << ")";
    }
    
    stream << "\n";
}

void NodeObject::write(std::ostream& stream, int indent_level) const {
    std::string indent = create_indent(indent_level);
    stream << indent << "OBJECT:\n";
    
    // Write named properties
    if (!m_namedProperties.empty()) {
        stream << indent << "  Named Properties:\n";
        for (const auto& [key, value] : m_namedProperties) {
            stream << indent << "    " << key << ": ";
            if (value) {
                value->write(stream, indent_level + 3);
            } else {
                stream << "null\n";
            }
        }
    }
    
    // Write positional properties
    if (!m_positionalProperties.empty()) {
        stream << indent << "  Positional Properties:\n";
        for (size_t i = 0; i < m_positionalProperties.size(); ++i) {
            stream << indent << "    [" << i << "]: ";
            if (m_positionalProperties[i]) {
                m_positionalProperties[i]->write(stream, indent_level + 3);
            } else {
                stream << "null\n";
            }
        }
    }
}

void NodeComponent::write(std::ostream& stream, int indent_level) const {
    std::string indent = create_indent(indent_level);
    stream << indent << "COMPONENT: " << name << "\n";
    
    if (value) {
        stream << indent << "  Value:\n";
        value->write(stream, indent_level + 2);
    } else {
        stream << indent << "  Value: null\n";
    }
}

void NodeIdent::write(std::ostream& stream, int indent_level) const {
    std::string indent = create_indent(indent_level);
    stream << indent << "IDENT: " << ident << "\n";
}
} // namespace SIML