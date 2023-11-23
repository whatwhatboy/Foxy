#pragma once
#include "Pch/Common.hpp"

namespace base::ui::informational {
	struct formattedString {
		template <typename ...T>
		formattedString(std::string format, T&&... args) : m_value(std::vformat(format, std::make_format_args(args...))) {}

		std::string str() {
			return m_value;
		}
		std::vector<std::string> split(char s) {
			return g_splitStr(m_value, s);
		}
		std::pair<std::string, std::string> midSplit(char s, bool oneCharOffet = true) {
			auto splitStr{ split(s) };
			return std::make_pair(splitStr[0], &(splitStr[1])[oneCharOffet]);
		}
		constexpr const char* c_str() {
			return m_value.c_str();
		}

		std::string m_value{};
	};
}