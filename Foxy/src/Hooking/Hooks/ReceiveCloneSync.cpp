#include "Hooking/Hooking.hpp"
#include "ReceiveCloneData.hpp"

namespace base {
	int64_t hooks::receiveCloneSync(CNetworkObjectMgr* objMgr, CNetGamePlayer* sender, CNetGamePlayer* receiver, uint16_t syncType, uint16_t objId, rage::datBitBuffer* buffer, uint16_t unk, int timestamp) {
		switch (g_receiveCloneData(objMgr, sender, syncType, objId, buffer)) {
		case 2: {
			return static_cast<int64_t>(eSyncReturnTypes::wrongObjOwner);
		} break;
		case 3: {
			return static_cast<int64_t>(eSyncReturnTypes::noGameObjData);
		} break;
		case 4: {
			return static_cast<int64_t>(eSyncReturnTypes::noNetObjData);
		} break;
		}
		return g_hooking->m_receiveCloneSyncHk.getOg<funcTypes::receiveCloneSyncT>()(objMgr, sender, receiver, syncType, objId, buffer, unk, timestamp);
	}
}