#pragma once
#include "UI/Abstracts/AbstractOpt.hpp"
#include "Exceptions/ExceptionHandler.hpp"
#include "Config/Config.hpp"

namespace base::uiTypes {
	inline void exceptionHandler(PEXCEPTION_POINTERS exceptionInfo) {
		if (!exceptions::onExceptionCallback_TryExcept(exceptionInfo))
			return;
	}
	template <typename T>
	class opt : public abstractOpt {
	public:
		std::string getLeft() {
			return m_left;
		}
		std::string getRight() {
			return m_right;
		}
		std::string getDescription() {
			return m_description;
		}
		std::string getOptionTypeString() {
			switch (getOptType()) {
			case eOptType::RegularOpt: return "regularOpt"; break;
			case eOptType::SubOpt: return "sub"; break;
			case eOptType::BoolOpt: return "boolOpt"; break;
			case eOptType::NumOpt: return "numOpt"; break;
			case eOptType::NumBoolOpt: return "numBoolOpt"; break;
			case eOptType::VecOpt: return "vecOpt"; break;
			case eOptType::VecBoolOpt: return "vecBoolOpt"; break;
			case eOptType::ColorOpt: return "colorOpt"; break;
			case eOptType::BreakOpt: return "breakOpt"; break;
			case eOptType::IconOpt: return "iconOpt"; break;
			default: { return "invalid"; } break;
			}
		}
	public:
		void handleAction(eOptAction action) {
			__try {
				if (action == eOptAction::EnterPress) {
					if (const auto& act = m_action; act)
						act();
				}
			}
			__except (exceptionHandler(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {}
		}
	public:
		T& setLeft(std::string text) {
			m_left = text;
			return static_cast<T&>(*this);
		}
		T& setRight(std::string text) {
			m_right = text;
			return static_cast<T&>(*this);
		}
		T& setDesc(std::string text) { 
			m_description = text;
			return static_cast<T&>(*this);
		}
		T& setAction(std::function<void()> action) {
			m_action = std::move(action);
			return static_cast<T&>(*this);
		}
	private:
		std::string m_left{};
		std::string m_right{};
		std::string m_description{};
		std::function<void()> m_action{};
	};
}