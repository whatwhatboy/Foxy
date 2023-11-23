#include "Hooking/Hooking.hpp"
#include "Features/Features.hpp"

namespace base {
	bool hooks::getPeerAddress(uint64_t* peerAddress) {
		if (features::spoofing::peerAddress.second) {
			*peerAddress = features::spoofing::peerAddress.first;
			return true;
		}
		RET_CALL(getPeerAddress, peerAddress)
	}
}