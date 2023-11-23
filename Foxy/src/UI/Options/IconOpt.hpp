#pragma once
#include <functional>
#include "Opt.hpp"

namespace base::uiTypes {
	class iconOpt : public opt<iconOpt> {
	public:
		iconOpt& setIcon(char iconKey) {
			std::string key({ iconKey, '\0' });
			if (!key.empty() && iconKey != '\0')
				setRight(key);
			return *this;
		}
	public:
		eOptType getOptType() {
			return eOptType::IconOpt;
		}
	};
}