#pragma once
#include "Pch/Common.hpp"

namespace base::memory {
	template <typename t>
	concept spanCompatibleType = requires(t a) {
		std::span{ a };
	};
	class bytePatch {
	public:
		template <typename t>
		bytePatch(t address, std::remove_pointer_t<std::remove_reference_t<t>> value) : m_address(address) {
			m_size = sizeof(std::remove_pointer_t<std::remove_reference_t<t>>);
			m_originalBytes = std::make_unique<uint8_t[]>(m_size);
			memcpy(m_originalBytes.get(), m_address, m_size);
			m_value = std::make_unique<uint8_t[]>(m_size);
			memcpy(m_value.get(), &value, m_size);
		}
		template <typename t, typename arr, std::size_t s>
		bytePatch(t address, std::span<arr, s> span) : m_address((void*)address) {
			m_size = span.size();
			m_originalBytes = std::make_unique<byte[]>(m_size);
			memcpy(m_originalBytes.get(), m_address, m_size);
			m_value = std::make_unique<byte[]>(m_size);
			for (int32_t i{}; i != m_size; ++i)
				m_value[i] = span[i];
		}
		virtual ~bytePatch() {
			restore();
		}
	public:
		template <typename t>
		static const std::unique_ptr<bytePatch>& make(t address, std::remove_pointer_t<std::remove_reference_t<t>> val) {
			return m_patches.emplace_back(std::unique_ptr<bytePatch>(new bytePatch(address, val)));
		}
		template <typename t, typename arr> requires spanCompatibleType<arr>
		static const std::unique_ptr<bytePatch>& make(t address, arr spanCompatible) {
			return m_patches.emplace_back(std::unique_ptr<bytePatch>(new bytePatch(address, std::span{ spanCompatible })));
		}
	public:
		void apply() const {
			memcpy(m_address, m_value.get(), m_size);
		}
		void restore() const {
			memcpy(m_address, m_originalBytes.get(), m_size);
		}
	public:
		void remove() const {
			if (const auto val = std::find(m_patches.begin(), m_patches.end(), this); val != m_patches.end()) {
				m_patches.erase(val);
			}
		}
		static void removeAll() {
			m_patches.clear();
		}
	public:
		friend bool operator== (const std::unique_ptr<bytePatch>& a, const bytePatch* b) {
			return a->m_address == b->m_address;
		}
	protected:
		static inline std::vector<std::unique_ptr<bytePatch>> m_patches{};
	public:
		void* m_address{};
		size_t m_size{};
	public:
		std::unique_ptr<uint8_t[]> m_value{};
		std::unique_ptr<uint8_t[]> m_originalBytes{};
	};
}