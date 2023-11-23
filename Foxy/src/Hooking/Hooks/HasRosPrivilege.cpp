#include "Hooking/Hooking.hpp"
#include "Features/Features.hpp"

namespace base {
	bool hooks::hasRosPrivilege(void* _This, int privilege) {
		if (features::spoofing::flags::rockstarDev && privilege == 12) {
			return true;
		}
		RET_CALL(hasRosPrivilege, _This, privilege)
	}
}