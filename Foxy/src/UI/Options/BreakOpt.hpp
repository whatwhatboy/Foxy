#pragma once
#include <functional>
#include "Opt.hpp"

namespace base::uiTypes {
	class breakOpt : public opt<breakOpt> {
	public:
		eOptType getOptType() {
			return eOptType::BreakOpt;
		}
	};
}