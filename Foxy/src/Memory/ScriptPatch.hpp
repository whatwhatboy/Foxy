#pragma once
#include "Pch/Common.hpp"
#include "Core/Logger.hpp"
#include "R.A.G.Engine/Joaat.hpp"
#include "Memory/Util.hpp"

namespace base::memory::script {
	struct data {
		data(uint32_t size, uint8_t** bytes, uint32_t count) : size(size), bytes(bytes), count(count) {

		}
		~data() {
			for (int i{}; i != count; ++i) {
				delete[] bytes[i];
			}
			delete[] bytes;
			bytes = nullptr;
		}

		uint32_t size{};
		uint8_t** bytes{};
		uint32_t count{};
	};
	struct patch {
		patch(rage::joaat_t script, std::string pattern, int32_t offset, std::vector<std::uint8_t> bytes, bool* toggle) :
			script(script), pattern(pattern), offset(offset), bytes(bytes), toggle(toggle), original(std::vector<uint8_t>()), ip(NULL)
		{

		}
		static uint8_t* getCodeAddr(data* data, uint32_t index) {
			return &data->bytes[index >> 14][index & 0x3FFF];
		}
		static const std::optional<uint32_t> getCodeLocByPtr(data* data, std::string pattern) {
			uint32_t size{ data->size };
			std::vector<std::optional<uint8_t>> bytes{ util::createBytesFromString(pattern) };
			for (uint32_t i{}; i != size - bytes.size(); ++i) {
				for (uint32_t j{}; j != bytes.size(); j) {
					if (bytes[j].has_value()) {
						if (bytes[j].value() != *getCodeAddr(data, i + j)) {
							continue;
						}
					}
				}
				return i;
			}
			return std::nullopt;
		}
		void enable(data* data) {
			memcpy(getCodeAddr(data, ip), bytes.data(), bytes.size());
		}
		void disable(data* data) {
			memcpy(getCodeAddr(data, ip), original.data(), original.size());
		}
		void update(data* data) {
			if (!ip) {
				auto result = getCodeLocByPtr(data, pattern);
				if (!result.has_value())
					LOG(Fatal, "Failed to find script pattern!");
				ip = result.value() + offset;
				for (int i{}; i != bytes.size(); ++i)
					original.push_back(*getCodeAddr(data, ip + i));
			}
			if (!toggle || *toggle)
				enable(data);
			else
				disable(data);
		}

		rage::joaat_t script{};
		std::string pattern{};
		int32_t offset{};
		std::vector<uint8_t> bytes{};
		bool* toggle{};
		std::vector<uint8_t> original{};
		int32_t ip{};
	};
}