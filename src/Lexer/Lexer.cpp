#include "./Lexer.h"
#include "Source.h"
#include <cctype>
#include <cassert>
#include <optional>
#include <string_view>
#include <sys/types.h>
#include "../Expected.h"

std::optional<SIML::TokenType> SIML::Lexer::peek() noexcept {
	while (std::optional<char> peeked = m_source.peek()) {
		if (!std::isspace(*peeked)) {break;}
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
			case '}':
				return TokenType::BLOCK_CLOSE;
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

std::string_view substr(std::string_view source, std::uint32_t from, std::uint32_t size) {
	return std::string_view(source.data() + from, size);
}

std::string_view SIML::Lexer::getNextIdent() noexcept {
	// TODO: Assert
	int from = m_source.m_pointer;
	m_source.next(); // skip first char
	while (auto character = m_source.peek()) {
		if (!isalpha(*character) && !isdigit(*character)) { 
			return substr(m_source.m_data, from, m_source.m_pointer - from); 
		}		
		
		m_source.next();
	}

	return substr(m_source.m_data, from, m_source.m_pointer - from);
}

Expected<std::string_view, SIML::ParseError> SIML::Lexer::getNextString() noexcept {
	// TODO: Assert
	m_source.next(); // skip first "
	int from = m_source.m_pointer;
	while (auto character = m_source.peek()) {
		m_source.next();

		if (*character == '\\') {
			m_source.next();
		}
		
		if (*character == '"') {
			return substr(m_source.m_data, from, m_source.m_pointer - 1 - from);
		}
	}

	return Unexpected(SIML::ParseError("'\"\' expected, EOF found", *this));
}
std::string_view SIML::Lexer::getNextNumber() noexcept {
	// TODO: Debug assert
	int from = m_source.m_pointer;
	m_source.next(); // skip first digit
	while (auto character = m_source.peek()) {
		if (!isdigit(*character)) { return substr(m_source.m_data, from, m_source.m_pointer - from); }
		m_source.next();
	}

	return substr(m_source.m_data, from, m_source.m_pointer - from);
}
