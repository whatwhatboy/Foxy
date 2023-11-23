#pragma once
#include "Pch/Common.hpp"

namespace base::ui {
	
	enum class notifyPhase : int {
		wait = 1,
		fadeIn = 2,
		fadeOut = 3,
		expired = 4,
	};
	class notifyTemplate {
	public:
		template <typename ...arguments>
		notifyTemplate(std::string title, int16_t dismissTime, bool isRepeat, int repeatCount, std::string format, arguments... args) {
			m_creationTime = GetTickCount64();
			m_dismissTime = dismissTime;
			m_title = title;
			m_isRepeat = isRepeat;
			m_content = std::vformat(format, std::make_format_args(args...));
		}
	public:
		auto elapsedTime() { return GetTickCount64() - m_creationTime; }
		auto phase() {
			auto elapsed = elapsedTime();
			if (uint64_t(elapsed) > uint64_t(m_dismissTime) + (250ui64 * 2ui64))
				return notifyPhase::expired;
			else if (uint64_t(elapsed) > uint64_t(m_dismissTime) + 250ui64)
				return notifyPhase::fadeOut;
			else if (uint64_t(elapsed) > 250ui64)
				return notifyPhase::wait;
			else
				return notifyPhase::fadeIn;
		}
		auto fadePercent() {
			auto elapsed = elapsedTime();
			if (phase() == notifyPhase::fadeIn)
				return float(elapsed) / 250.f;
			else if (phase() == notifyPhase::fadeOut)
				return ((float(elapsed) - float(m_dismissTime)) / 250.f);
			return 1.f;
		}
		auto getFlashTimeframe() {
			return elapsedTime() + 250;
		}
		auto setFlash(ImU32& color, ImU32 flashColor) {
			std::thread thr([&] {
				ImU32 col = color;
				for (int8_t i{}; i != m_repeatCount; ++i) {
					color = flashColor;
					std::this_thread::sleep_for(100ms);
					color = col;
				}
			});
			thr.detach();
		}
	public:
		uint64_t m_creationTime{};
		int16_t m_dismissTime{ 3500 };
		std::string m_title{};
		std::string m_content{};
		bool m_isRepeat{};
		int m_repeatCount{};
	};
	inline auto percentToAlpha(float percent) {
		return int(percent) * 255;
	}
}