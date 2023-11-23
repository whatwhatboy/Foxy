#include "Util.hpp"
#include "Memory/Patterns/Pointers.hpp"
#include "R.A.G.Engine/Rage.hpp"
#include "R.A.G.Engine/eNetMessage.hpp"
#include "R.A.G.Engine/BitBuffer.hpp"
#include "R.A.G.Engine/ScrHandlers.hpp"
#include "Fibers/Fiber.hpp"
#include "Fibers/Threading.hpp"
#include "R.A.G.Engine/Natives/Natives.hpp"

namespace base::util {
	namespace modules {
		std::map<std::string, std::pair<uint64_t, uint64_t>> getAllModuleInfo() {
			decltype(g_moduleMap) map{};
			HMODULE modules[1024]{};
			HANDLE process{ GetCurrentProcess() };
			DWORD cbNeeded{};
			if (K32EnumProcessModules(process, modules, 1024, &cbNeeded)) {
				for (int32_t i{}; i != (cbNeeded / sizeof(HMODULE)); ++i) {
					//Get the full path to the module's file
					char modNameCharArray[MAX_PATH];
					if (K32GetModuleFileNameExA(process, modules[i], modNameCharArray, MAX_PATH)) {
						std::string modName(modNameCharArray);
						MODULEINFO info{};
						if (K32GetModuleInformation(process, modules[i], &info, sizeof(MODULEINFO))) { //Query the MODULEINFO clas from the module list
							map.emplace(modName, std::make_pair(uintptr_t(info.lpBaseOfDll), uintptr_t(info.SizeOfImage))); //Return name, base addr and image size
						}
					}
				}
			}
			return map;
		}
		template <typename addressType>
		std::string getModuleNameFromAddressImpl(addressType address) {
			for (auto const& [key, val] : g_moduleMap) {
				if (uintptr_t(address) > val.first && uintptr_t(address) < (val.first + val.second)) { //If address is in range of base addr to size
					return std::format("{}+0x{:X}", key.substr(key.find_last_of('\\') + 1, key.find('.') + 3), uintptr_t(address) - val.first);
				}
			}
			return {};
		}
		template <typename addressType>
		std::string getModuleNameFromAddress(addressType address) {
			if (g_moduleMap.empty())
				g_moduleMap = getAllModuleInfo();
			auto returnValue = getModuleNameFromAddressImpl<addressType>(address);
			if (returnValue.empty()) {
				g_moduleMap.clear();
				g_moduleMap = getAllModuleInfo(); //Incase the module isn't found, we update the module map and try again.
				returnValue = getModuleNameFromAddressImpl<addressType>(address);
			}
			return returnValue;
		}
		bool isAddressInGameRegion(u64 addr) {
			static u64 moduleBase{};
			static u64 moduleSize{};
			if (!moduleBase || !moduleSize) {
				MODULEINFO info{};
				if (!K32GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(NULL), &info, sizeof(info))) {
					LOG(Fatal, "GMI on QD failed.");
					return true;
				}
				else {
					moduleBase = (u64)GetModuleHandleA(NULL);
					moduleSize = (u64)info.SizeOfImage;
				}
			}
			return addr > moduleBase && addr < (moduleBase + moduleSize);
		}
		bool isJump(u64 addr) {
			if (!isAddressInGameRegion(addr))
				return false;
			return *(u8*)(addr) == 0xE9;
		}
	}
	namespace callstack {
		std::string dump() {
			std::string str{};
			void* trace[62]{};
			auto fnCount = RtlCaptureStackBackTrace(0, 62, trace, NULL);
			if (fnCount != NULL) {
				str += "[";
				for (int32_t i{}; i != fnCount - 1; ++i) {
					uint64_t addr{ reinterpret_cast<uint64_t>(trace[i]) };
					std::string moduleName{ modules::getModuleNameFromAddress(addr) };
					if (!moduleName.empty())
						str += moduleName + ", ";
					else
						str += std::format("0x{:X}", addr) + ", ";
				}
				uint64_t addr{ reinterpret_cast<uint64_t>(trace[fnCount]) };
				std::string moduleName{ modules::getModuleNameFromAddress(addr) };
				if (!moduleName.empty())
					str += moduleName;
				else
					str += std::format("0x{:X}", addr);
				str += "]";
			}
			return str;
		}
	}
	namespace localization {
		std::string getGxtLabel(std::string key, bool allowNative) {
			if (allowNative)
				return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(key.c_str());
			auto gxtViaString{ pointers::g_getGxtLabelFromTable(pointers::g_gxtLabels, key.c_str()) };
			if (!gxtViaString) {
				auto gxtViaJoaat{ pointers::g_getJoaatedGxtLabelFromTable(pointers::g_gxtLabels, rage::atStringHash(key)) };
				if (!gxtViaString) {
					if (allowNative)
						return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(key.c_str());
					return key;
				}
				else {
					return gxtViaJoaat;
				}
			}
			else {
				if (allowNative)
					return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(key.c_str());
				return gxtViaString;
			}
		}
		std::string createRandomString(size_t len) {
			auto randomCharacter = []() -> char {
				const char characterSet[] =
					"0123456789"
					"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
					"abcdefghijklmnopqrstuvwxyz";
				const size_t max_index = sizeof(characterSet) - 1;
				return characterSet[rng(0, max_index).get<size_t>()];
			};
			std::string str(len, '\0');
			std::generate_n(str.begin(), len, randomCharacter);
			return str;
		}
	}
	namespace network {
		namespace misc {
			bool isVehicleSyncType(uint16_t syncType) {
				if (syncType >= bikeObjType && syncType <= heliObjType)
					return true;
				else if (syncType >= trailerObjType && syncType <= trainObjType)
					return true;
				else if (syncType >= planeObjType && syncType <= submarineObjType)
					return true;
				return false;
			}
			#define RET_DEAD() { \
				msg = rage::eNetMessage::MsgInvalid; \
				return false; \
			}
			bool deserialiseNetMessage(enum rage::eNetMessage& msg, class rage::datBitBuffer& buffer) {
				if (buffer.m_flagBits & 2)
					RET_DEAD();
				uint32_t pos{ (buffer.m_flagBits & 1) == 0 ? buffer.m_curBit : buffer.m_maxBit };
				uint32_t magic{};
				uint32_t extended{};
				if (buffer.m_bitsRead + 15 > pos)
					RET_DEAD();
				if (!buffer.ReadDword(&magic, 14))
					RET_DEAD();
				if (magic != 0x3246)
					RET_DEAD();
				if (!buffer.ReadDword(&extended, 1))
					RET_DEAD();
				uint32_t length{ extended ? 16u : 8u };
				pos = (buffer.m_flagBits & 1) == 0 ? buffer.m_curBit : buffer.m_maxBit;
				if (length + buffer.m_bitsRead <= pos) {
					if (buffer.ReadDword((uint32_t*)&msg, length))
						return true;
				}
				return false;
			}
			//TODO Lmao, needs to be dynamic
			std::string makeSenseOfDebugMessage(std::string msg) {
				auto strs = g_splitStr(msg, '|');
				if (strs.size() == NULL || strs.size() == 1) {
					auto string = msg.substr(msg.find(xorstr_("[R]_")));
					if (auto strIdx = string.find(xorstr_("(")); strIdx != std::string::npos) {
						auto str = string.substr(strIdx, string.find(xorstr_(")"))); //Get array index if value isn't npos
						auto arrIdx = std::stoi(str);
						if (arrIdx > static_cast<int>(eNetworkEvents::CNetworkCheckCatalogCrc)) {
							return std::format("Network event: {} | Debug value: {}", arrIdx, string.substr(string.find(xorstr_(")"))));
						}
						else {
							return std::format("[eNetworkEvents]:{}, Debug value: {}", arrIdx, string.substr(string.find(xorstr_(")"))));
						}
					}
					else {

					}
				}
				else if (strs.size() == 2) {
					auto string = strs[0].substr(strs[0].find(xorstr_("[R]_")));
					if (auto strIdx = string.find(xorstr_("(")); strIdx != std::string::npos) {
						auto str = string.substr(strIdx, string.find(xorstr_(")"))); //Get array index if value isn't npos
						auto arrIdx = std::stoi(str);
						if (arrIdx <= static_cast<int>(eNetworkEvents::CNetworkCheckCatalogCrc)) {
							switch (static_cast<eNetworkEvents>(arrIdx)) {
							case eNetworkEvents::CNetworkCheckCatalogCrc: {

							}
							}
						}
						else {

						}
					}
				}
			}
			bool isAdvertisementMessage(std::string msg) {
				//We need to check if we can tell the scaleform to fuck off when parsing these
				int numberOfFoundStrings{};
				if (!msg.empty() || msg.size() < 2) {
					for (int8_t i{}; i != sizeof(g_advertisementStrings); ++i) {
						if (msg.find(g_advertisementStrings[i]) != std::string::npos)
							numberOfFoundStrings++;
					}
				}
				return numberOfFoundStrings >= 2; //If the advert string count is 2 or more, it's likely an advertisement.
			}
			CNetGamePlayer* getHostOfScript(std::string script) {
				if (auto scrThread = threading::getThr(script.c_str()); scrThread) {
					if (auto cGameScriptHandlerNetComponent = scrThread->m_net_component; cGameScriptHandlerNetComponent) {
						return cGameScriptHandlerNetComponent->get_host();
					}
				}
				return nullptr;
			}
		}
		namespace session {
			namespace peer {
				rage::snPeer* getViaConnectionId(uint8_t conId) {
					return session::get().m_peers[conId];
				}
			}
			namespace player {
				rage::snPlayer* getViaConnectionId(uint8_t conId) {
					return session::get().m_players[conId];
				}
				rage::snPlayer* getViaMsgId(uint32_t msgId) {
					return session::getPointer()->getPlayerByMsgId(msgId);
				}
				rage::snPlayer* getViaPeerAddress(uint64_t peerAddress) {
					return session::getPointer()->getPlayerByPeerAddress(peerAddress);
				}
				rage::snPlayer* getViaPlatformData(uint64_t platformData) {
					return session::getPointer()->getPlayerByPlatformData(platformData);
				}
			}
			rage::snSession& get() {
				return network::get()->m_game_session;
			}
			rage::snSession* getPointer() {
				return network::get()->m_game_session_ptr;
			}
			rage::snSession& getTransition() {
				return network::get()->m_transition_session;
			}
			rage::snSession* getTransitionPointer() {
				return network::get()->m_transition_session_ptr;
			}
		}
		namespace object {
			namespace mgr {
				CNetworkObjectMgr* get() {
					return *pointers::g_netObjMgr;
				}
			}
		}
		CNetGamePlayer* getCNetGamePlayerViaConnectionId(uint8_t conId) {
			if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
				if (auto& cNetGamePlayer = cNetworkPlayerMgr->m_player_list[conId]; cNetGamePlayer && cNetGamePlayer->IsConnected()) {
					return cNetGamePlayer;
				}
			}
			return nullptr;
		}
		CNetGamePlayer* getCNetGamePlayerViaPeerAddress(uint64_t peerAddress) {
			if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
				for (uint16_t i{ cNetworkPlayerMgr->m_player_count }; i; --i) {
					if (auto cNetGamePlayer = cNetworkPlayerMgr->m_player_list[i]; cNetGamePlayer && cNetGamePlayer->IsConnected()) {
						if (auto rlGamerInfo = cNetGamePlayer->GetGamerInfo()) {
							if (rlGamerInfo->m_peer_address == peerAddress) {
								return cNetGamePlayer;
							}
						}
					}
				}
			}
			return nullptr;
		}
		CNetGamePlayer* getCNetGamePlayerViaPeerId(uint64_t peerId) {
			if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
				for (auto& cNetGamePlayer : cNetworkPlayerMgr->m_player_list) {
					if (cNetGamePlayer && cNetGamePlayer->IsConnected()) {
						if (auto rlGamerInfo = cNetGamePlayer->GetGamerInfo(); rlGamerInfo)
							if (rlGamerInfo->m_platform_data == peerId)
								return cNetGamePlayer;
					}
				}
			}
			return nullptr;
		}
		CNetGamePlayer* getCNetGamePlayerViaNetMsgId(uint32_t msgId) {
			if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
				for (auto& cNetGamePlayer : cNetworkPlayerMgr->m_player_list) {
					if (cNetGamePlayer && cNetGamePlayer->IsConnected()) {
						if (cNetGamePlayer->m_msg_id == msgId)
							return cNetGamePlayer;
					}
				}
			}
			return nullptr;
		}
		CNetGamePlayer* getRandomCNetGamePlayer(bool excludeSelf, bool excludeFriends) {
			if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
				for (auto& cNetGamePlayer : cNetworkPlayerMgr->m_player_list) {
					if (cNetGamePlayer && cNetGamePlayer->IsConnected()) {
						if (auto rlGamerInfo = cNetGamePlayer->GetGamerInfo(); rlGamerInfo) {
							if (excludeSelf && cNetGamePlayer == player::g_networkHandle)
								continue;
							if (excludeFriends) {
								for (uint32_t i{ pointers::g_friendRegistry->m_friend_count }; i; --i) {
									if (auto cFiendInfo = pointers::g_friendRegistry->get(i); cFiendInfo) {
										if (rlGamerInfo->m_gamer_handle.m_rockstar_id == cFiendInfo->m_rockstar_id)
											continue;
									}
								}
							}
							return cNetGamePlayer;
						}
					}
				}
			}
			return nullptr;
		}
		CNetGamePlayer* getHostCNetGamePlayer() {
			if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
				for (auto& cNetGamePlayer : cNetworkPlayerMgr->m_player_list) {
					if (cNetGamePlayer && cNetGamePlayer->IsConnected() && cNetGamePlayer->IsNetworkHost()) {
						return cNetGamePlayer;
					}
				}
			}
			return nullptr;
		}
		Network* get() {
			return *pointers::g_network;
		}
		uint16_t getPlayerCount() {
			if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
				return cNetworkPlayerMgr->m_player_count;
			}
			return 0ui16;
		}
		bool isSessionActive() {
			return player::g_entityHandle != -1 && player::g_networkHandle && player::g_networkHandle->IsConnected();
		}
	}
	namespace entity {
		Entity getEntityFromHandle(rage::CEntity* handle) {
			if (handle)
				return pointers::g_pointerToHandle(handle);
			return NULL;
		}
		rage::CEntity* getCEntityFromHandle(Entity handle) {
			if (handle)
				return pointers::g_handleToPointer(handle);
			return nullptr;
		}
	}
	namespace player {
		Player getLocalPlayerHandle() {
			if (auto cNetGamePlayer = getLocalCNetGamePlayer(); cNetGamePlayer) {
				return static_cast<Player>(cNetGamePlayer->m_player_id);
			}
			return network::isSessionActive() ? -1 : NULL;
		}
		CNetGamePlayer* getLocalCNetGamePlayer() {
			if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
				if (auto cNetGamePlayer = cNetworkPlayerMgr->m_local_net_player; cNetGamePlayer && cNetGamePlayer->IsConnected()) {
					return cNetGamePlayer;
				}
			}
			return nullptr;
		}
		CPlayerInfo* getLocalCPlayerInfo() {
			return ped::g_handle->m_player_info;
		}
		void tick() {
			g_handle = getLocalCPlayerInfo();
			g_entityHandle = getLocalPlayerHandle();
			g_networkHandle = getLocalCNetGamePlayer();
		}
	}
	namespace ped {
		CPed* getLocalCPed() {
			if (auto& cPedFactory = *pointers::g_pedFactory; cPedFactory) {
				if (auto& cPed = cPedFactory->m_local_ped; cPed) {
					return cPed;
				}
			}
			return nullptr;
		}
		Ped getLocalPedHandle() {
			if (auto cPed = getLocalCPed(); cPed)
				return entity::getEntityFromHandle(cPed);
			return NULL;
		}
		void tick() {
			g_handle = getLocalCPed();
			g_entityHandle = getLocalPedHandle();
		}
	}
	namespace vehicle {
		CVehicle* getLocalCVehicle() {
			return ped::g_handle->m_vehicle;
		}
		Vehicle getLocalVehicleHandle() {
			return entity::getEntityFromHandle(vehicle::g_handle);
		}
		rage::CBaseModelInfo* getModelData(uint32_t hash) {
			auto modelTble = pointers::g_modelTable;
			for (auto i = modelTble->m_lookup_table[hash % modelTble->m_lookup_key]; i; i = i->m_next)
				if (i->m_hash == hash)
					if (auto model = modelTble->m_data[i->m_idx]; model)
						return model;
			return nullptr;
		}
		void tick() {
			g_handle = getLocalCVehicle();
			g_entityHandle = getLocalVehicleHandle();
		}
	}
	void tick() {
		while (true) {
			ped::tick();
			player::tick();
			vehicle::tick();
			fiber::cur()->wait();
		}
	}
}