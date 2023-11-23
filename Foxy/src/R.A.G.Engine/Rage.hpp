#pragma once
#include "Pch/Common.hpp"
#include "ScrThread.hpp"
#include "AtArray.hpp"
#include "NetGameEvent.hpp"
#include "NetPlayer.hpp"
#include "SyncTree.hpp"
#include "Colors.hpp"
#include "CPlayerInfo.hpp"

#include "CEntity.hpp"
#include "fwEntity.hpp"
#include "CPhysical.hpp"

#include "CPed.hpp"
#include "CPedFactory.hpp"
#include "CPedModelInfo.hpp"
#include "CPedWeaponManager.hpp"

#include "CWeaponInfo.hpp"
#include "CItemInfo.hpp"
#include "CAmmoInfo.hpp"
#include "CAimingInfo.hpp"
#include "CVehicleModelInfo.hpp"

#include "NetPlayer.hpp"
#include "GamerInfo.hpp"
#include "PeerInfo.hpp"
#include "GamerHandle.hpp"
#include "NetSocketAddress.hpp"
#include "ClanData.hpp"

#include "CNetworkPlayerMgr.hpp"
#include "NetPlayerMgrBase.hpp"

#include "fwExtensible.hpp"

#include "scrNativeCallContext.hpp"

namespace rage {
	//Define bitbuffer because include errors
	class datBitBuffer;
	class CSyncDataReader;
	class CSyncDataWriter;
	class netEventMgr;
}