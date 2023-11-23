#include "Hooking/Hooking.hpp"
#include "R.A.G.Engine/Network.hpp"
#include "R.A.G.Engine/RemoteGamerInfoMsg.hpp"
#include "Features/Features.hpp"

namespace base {
	bool hooks::constructJoinRequest(RemoteGamerInfoMsg* info, void* data, int size, int* bits) {
		if (!info->unk_00C0)
			info->unk_00C0 = 1;
		memset(info->m_handles, 0, sizeof(info->m_handles));
		info->m_num_handles = 0;
		RET_CALL(constructJoinRequest, info, data, size, bits);
	}
}