#ifndef SIML_LEXER_H
#define SIML_LEXER_H

#include "./Source.h"
#include <algorithm>
#include <string_view>
#include <optional>
#include <tuple>
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
        ParseError(std::string message, Lexer& lexer) noexcept : m_message(message), m_lexer(lexer) {}; 

        std::string m_message;
        Lexer& m_lexer;

		std::tuple<unsigned int, unsigned int> to_ln_col() {
			unsigned int line = 0;
			unsigned int column = 0;

			for (int i = 0; i < m_lexer.m_source.m_pointer; i++) {
				column++;
				if (m_lexer.m_source.m_data[i] == '\n') {
					line++; column = 0;
				}
			}

			return std::make_tuple(line, column);
		}
    };
}

#endif