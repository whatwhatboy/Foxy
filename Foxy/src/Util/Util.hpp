#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/CNetGamePlayer.hpp"
#include "R.A.G.Engine/CEntity.hpp"
#include "R.A.G.Engine/CPed.hpp"
#include "R.A.G.Engine/CBaseModelInfo.hpp"
#include "R.A.G.Engine/eNetMessage.hpp"
#include "R.A.G.Engine/Network.hpp"
#include "R.A.G.Engine/Natives/NativeDefines.hpp"
#include "Auth/XorStr.hpp"

namespace base::compile::util {
	template <size_t arrayS>
	struct arrayData {
		constexpr arrayData(char const(&val)[arrayS]) { std::copy_n(val, arrayS, data); }
		char data[arrayS]{};
		constexpr void clear() {
			char cleanArray[arrayS]{};
			std::copy_n(cleanArray, arrayS, data);
		}
	};
	template <size_t size>
	constexpr size_t byteCount(char const(&val)[size]) {
		size_t byteCount{};
		for (size_t i{}; i != size; ++i)
			if (val[i] == ' ')
				byteCount++;
		return byteCount + 1;
	}
	static_assert(byteCount("FF E3") == 2);
	constexpr int8_t charToHex(char const c) {
		if (c >= 'a' && c <= 'f')
			return static_cast<int>(c) - 87;
		if (c >= 'A' && c <= 'F')
			return static_cast<int>(c) - 55;
		if (c >= '0' && c <= '9')
			return static_cast<int>(c) - 48;
		return -1;
	}
	static_assert(charToHex('C') == 0xC && charToHex('c') == 0xc);
}
namespace base::util {
	namespace modules {
		inline std::map<std::string, std::pair<uint64_t, uint64_t>> g_moduleMap{};
		extern std::map<std::string, std::pair<uint64_t, uint64_t>> getAllModuleInfo();
		template <typename addressType = uintptr_t>
		extern std::string getModuleNameFromAddressImpl(addressType address);
		template <typename addressType = uintptr_t>
		extern std::string getModuleNameFromAddress(addressType address);
		extern bool isAddressInGameRegion(u64 addr);
		extern bool isJump(u64 addr);
	}
	namespace callstack {
		extern std::string dump();
	}
	namespace localization {
		extern std::string getGxtLabel(std::string key, bool allowNative = false);
		extern std::string createRandomString(size_t len);
	}
	namespace network {
		namespace misc {
			extern bool isVehicleSyncType(uint16_t syncType);
			extern bool deserialiseNetMessage(enum rage::eNetMessage& msg, class rage::datBitBuffer& buffer);
			inline constexpr char const* g_advertisementStrings[12] = {
				"Discord",
				"www.",
				".gg",
				".c",
				"http",
				//Known advertisers
				"wavy",
				"krutka",
				"ItzGoated",
				"warlord",
				"doit",
				"OrangeMango",
				"Faynx"
			};
			extern std::string makeSenseOfDebugMessage(std::string msg);
			extern bool isAdvertisementMessage(std::string msg);
			extern CNetGamePlayer* getHostOfScript(std::string script);
		}
		namespace session {
			namespace peer {
				extern rage::snPeer* getViaConnectionId(uint8_t conId);
			}
			namespace player {
				extern rage::snPlayer* getViaConnectionId(uint8_t conId);
				extern rage::snPlayer* getViaMsgId(uint32_t msgId);
				extern rage::snPlayer* getViaPeerAddress(uint64_t peerAddress);
				extern rage::snPlayer* getViaPlatformData(uint64_t platformData);
			}
			extern rage::snSession& get();
			extern rage::snSession* getPointer();
			extern rage::snSession& getTransition();
			extern rage::snSession* getTransitionPointer();
		}
		namespace object {
			namespace mgr {
				extern CNetworkObjectMgr* get();
			}
		}
		extern CNetGamePlayer* getCNetGamePlayerViaConnectionId(uint8_t conId);
		extern CNetGamePlayer* getCNetGamePlayerViaPeerAddress(uint64_t peerAddress);
		extern CNetGamePlayer* getCNetGamePlayerViaPeerId(uint64_t peerId);
		extern CNetGamePlayer* getCNetGamePlayerViaNetMsgId(uint32_t msgId);
		extern CNetGamePlayer* getRandomCNetGamePlayer(bool excludeSelf, bool excludeFriends);
		extern CNetGamePlayer* getHostCNetGamePlayer();
		extern Network* get();
		extern uint16_t getPlayerCount();
		extern bool isSessionActive();
	}
	namespace entity {
		extern Entity getEntityFromHandle(rage::CEntity* handle);
		extern rage::CEntity* getCEntityFromHandle(Entity handle);
	}
	namespace player {
		inline CPlayerInfo* g_handle{};
		inline CNetGamePlayer* g_networkHandle{};
		inline Player g_entityHandle{};
		extern Player getLocalPlayerHandle();
		extern CNetGamePlayer* getLocalCNetGamePlayer();
		extern CPlayerInfo* getLocalCPlayerInfo();
		extern void tick();
	}
	namespace ped {
		inline CPed* g_handle{};
		inline Ped g_entityHandle{};
		extern CPed* getLocalCPed();
		extern Ped getLocalPedHandle();
		extern void tick();
	}
	namespace vehicle {
		inline CVehicle* g_handle{};
		inline Vehicle g_entityHandle{};
		extern CVehicle* getLocalCVehicle();
		extern Vehicle getLocalVehicleHandle();
		extern rage::CBaseModelInfo* getModelData(uint32_t hash);
		extern void tick();
	}
	extern void tick();
}