#ifndef SIML_LEXER_H
#define SIML_LEXER_H

#include "./Source.h"
#include <algorithm>
#include <string_view>
#include <optional>
#include "../Expected.h"

namespace SIML {
	struct ParseError; 

	enum TokenType {
		IDENT, // [a-zA-Z]
		STRING, // "
		NUMBER, // [0-9]
		DOT, // .
		BLOCK_OPEN, // {
		BLOCK_CLOSE, // }
		EXPR_END, // ;
		TEXT_BLOCK_OPEN, // [
		TEXT_BLOCK_CLOSE, // ]
		DOUBLE_DOT // :
	};

	class Lexer {
	public:
		Source& m_source;

		Lexer(Source& source) noexcept : m_source(source){};
		
		std::optional<SIML::TokenType> peek() noexcept;

		void consume_next() noexcept;

		std::string_view get_next_ident() noexcept;
		Expected<std::string_view, SIML::ParseError> get_next_string() noexcept;
		std::string_view get_next_number() noexcept;
	};

	struct ParseError {
        ParseError(std::string message, Lexer& lexer) noexcept : m_message(message), m_position(lexer.m_source.m_pointer) {}; 

        std::string m_message;
        unsigned int m_position;
    };
}

#endif