#include "Hooking/Hooking.hpp"

namespace base {
	inline bool isUnwantedDependency(u64* callback) {
		u64 f1{ callback[0x60] }, f2{ callback[0x100] }, f3{ callback[0x1A0] };
		if (!util::modules::isAddressInGameRegion(f1) || !util::modules::isAddressInGameRegion(f2) || !util::modules::isAddressInGameRegion(f3))
			return false;
		return util::modules::isJump(f1) || util::modules::isJump(f2) || util::modules::isJump(f3);
	}
	void hooks::queueDependency(void* dependency) {
		if (dependency == pointers::g_reportDependency) {
			LOG_DEBUG("[rage::arxanModules::report]: Report module reuqested");
			return;
		}
		else if (isUnwantedDependency((u64*)dependency)) {
			return;
		}
		CALL(queueDependency, dependency)
	}
}