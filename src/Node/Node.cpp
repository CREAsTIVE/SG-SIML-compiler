#include "./Node.h"
#include "../Expected.h"
#include "../Node/Node.h"
#include <charconv>
#include <exception>
#include <optional>
#include <string>
#include <vector>

std::string SIML::NodeString::escaped() noexcept {
    auto chars = std::vector<char>();
    for (auto c : m_unescapedValue) {
        if (c != '\\') {
            chars.push_back(c);
        }
    }

    return std::string(chars.begin(), chars.end());
}

bool SIML::NodeNumber::isFloat() noexcept {
    return m_rawFloatPart != std::nullopt;
}

int parse_str(std::string_view str) noexcept {
    try {
        int i;
        std::from_chars(&*str.begin(), &*str.end(), i);
        return i;
    } catch (const std::exception& e) {}
    return 0; // How tf did you get there???
}

int SIML::NodeNumber::integerPart() noexcept {
    if (!m_rawFloatPart) {
        return 0;
    }
    return parse_str(*m_rawIntegerPart);
}

int SIML::NodeNumber::floatPart() noexcept {
    if (!m_rawFloatPart && !(*m_rawFloatPart)) {
        return 0;
    }
    return parse_str(**m_rawFloatPart);
}

float SIML::NodeNumber::asFloat() noexcept {
    float total = integerPart();
    if (m_rawFloatPart && *m_rawFloatPart) {
        total += (float)floatPart() / ((**m_rawFloatPart).length() * 10);
    }

    return total;
}

double SIML::NodeNumber::asDouble() noexcept {
    double total = integerPart();
    if (m_rawFloatPart && *m_rawFloatPart) {
        total += (double)floatPart() / ((**m_rawFloatPart).length() * 10);
    }

    return total;
}