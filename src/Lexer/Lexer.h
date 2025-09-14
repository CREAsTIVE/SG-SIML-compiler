#include "./Source.h"
#include <algorithm>
#include <string_view>
#include <optional>

namespace SIML {
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
		Source m_source;

		Lexer(Source source) noexcept : m_source(std::move(source)){};
		
		std::optional<TokenType> peek() noexcept;

		void consume_next() noexcept;

		std::string_view get_next_ident() noexcept;
		std::string_view get_next_string() noexcept;
		std::string_view get_next_number() noexcept;
	};
}
