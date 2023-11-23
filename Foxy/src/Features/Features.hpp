#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/Rage.hpp"
#include "Memory/Patterns/Pointers.hpp"
#include "R.A.G.Engine/Natives/Invoker/Invoker.hpp"
#include "R.A.G.Engine/Natives/Natives.hpp"
#include "Util/Globals.hpp"
#include "Fibers/Fiber.hpp"
#include "Fibers/FiberPool.hpp"
#include "R.A.G.Engine/Network.hpp"
#include "R.A.G.Engine/Colors.hpp"
#include "R.A.G.Engine/grcTexture.hpp"
#include "R.A.G.Engine/ScrHandlers.hpp"
#include "nlohmann/json.hpp"

namespace rage {
	class grcTexture;
	class grcTextureStored;
	class pgDictionary;
}
namespace base {
	namespace math {
		inline float degToRad(const float deg) {
			auto rad = (3.14159265359 / 180) * deg;
			return (float)rad;
		}
		inline float distance3D(Vector3 a, Vector3 b) {
			return (float)sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
		}
		inline Vector3 rotToDir(const Vector3 rot) {
			auto xRadian = degToRad(rot.x);
			auto zRadian = degToRad(rot.z);
			auto num = abs(cos(xRadian));
			return { (-sin(zRadian)) * num, cos(zRadian) * num, sin(xRadian) };
		}
		inline float distanceBetweenVecs(Vector3 a, Vector3 b) {
			return float(sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2)));
		}
	}
	namespace entity {
		inline bool takeControlOf(Entity ent) {
			if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(ent))
				return true;
			for (int32_t i{}; !NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(ent) && !NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(ent) && i != 30; ++i) {
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(ent);
				NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(ent);
				fiber::cur()->wait(1ms);
			}
			if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(ent))
				return false;
			NETWORK::SET_NETWORK_ID_CAN_MIGRATE(NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(ent), true);
			return true;
		}
		struct raycast {
			Vector3 end{}, surface{};
			Entity ent{};

			bool check(float distance) {
				BOOL hit{};
				Vector3 camCoords{ CAM::GET_GAMEPLAY_CAM_COORD() };
				Vector3 camRotation{ CAM::GET_GAMEPLAY_CAM_ROT(2) };
				Vector3 dir{ math::rotToDir(camRotation) };
				Vector3 dist{ camCoords + dir * distance };
				int ray{ SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(camCoords, dist, -1, ent, 7) };
				SHAPETEST::GET_SHAPE_TEST_RESULT(ray, &hit, &end, &surface, &ent);
				return hit;
			}
		};
	}
	inline bool g_isUsingKeyboard{};
	namespace features {
		//Self
		inline bool godModeBool{};
		extern void godMode();
		inline bool neverWantedBool{};
		extern void neverWanted();
		inline bool invisibilityBool{};
		inline bool locallyVisbleBool{};
		extern void invisibility();
		inline bool offRadarBool{};
		extern void offRadar();
		inline bool instantRespawnBool{};
		extern void instantRespawn();
		inline uint32_t wantedLevelNumber{};
		extern void wantedLevel();
		inline float fastSwimSpeed{ 2.f };
		inline bool fastSwimBool{};
		extern void fastSwim();
		inline float fastRunSpeed{ 2.f };
		inline bool fastRunBool{};
		extern void fastRun();
		inline bool noRagdollBool{};
		extern void noRagdoll();
		inline bool noClipBool{};
		static float noClipMultiplier{ 0.5f };
		extern void noClip();
		inline bool superRunBool{};
		extern void superRun();
		inline bool superJumpBool{};
		extern void superJump();
		//Weapon
		inline bool infiniteAmmoBool{};
		extern void infiniteAmmo();
		inline bool deleteGunBool{};
		extern void deleteGun();
		inline bool teleportGunBool{};
		extern void teleportGun();
		inline bool explosiveAmmoBool{};
		extern void explosiveAmmo();
		inline bool rapidFireBool{};
		extern void rapidFire();
		inline bool oneHitGunBool{};
		extern void oneHitGun();
		inline bool customrapidFireBool{};
		extern void customrapidFire();
		inline bool airstrikeGunBool{};
		extern void airstrikeGun();
		inline bool driveitGunBool{};
		extern void driveitGun();
		static int explosionType{ 1 };
		inline bool customExplosionGunBool{};
		extern void customExplosionGun();
		

		//Vehicle
		inline bool vehGodModeBool{};
		extern void vehGodMode();
		inline bool vehInvisibilityBool{};
		extern void vehInvisibility();
		inline bool fixLoopBool{};
		extern void fixLoop();
		inline float hornBoostSpeed{};
		inline bool hornBoostBool{};
		extern void hornBoost();
		inline bool customAcrobaticsBool{};
		extern void customAcrobatics();
		struct vehicleData {
			vehicleData(const rage::CVehicleModelInfo* mi, const std::string ml, const std::string nl) : modelInfo(mi), manufacturerLabel(ml), nameLabel(nl), finalLabel((manufacturerLabel.compare("NULL") ? manufacturerLabel + " " : std::string()) + nameLabel) {}
			const rage::CVehicleModelInfo* modelInfo{};
			const std::string manufacturerLabel{}, nameLabel{}, finalLabel{};
		};
		inline char const* plateText{ xorstr_(BRANDING_NAME) };
		inline bool withPlateText{};
		inline bool withNetParamaters{ true };
		inline bool asOwnedVehicle{};
		inline bool spawnInVehicle{};
		extern void spawnVehicle(vehicleData data);
		inline bool speedLimitBool{};
		inline float speedLimitSpeed{ 1.f };
		extern void speedLimit();
		inline bool instantBrakeBool{};
		extern void instantBrake();
		inline bool driftBool{};
		extern void drift();
		extern void setPrimaryColor(RGBA color);
		extern void setSecondaryColor(RGBA color);
		extern void setColors(RGBA primary, RGBA secondary);
		inline bool rainbowVehicleBool{};
		extern void rainbowVehicle();
		//Spawn Helpers
		extern Vehicle spawnVehicle(Player player, Hash model);
		extern bool setNetParamsVeh(Vehicle veh);
		extern Ped spawnPed(Player player, Hash model);
		extern bool setNetParamsPed(Ped ped);
		extern Object spawnObj(Player player, Hash model);
		extern bool setNetParamsObj(Object obj);
		//Network
		inline bool chatFilter{};
		namespace spoofing {
			namespace clan {
				inline std::pair<std::string, bool> name{};
				inline std::pair<std::string, bool> tag{};
				inline std::pair<std::string, bool> motto{};
				inline std::pair<std::string, bool> rankName{};
				inline std::pair<std::string, bool> rankTitle{};
				inline std::pair<int, bool> color{};
				inline std::pair<int, bool> clanId{ 41564112, false };
				inline std::pair<int, bool> creationTime{};
				inline std::pair<int, bool> memberId{};
				inline std::pair<int, bool> memberCount{};
				inline bool privateCrew{};
			}
			inline std::pair<std::string, bool> name{};
			struct socketAddresses {
				std::pair<rage::netSocketAddress, std::pair<bool, bool>> relayAddr, publicAddr, localAddr;
			};
			inline socketAddresses socketAddress{};
			inline std::pair<uint64_t, bool> rid{};
			inline std::pair<uint64_t, bool> peerAddress{};
			inline std::pair<uint64_t, bool> peerId{};
			namespace flags {
				inline bool rockstarDev{};
				inline bool rockstarQA{};
				inline bool isCheater{};
			}
			inline bool hideEntityProofs{};
			inline bool hidePlayerSpectating{};
		}
		namespace scapi {
			struct account {
				std::string name;
				uint64_t rid{};
				bool online{};
			};
			struct result {
				int total;
				int nextOffset;
			};
			extern void stringToRid(std::string str, std::string notifyTitle, uint64_t& rid);
			extern nlohmann::json sendRequest(nlohmann::json body, std::string endpoint);
			extern bool isRidOnline(uint64_t rid);
			extern std::string ridToName(uint64_t rid);
			extern std::pair<std::vector<account>, result> findPlayer(std::string name, int page);
			extern uint64_t nameToRid(std::string name);
		}
		extern void getSessionFromGamerHandle(rage::rlGamerHandle& handle, std::function<void(rage::rlSessionByGamerTaskResult, bool)> callback);
		struct ridJoinData { bool isQueued; class rage::rlGamerHandle gamerHandle{ 0 }; class rage::rlSessionInfo sessionInfo; };
		inline ridJoinData m_data{};
		extern std::string getGsInfo();
		extern void ridJoinJBGSI(char const* gsinfo, bool encrypted = true);
		extern void ridJoinSIAPE(uint64_t rid);
		extern void ridJoinJBSI(uint64_t rid);
		extern void ridJoinCFR(uint64_t rid);
		extern void sendGameInvite(uint64_t rid);
		extern void ridJoin(uint64_t rid, int type);
		extern void sessionJoin(int type, bool correctPassedType = false);
		extern int region(int type, bool correctPassedType = false);
		extern int language(int type, bool correctPassedType = false);
		extern void drawChat(char const* msg, uint32_t playerId, bool teamChat, RGBA color = RGBA());
		extern void test(int reciever, std::string message);
		inline int personToSpoofAs{};
		extern void chatSender(std::string message, bool isTeam);
		inline int chatSpamDelayInMs = 100;
		inline std::string chatSpamMsg{};
		inline bool chatSpamInTeamBool{};
		inline bool chatSpamBool{};
		extern void chatSpam();
		inline bool blockAllMsgSyncs{};
		inline bool msgSyncWithPerson[32]{};
		inline bool chatAsSomeoneElseBool[32]{};
		extern void chatAsSomeoneElse();
		inline bool loadDevDLC{};
		inline bool joinInsideSCTVSlots{};
		namespace matchmaking {
			namespace filters {
				namespace language {
					inline int32_t value{};
					inline bool toggle{};
				}
				namespace region {
					inline uint32_t value{};
					inline bool toggle{};
				}
				namespace playerCount {
					namespace min {
						inline uint8_t value{ 1 };
						inline bool toggle{};
					}
					namespace max {
						inline uint8_t value{ 32 };
						inline bool toggle{};
					}
					inline uint8_t value{};
					inline bool toggle{};
				}
			}
			struct sessionAttributes {
				int32_t discriminator{};
				int32_t playerCount{};
				uint32_t region{};
				int32_t language{};
			};
			struct session {
				rage::rlSessionInfo info{}; sessionAttributes attributes{}; bool valid{};
			};
			inline uint32_t numberOfFoundSessions{};
			inline std::array<session, 1400> sessions{};
			inline bool replaceRSMatchmaking{};
			inline bool findingSessions{};
			extern bool triggerSessionFind(std::optional<uint32_t> constraint = std::nullopt);
		}
		// Selected Player
		inline bool spectatePlayerBool[32]{};
		extern void spectatePlayer();
		inline int dropType{};
		inline rage::joaat_t dropHash{};
		inline int32_t dropValue{ 2500 };
		inline double dropDelay{ 2.5 };
		inline bool dropBool[32]{};
		extern void drop();
		extern bool passHostAroundLikeACheerleader(CNetGamePlayer* player, uint32_t hash, std::function<void(CGameScriptHandlerNetComponent*)> onHostGive, std::function<void()> onHostGiven);
		struct exploitData {
			Any event; global handle;
		};
		class remoteGlobals {
		public:
			remoteGlobals(uint64_t index, exploitData data) : m_index(index), m_data(data) {}
			remoteGlobals at(uint64_t index) {
				return remoteGlobals(m_index + index, m_data);
			}
			remoteGlobals at(uint64_t index, uint64_t size) {
				return at(1 + (index * size));
			}
			std::pair<std::function<void(CGameScriptHandlerNetComponent*)>, std::function<void()>> get(CNetGamePlayer* target, Any value) {
				return std::make_pair([&](CGameScriptHandlerNetComponent* netComponent) { netComponent->register_host_broadcast_data(&(m_data.handle.Value()->Int), INT_MAX, NULL); }, [&] {
					Any data[4] = { m_data.event, target->m_player_id, m_index - m_data.handle.Index, value };
					pointers::g_triggerScriptEvent(1, data, sizeof(data), 1 << target->m_player_id);
				});
			}
			template <typename t>
			bool send(CNetGamePlayer* target, t value) {
				if (target && target->IsConnected()) {
					auto data = get(target, (Any)value);
					return passHostAroundLikeACheerleader(target, "freemode"_joaat, data.first, data.second);
				}
				return false;
			}
		public:
			uint64_t m_index{};
			exploitData m_data;
		};
		// All Players
		inline rage::joaat_t dropAllHash{};
		inline int dropAllType{};
		inline int32_t dropAllValue{ 2500 };
		inline double dropAllDelay{ 2.5 };
		inline bool dropAllBool{};
		extern void dropAll();
		//Miscellaneous
		inline bool policeAutomobileDispatch{ true };
		inline bool policeHelicopterDispatch{ true };
		inline bool fireDepartmentDispatch{ true };
		inline bool swatAutomobileDispatch{ true };
		inline bool ambulanceDepartmentDispatch{ true };
		inline bool policeRidersDispatch{ true };
		inline bool policeVehicleRequestDispatch{ true };
		inline bool policeRoadBlockDispatch{ true };
		inline bool policeAutomobileWaitPulledOverDispatch{ true };
		inline bool policeAutomobileWaitCruisingDispatch{ true };
		inline bool gangsDispatch{ true };
		inline bool swatHelicopterDispatch{ true };
		inline bool policeBoatDispatch{ true };
		inline bool armyVehicleDispatch{ true };
		inline bool bikerBackupDispatch{ true };
		extern void tickDispatchDisables();
		inline bool mobileRadioBool{};
		extern void mobileRadio();
		inline bool thermalVisionBool{};
		extern void thermalVision();
		inline bool bypassStickyLimitBool{};
		inline bool bypassFlareLimitBool{};
		extern void bypassFiredLimit();
		extern rage::pgDictionary* findDictionary(std::string dictionary);
		extern rage::grcTexture* findTexture(std::string dictionary, std::string name);
		class assetData {
		public:
			assetData(std::string d, std::string n) : dictionary(d), name(n) {}
			assetData(uint32_t d, std::string n) : dictionary(std::to_string(d)), name(n) {}
			assetData(std::string d) : assetData(d, std::string()) {}
			assetData(uint32_t d) : assetData(d, std::string()) {}
			assetData() : assetData(std::string()) {}
			uint32_t toDec(bool dict = true) {
				return std::stoul(dict ? dictionary : name);
			}
			bool shouldToDecBeUsed(bool dict = true) {
				return isNumber(dict ? dictionary : name) && toDec(dict);
			}
			bool valid() {
				return !dictionary.empty() && !name.empty();
			}
			bool exists() {
				if (!valid())
					return false;
				if (!findDictionary(dictionary))
					return false;
				if (!findTexture(dictionary, name))
					return false;
				return true;
			}
			std::string dictionary{}, name{};
		};
		class textureReplace {
		public:
			textureReplace(assetData asset, bool active, bool orginialSize, bool disableNotify) : m_asset(asset), m_active(active), m_swap(false), m_retainOrginialSizes(orginialSize), m_disableErrorNotify(disableNotify), m_texture({ nullptr, std::string() }) {}
			textureReplace(assetData asset, bool orginialSize, bool disableNotify) : textureReplace(asset, true, orginialSize, disableNotify) {}
			textureReplace(assetData asset, bool orginialSize) : textureReplace(asset, orginialSize, false) {}
			textureReplace(bool useOrginialSize) : textureReplace(assetData(), useOrginialSize) {}
			assetData m_asset{};
			bool m_active{}, m_swap{}, m_retainOrginialSizes{}, m_disableErrorNotify{};
			std::pair<rage::grcTexture*, std::string> m_texture{};
			rage::grcTextureStored m_original{};
		};
		inline std::vector<textureReplace> textureReplacements{};
		extern rage::grcTexture* createTexture(fs::path filepath);
		extern void handleTextureReplacements();
		//Tick
		extern void tick();
	}
}