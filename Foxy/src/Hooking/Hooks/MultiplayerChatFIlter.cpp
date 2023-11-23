#include "Hooking/Hooking.hpp"
#include "Features/Features.hpp"

namespace base {
	int hooks::multiplayerChatFilter(int64_t chatType, const char* input, const char** output) {
		if (!features::chatFilter) {
			return -1;
		}
		RET_CALL(multiplayerChatFilter, chatType, input, output)
	}
}