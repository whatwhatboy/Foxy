#pragma once
#include <functional>
#include "Opt.hpp"

namespace base::uiTypes {
	class regularOpt : public opt<regularOpt> {
	public:
		eOptType getOptType() {
			return eOptType::RegularOpt;
		}
	};
}