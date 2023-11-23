#include "Hooking/Hooking.hpp"
#include "Features/Features.hpp"

namespace base {
	bool hooks::startMatchmakingFindSessions(int profileIndex, int availableSlots, NetworkGameFilterMatchmakingComponent* filter, uint32_t maxSessions, rage::rlSessionInfo* results, uint32_t* resultCount, int* state) {
		if (features::matchmaking::replaceRSMatchmaking && filter->m_filter_type == 1) {
			uint32_t discriminator = filter->m_param_values[0];
			*state = 1;
			auto result = features::matchmaking::triggerSessionFind(features::joinInsideSCTVSlots ? std::optional<uint32_t>(std::nullopt) : discriminator);
			if (result) {
				for (int32_t i{}; i != features::matchmaking::numberOfFoundSessions; ++i) {
					if (features::matchmaking::sessions[i].valid) {
						results[*resultCount] = features::matchmaking::sessions[i].info;
						(*resultCount)++;

						if (maxSessions <= *resultCount)
							break;
					}
				}
				*state = 3;
			}
			else {
				*state = 2;
			}
			return true;
		}
		RET_CALL(startMatchmakingFindSessions, profileIndex, availableSlots, filter, maxSessions, results, resultCount, state);
	}
}