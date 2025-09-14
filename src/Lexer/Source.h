#ifndef SIML_SOURCE_H
#define SIML_SOURCE_H

#include <string>
#include <string_view>
#include <optional>

namespace SIML {
	class Source {
	public:
		std::string m_data;
		int m_pointer = 0;
		Source(std::string source) noexcept;

		[[nodiscard]] std::optional<char> peek() noexcept;
		// [[nodiscard]] std::optional<std::string_view> peek(int amount) noexcept;

        void next() noexcept;
		// void next(int amount) noexcept;
    };
}

#endif
