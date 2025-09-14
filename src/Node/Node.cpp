#include "./Node.h"
#include "Expected.h"
#include "Node/Node.h"
#include <charconv>
#include <exception>
#include <optional>
#include <string>
#include <vector>

std::string SIML::NodeString::escaped() noexcept {
    auto chars = std::vector<char>();
    for (auto c : m_unescaped_value) {
        if (c != '\\') {
            chars.push_back(c);
        }
    }

    return std::string(chars.begin(), chars.end());
}

bool SIML::NodeNumber::is_float() noexcept {
    return raw_float_part != std::nullopt;
}

int parse_str(std::string_view str) noexcept {
    try {
        int i;
        std::from_chars(str.begin(), str.end(), i);
        return i;
    } catch (const std::exception& e) {}
    return 0; // How tf did you get there???
}

int SIML::NodeNumber::integer_part() noexcept {
    if (!raw_integer_part) {
        return 0;
    }
    return parse_str(*raw_integer_part);
}

int SIML::NodeNumber::float_part() noexcept {
    if (!raw_float_part && !(*raw_float_part)) {
        return 0;
    }
    return parse_str(**raw_float_part);
}

float SIML::NodeNumber::as_float() noexcept {
    float total = integer_part();
    if (raw_float_part && *raw_float_part) {
        total += (float)float_part() / ((**raw_float_part).length() * 10);
    }

    return total;
}

double SIML::NodeNumber::as_double() noexcept {
    double total = integer_part();
    if (raw_float_part && *raw_float_part) {
        total += (double)float_part() / ((**raw_float_part).length() * 10);
    }

    return total;
}