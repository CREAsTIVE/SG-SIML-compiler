#include "./Lexer.h"
#include "Source.h"
#include <cctype>
#include <cassert>
#include <optional>

std::optional<SIML::TokenType> SIML::Lexer::peek() noexcept {
	while (std::optional<char> peeked = m_source.peek() 
		&& std::isspace(*peeked)
	) {
		m_source.next();
	}
	
	if (auto peeked = m_source.peek()) {
		switch (*peeked) {
			case '.':
				return SIML::TokenType::DOT;
			case '{':
				return TokenType::BLOCK_OPEN;
			case ';':
				return TokenType::EXPR_END;
			case '[':
				return TokenType::TEXT_BLOCK_OPEN;
			case ']':
				return TokenType::TEXT_BLOCK_CLOSE;
			case '"':
				return TokenType::STRING;
			case ':':
				return TokenType::DOUBLE_DOT;
		}

		if (isdigit(*peeked)) { return TokenType::NUMBER; }
		if (isalpha(*peeked)) { return TokenType::IDENT; }
	}

	return {};
}

/// Read next one-symbol token
void SIML::Lexer::consume_next() noexcept {
	m_source.next();
	// TODO: Add assert for TokenType to be "simple" in DEBUG mode
}

std::string_view SIML::Lexer::get_next_ident() noexcept {
	// TODO: Assert
	int from = m_source.m_pointer;
	m_source.next(); // skip first char
	while (auto character = m_source.peek()) {
		m_source.next();

		if (!isalpha(*character) && !isdigit(*character)) { 
			return m_source.m_data.substr(from, m_source.m_pointer); 
		}		
	}

	return m_source.m_data.substr(from, m_source.m_pointer);
}

std::string_view SIML::Lexer::get_next_string() noexcept {
	// TODO: Assert
	m_source.next(); // skip first "
	int from = m_source.m_pointer;
	while (auto character = m_source.peek()) {
		m_source.next();

		if (*character == '\\') {
			m_source.next();
		}
		
		if (*character == '"') {
			return m_source.m_data.substr(from, m_source.m_pointer - 1);
		}
	}

	// TODO: Error
}
std::string_view SIML::Lexer::get_next_number() noexcept {
	// TODO: Debug assert
	int from = m_source.m_pointer;
	m_source.next(); // skip first digit
	while (auto character = m_source.peek()) {
		m_source.next();
		if (!isdigit(*character)) { return m_source.m_data.substr(from, m_source.m_pointer); }
	}

	return m_source.m_data.substr(from, m_source.m_pointer);
}
