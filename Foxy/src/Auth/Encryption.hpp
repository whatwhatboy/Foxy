#pragma once
#include "Pch/Common.hpp"

constexpr std::string_view base64Charaters{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };
inline bool IsBase64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}
inline constexpr bool isCharacterBase64(uint8_t c) {
	if (base64Charaters.find(c) != -1) {
		return true;
	}
	return false;
}
inline std::string base64Encode(std::string str) {
	int strLen{ (int)str.length() };
	int i{}, j{}, in_{};
	uint8_t char_array_4[4], char_array_3[3];
	std::string ret{};
	while (strLen--) {
		char_array_3[i++] = str[in_]; in_++;
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			for (i = 0; (i < 4); i++)
				ret += base64Charaters[char_array_4[i]];
			i = 0;
		}
	}
	if (i) {
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
		for (j = 0; (j < i + 1); j++)
			ret += base64Charaters[char_array_4[j]];
		while ((i++ < 3))
			ret += '=';
	}
	return ret;
}
inline std::string base64Decode(std::string str) {
	int strLen{ (int)str.length() };
	int i{}, j{}, in_{};
	uint8_t char_array_4[4], char_array_3[3];
	std::string ret{};
	while (strLen-- && (str[in_] != '=') && IsBase64(str[in_])) {
		char_array_4[i++] = str[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64Charaters.find(char_array_4[i]);
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}
	if (i) {
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;
		for (j = 0; j < 4; j++)
			char_array_4[j] = base64Charaters.find(char_array_4[j]);
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
		for (j = 0; (j < i - 1); j++)
			ret += char_array_3[j];
	}
	return ret;
}