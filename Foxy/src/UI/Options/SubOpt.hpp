#pragma once
#include "Opt.hpp"
#include "UI/Manager/DXUiManager.hpp"

namespace base::uiTypes {
	class subOpt : public opt<subOpt> {
	public:
		subOpt() = default;
	public:
		subOpt setId(subId subId) {
			m_subId = subId;
			return subOpt(*this);
		}
		void handleAction(eOptAction action) {
			switch (action) {
			case eOptAction::EnterPress: {
				opt::handleAction(action);
				g_dxUiManager.switchToSub(m_subId);
			} break;
			}
		}
		eOptFlag getFlag(eOptFlag flag) {
			return eOptFlag::Enterable;
		}
		eOptType getOptType() {
			return eOptType::SubOpt;
		}
	private:
		subId m_subId;
	};
}