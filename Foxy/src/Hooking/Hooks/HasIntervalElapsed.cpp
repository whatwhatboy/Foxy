#include "Hooking/Hooking.hpp"
#define MS(s) 1000 * s

namespace base {
	bool hooks::hasIntervalElapsed(uint32_t timestamp, uint32_t interval) {
		switch (interval) {
		case MS(3): {
			return false;
		} break;
		case MS(30): {
			return false;
		} break;
		}
		RET_CALL(hasIntervalElapsed, timestamp, interval)
	}
}