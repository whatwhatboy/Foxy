#include "Features.hpp"
#include "UI/Manager/DXUiManager.hpp"
#include "HTTP/HTTPRequest.hpp"
#include "nlohmann/json.hpp"
#include "R.A.G.Engine/fwuiInputMessageBase.hpp"
#include "R.A.G.Engine/CGameInvite.hpp"
#include "R.A.G.Engine/CMsgTextMessage.hpp"
#include "R.A.G.Engine/grcTextureStore.hpp"
#include "R.A.G.Engine/grcTextureFactory.hpp"
#include "Fibers/Fiber.hpp"
#include "Fibers/FiberMgr.hpp"
#include "Fibers/FiberPool.hpp"
#include "Fibers/Threading.hpp"
#include "Hooking/Hooking.hpp"
#include "Util/Util.hpp"
#include <Util/Packet.hpp>
#include <HTTP/HTTPSRequest.hpp>
#include "R.A.G.Engine/HashKeys.hpp"

namespace base::features {
	//Self
	void godMode() {
		if (util::ped::g_handle)
			ENTITY::SET_ENTITY_INVINCIBLE(util::ped::g_entityHandle, godModeBool);
	}
	void neverWanted() {
		if (util::player::g_handle) {
			util::player::g_handle->m_is_wanted = neverWantedBool;
			if (neverWantedBool)
				util::player::g_handle->m_wanted_level_display = util::player::g_handle->m_wanted_level = 0;
		}
	}
	void invisibility() {
		if (util::ped::g_handle) {
			ENTITY::SET_ENTITY_VISIBLE(util::ped::g_entityHandle, invisibilityBool ? (!util::network::isSessionActive() && locallyVisbleBool ? true : false) : true, FALSE);
			if (locallyVisbleBool)
				NETWORK::SET_LOCAL_PLAYER_VISIBLE_LOCALLY(true);
		}
	}
	void offRadar() {
		if (util::network::isSessionActive()) {
			global(2657589).Add(util::player::g_entityHandle, 466).Add(210).Value()->Int = offRadarBool;
			global(2672505).Add(57).Value()->Int =  NETWORK::GET_NETWORK_TIME() + (offRadarBool ? 0xB8E10 : NULL);
		}
	}
	void instantRespawn() {
		if (instantRespawnBool) {
			auto pos = util::ped::g_handle->get_position();
			if (PED::IS_PED_DEAD_OR_DYING(util::ped::g_entityHandle, TRUE)) {
				if (!util::network::isSessionActive() && CUTSCENE::IS_CUTSCENE_ACTIVE()) {
					CUTSCENE::STOP_CUTSCENE(FALSE);
				}
				PED::RESURRECT_PED(util::ped::g_entityHandle);
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(util::ped::g_entityHandle, Vector3(pos.x, pos.y, pos.x), FALSE, FALSE, FALSE);
			}
		}
	}
	void wantedLevel() {
		if (util::player::g_handle) {
			util::player::g_handle->m_wanted_level_display = util::player::g_handle->m_wanted_level = wantedLevelNumber;
			util::player::g_handle->m_is_wanted = util::player::g_handle->m_wanted_level > 0;
		}
	}
	void fastSwim() {
		if (util::player::g_handle)
			util::player::g_handle->m_swim_speed = fastSwimBool ? fastSwimSpeed : 1.f;
	}
	void fastRun() {
		if (util::player::g_handle)
			util::player::g_handle->m_run_speed = fastRunBool ? fastRunSpeed : 1.f;
	}
	void noRagdoll() {
		if (util::ped::g_handle) {
			if (noRagdollBool)
				PED::SET_RAGDOLL_BLOCKING_FLAGS(util::ped::g_entityHandle, 2);
			else
				PED::CLEAR_RAGDOLL_BLOCKING_FLAGS(util::ped::g_entityHandle, 2);
			PED::SET_PED_CAN_RAGDOLL(util::ped::g_entityHandle, features::noRagdollBool);
		}
	}
	void noClip() {
		ENTITY::SET_ENTITY_COLLISION(util::ped::g_entityHandle, !noClipBool, FALSE);
		if (util::ped::g_handle && noClipBool) {
			Vector3 pos{ util::ped::g_handle->get_position().serialize() };
			Vector3 rot{ math::rotToDir(CAM::GET_GAMEPLAY_CAM_ROT(0)) };
			Vector3 camCoords{ pos + rot * noClipMultiplier };
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(util::ped::g_entityHandle, pos, true, true, true);
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, static_cast<int>(eControllerInputs::INPUT_MOVE_UP_ONLY)))
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(util::ped::g_entityHandle, camCoords, true, true, true);
		}
	}
	void superRun() {
		if (superRunBool && util::ped::g_handle && (PAD::IS_DISABLED_CONTROL_PRESSED(2, 32) && PAD::IS_DISABLED_CONTROL_PRESSED(2, 21))) {
			ENTITY::APPLY_FORCE_TO_ENTITY(util::ped::g_entityHandle, 1, Vector3(0.f, 1.3f, 0.f), Vector3(), 0, true, true, true, false, false);
			PLAYER::SET_PLAYER_SPRINT(util::player::g_entityHandle, true);
			PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(util::player::g_entityHandle, 5.f);
		}
	}
	void superJump() {
		if (superJumpBool && util::ped::g_handle && util::ped::g_handle->m_ped_task_flag & (int)ePedTask::TASK_FOOT && !ENTITY::IS_ENTITY_IN_AIR(util::ped::g_entityHandle) && !PED::IS_PED_DOING_A_BEAST_JUMP(util::ped::g_entityHandle)) {
			if (PAD::IS_CONTROL_JUST_PRESSED(2, 22))
				TASK::TASK_JUMP(util::ped::g_entityHandle, true, true, true);
		}
	}
	//Weapon
	void infiniteAmmo() {
		if (util::ped::g_handle) {
			if (auto cPedInventory = util::ped::g_handle->m_inventory; cPedInventory) {
				cPedInventory->m_infinite_ammo = infiniteAmmoBool;
				cPedInventory->m_infinite_clip = infiniteAmmoBool;
			}
		}
	}
	void deleteGun() {
		if (auto ray = entity::raycast(); deleteGunBool && ray.check(5000.f) && PED::IS_PED_SHOOTING(util::ped::g_entityHandle)) {
			Vector3 coords{ ENTITY::GET_ENTITY_COORDS(ray.ent, true) };
			if (ENTITY::IS_ENTITY_A_PED(ray.ent) && PED::IS_PED_A_PLAYER(ray.ent)) {
				g_dxUiManager.sendNotify("Weapons", "You can't delete players!");
			}
			else {
				float distance{ math::distanceBetweenVecs(util::ped::g_handle->get_position().serialize(), coords) };
				if (distance >= 5000.f) {
					g_dxUiManager.sendNotify("Weapons", "The entity you're trying to delete is too far!");
				}
				else {
					if (entity::takeControlOf(ray.ent)) {
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ray.ent, true, true);
						ENTITY::DELETE_ENTITY(&ray.ent);
					}
					else {
						g_dxUiManager.sendNotify("Weapons", "Failed to take control of entity.");
					}
				}
			}
		}
	}
	void teleportGun() {
		if (auto ray = entity::raycast(); teleportGunBool && ray.check(5000.f) && PED::IS_PED_SHOOTING(util::ped::g_entityHandle)) {
			rage::vector3 pos{ ray.end.deserialize() };
			util::ped::g_handle->Warp(&pos, util::ped::g_handle->get_heading(), true);
			LOG_DEBUG("{}, {}, {}", ray.surface.x, ray.surface.y, ray.surface.z);
		}
	}
	void explosiveAmmo() {
		if (util::ped::g_handle) {
			if (auto weaponMgr = util::ped::g_handle->m_weapon_manager; weaponMgr) {
				if (auto cWeaponInfo = weaponMgr->m_weapon_info; cWeaponInfo) {
					if (auto cAmmoInfo = cWeaponInfo->m_ammo_info; cAmmoInfo) {
						static eAmmoSpecialType specialAmmoType = cAmmoInfo->m_ammo_special_type;
						cAmmoInfo->m_ammo_special_type = explosiveAmmoBool ? eAmmoSpecialType::Explosive : specialAmmoType;
					}
				}
			}
		}
	}
	void rapidFire() {
		if (rapidFireBool && util::ped::g_handle && !util::vehicle::g_handle) {
			Vector3 gpCam{ CAM::GET_GAMEPLAY_CAM_COORD() }, gpRot{ CAM::GET_GAMEPLAY_CAM_ROT(0) }, gpDirection{ math::rotToDir(gpRot) };
			Vector3 start{ gpCam + gpDirection }, end{ start + gpDirection * 500.f };
			if (PAD::IS_DISABLED_CONTROL_PRESSED(2, static_cast<int>(eControllerInputs::INPUT_FRONTEND_RT))) {
				*util::ped::g_handle->m_weapon_manager->m_weapon_object->GetCameraOffset() = end.deserialize();
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(start, end, 50, true, util::ped::g_handle->m_weapon_manager->m_selected_weapon_hash, util::ped::g_entityHandle, true, true, 1.f);
			}
		}
	}

	Hash getAmmoType(std::string ammoType) {
		static Hash ammo;
		ammo = MISC::GET_HASH_KEY(ammoType.c_str());
		WEAPON::REQUEST_WEAPON_ASSET(ammo, 1, 1);
		while (!WEAPON::HAS_WEAPON_ASSET_LOADED(ammo))
		return ammo;
	}

	void customrapidFire() { //Finish the callings for selecting an id later on X_X
		if (customrapidFireBool && util::ped::g_handle && !util::vehicle::g_handle) {
			Vector3 gpCam{ CAM::GET_GAMEPLAY_CAM_COORD() }, gpRot{ CAM::GET_GAMEPLAY_CAM_ROT(0) }, gpDirection{ math::rotToDir(gpRot) };
			Vector3 start{ gpCam + gpDirection }, end{ start + gpDirection * 500.f };
			if (PAD::IS_DISABLED_CONTROL_PRESSED(2, static_cast<int>(eControllerInputs::INPUT_FRONTEND_RT))) {
				*util::ped::g_handle->m_weapon_manager->m_weapon_object->GetCameraOffset() = end.deserialize();
				for (auto WeaponsHash : hashes::RangeweaponHashes)
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(start, end, 50, true, rage::atStringHash(WeaponsHash), util::ped::g_entityHandle, true, true, 1.f);
			}
		}
	}

	void airstrikeGun() {
		if (airstrikeGunBool && util::ped::g_handle && !util::vehicle::g_handle) {
			if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) {
				Vector3 pos;
				if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &pos)) {
					Hash airStrike = getAmmoType("WEAPON_AIRSTRIKE_ROCKET");
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(pos, pos, 250, 1, airStrike, PLAYER::PLAYER_PED_ID(), 1, 0, .5f);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(pos, pos, 250, 1, airStrike, PLAYER::PLAYER_PED_ID(), 1, 0, .5f);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(pos, pos, 250, 1, airStrike, PLAYER::PLAYER_PED_ID(), 1, 0, .5f);
				}
			}
		}
	}
	void oneHitGun() {
		if (oneHitGunBool && util::ped::g_handle && !util::vehicle::g_handle) {
			Vector3 gpCam{ CAM::GET_GAMEPLAY_CAM_COORD() }, gpRot{ CAM::GET_GAMEPLAY_CAM_ROT(0) }, gpDirection{ math::rotToDir(gpRot) };
			Vector3 start{ gpCam + gpDirection }, end{ start + gpDirection * 500.f };
			if (PAD::IS_DISABLED_CONTROL_PRESSED(2, static_cast<int>(eControllerInputs::INPUT_FRONTEND_RT))) {
				*util::ped::g_handle->m_weapon_manager->m_weapon_object->GetCameraOffset() = end.deserialize();
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(start, end, 500, true, MISC::GET_HASH_KEY("WEAPON_REMOTESNIPER"), util::ped::g_entityHandle, true, true, 1.f);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(start, end, 500, true, MISC::GET_HASH_KEY("WEAPON_REMOTESNIPER"), util::ped::g_entityHandle, true, true, 1.f);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(start, end, 500, true, MISC::GET_HASH_KEY("WEAPON_REMOTESNIPER"), util::ped::g_entityHandle, true, true, 1.f);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(start, end, 500, true, MISC::GET_HASH_KEY("WEAPON_REMOTESNIPER"), util::ped::g_entityHandle, true, true, 1.f);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(start, end, 500, true, MISC::GET_HASH_KEY("WEAPON_REMOTESNIPER"), util::ped::g_entityHandle, true, true, 1.f);
			}
		}
	}
	void driveitGun() {
		if (driveitGunBool && util::ped::g_handle && !util::vehicle::g_handle) {
			Vector3 pos;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &pos)) {
				Vehicle shootedcar = VEHICLE::GET_CLOSEST_VEHICLE(pos, 3.0f, 0, 70);
				Ped todelete = VEHICLE::GET_PED_IN_VEHICLE_SEAT(shootedcar, -1, 1);
				if (ENTITY::DOES_ENTITY_EXIST(todelete)) {
					PED::DELETE_PED(&todelete);
					PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), shootedcar, -1);
				}
				else if (!ENTITY::DOES_ENTITY_EXIST(todelete)) {
					PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), shootedcar, -1);
				}
			}
			
		}
	}
	void customExplosionGun() {
		if (customExplosionGunBool && util::ped::g_handle && !util::vehicle::g_handle) {
			Ped PedID = PLAYER::PLAYER_PED_ID();
			Vector3 pos;
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PedID, &pos);
			FIRE::ADD_EXPLOSION(pos, explosionType, 200.f, true, false, 0.f, 0);
		}

		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &cur, 1)) {
			if (WEAPON::IS_WEAPON_VALID(cur)) {
				int maxAmmo;
				if (WEAPON::GET_MAX_AMMO(PLAYER::PLAYER_PED_ID(), cur, &maxAmmo)) {
					WEAPON::SET_PED_AMMO(PLAYER::PLAYER_PED_ID(), cur, maxAmmo, 1);
					maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(PLAYER::PLAYER_PED_ID(), cur, 1);
					if (maxAmmo > 0) WEAPON::SET_AMMO_IN_CLIP(PLAYER::PLAYER_PED_ID(), cur, maxAmmo);
				}
			}
		}
	}



	//Vehicle
	void customAcrobatics() {

		if (customAcrobaticsBool && util::vehicle::g_handle) {

		}

	}
	void vehGodMode() {
		if (vehGodModeBool && util::vehicle::g_handle) {
			ENTITY::SET_ENTITY_INVINCIBLE(util::vehicle::g_entityHandle, vehGodModeBool);
		}
	}
	void vehInvisibility() {
		if (vehInvisibilityBool && util::vehicle::g_handle) {
			ENTITY::SET_ENTITY_VISIBLE(util::vehicle::g_entityHandle, !vehInvisibilityBool, false);
		}
	}
	void fixLoop() {
		if (fixLoopBool && util::vehicle::g_handle) {
			VEHICLE::SET_VEHICLE_FIXED(util::vehicle::g_entityHandle);
			VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(util::vehicle::g_entityHandle);
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(util::vehicle::g_entityHandle, 0);
		}
	}
	void hornBoost() {
		if (hornBoostBool && util::vehicle::g_handle && PLAYER::IS_PLAYER_PRESSING_HORN(util::player::g_entityHandle) && entity::takeControlOf(util::vehicle::g_entityHandle)) {
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(util::vehicle::g_entityHandle, hornBoostSpeed);
		}
	}
	void spawnVehicle(vehicleData data) {
		auto veh = spawnVehicle(util::player::g_entityHandle, data.modelInfo->m_model);
		if (withPlateText)
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plateText);
		if (spawnInVehicle)
			PED::SET_PED_INTO_VEHICLE(util::ped::g_entityHandle, veh, -1);
		if (withNetParamaters)
			features::setNetParamsVeh(veh);
		if (asOwnedVehicle && util::network::isSessionActive())
			DECORATOR::DECOR_SET_INT(veh, xorstr_("Player_Vehicle"), NETWORK::NETWORK_HASH_FROM_PLAYER_HANDLE(util::player::g_entityHandle));
	}
	void speedLimit() {
		if (util::vehicle::g_handle) {
			if (auto cVehicleModelInfo = util::vehicle::g_handle->m_model_info; cVehicleModelInfo) {
				if (speedLimitBool)
					VEHICLE::SET_VEHICLE_MAX_SPEED(util::vehicle::g_entityHandle, speedLimitSpeed);
				else
					VEHICLE::SET_VEHICLE_MAX_SPEED(util::vehicle::g_entityHandle, VEHICLE::GET_VEHICLE_MODEL_ESTIMATED_MAX_SPEED(cVehicleModelInfo->m_model));
			}
		}
	}
	void instantBrake() {
		if (instantBrakeBool && util::vehicle::g_handle) {
			if (PAD::IS_CONTROL_PRESSED(2, static_cast<int>(eControllerInputs::INPUT_VEH_BRAKE))) {
				VEHICLE::SET_VEHICLE_BRAKE(util::vehicle::g_entityHandle, true);
				if (!VEHICLE::IS_VEHICLE_STOPPED(util::vehicle::g_entityHandle))
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(util::vehicle::g_entityHandle, 0.f);
				VEHICLE::SET_VEHICLE_BRAKE(util::vehicle::g_entityHandle, false);
			}
		}
	}
	void drift() {
		if (util::vehicle::g_handle) {
			VEHICLE::SET_VEHICLE_HANDBRAKE(util::vehicle::g_entityHandle, driftBool && PAD::IS_CONTROL_PRESSED(2, static_cast<int>(eControllerInputs::INPUT_VEH_BRAKE)));
			VEHICLE::SET_VEHICLE_REDUCE_GRIP(util::vehicle::g_entityHandle, driftBool);
		}
	}
	void setPrimaryColor(RGBA color) {
		VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(util::vehicle::g_entityHandle, color.r, color.g, color.b);
	}
	void setSecondaryColor(RGBA color) {
		VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(util::vehicle::g_entityHandle, color.r, color.g, color.b);
	}
	void setColors(RGBA primary, RGBA secondary) {
		setPrimaryColor(primary);
		setSecondaryColor(secondary);
	}
	int* hsiToRgb(float H, float S, float I) {
		int rgb[3];
		if (H > 360) H = H - 360;
		H = fmod(H, 360);
		H = 3.14159 * H / (float)180;
		S = S > 0 ? (S < 1 ? S : 1) : 0;
		I = I > 0 ? (I < 1 ? I : 1) : 0;
		if (H < 2.09439) {
			rgb[0] = 255 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
			rgb[1] = 255 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
			rgb[2] = 255 * I / 3 * (1 - S);
		}
		else if (H < 4.188787) {
			H = H - 2.09439;
			rgb[1] = 255 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
			rgb[2] = 255 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
			rgb[0] = 255 * I / 3 * (1 - S);
		}
		else {
			H = H - 4.188787;
			rgb[2] = 255 * I / 3 * (1 + S * cos(H) / cos(1.047196667 - H));
			rgb[0] = 255 * I / 3 * (1 + S * (1 - cos(H) / cos(1.047196667 - H)));
			rgb[1] = 255 * I / 3 * (1 - S);
		}
		return rgb;
	}
	struct rainbow : public RGBA {
		void cycle(float speed, bool flipOrder = false) {
			static std::chrono::system_clock::time_point timerStart;
			std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsedSecondsFromLastTick = currentTime - timerStart;
			double ammountOfSecondsPassed = elapsedSecondsFromLastTick.count();
			static int i{};
			static int* rgb;
			if (ammountOfSecondsPassed > (float)0 / speed) {
				rgb = hsiToRgb(i, 1, 1);
				i++;
			}
			r = rgb[flipOrder ? 2 : 0];
			g = rgb[1];
			b = rgb[flipOrder ? 0 : 2];
		}
	};
	rainbow g_rainbow{};
	void rainbowVehicle() {
		if (rainbowVehicleBool) {
			g_rainbow.cycle(1000.f);
			setColors(g_rainbow, g_rainbow);
		}
	}
	//Spawn Helpers
	Vehicle spawnVehicle(Player player, Hash model) {
		STREAMING::HAS_MODEL_LOADED(model);
		if (!STREAMING::HAS_MODEL_LOADED(model)) {
			STREAMING::REQUEST_MODEL(model);
			fiber::cur()->wait(10ms);
		}
		auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(player), true);
		return VEHICLE::CREATE_VEHICLE(model, pos, 0.f, true, false, false);
	}
	bool setNetParamsVeh(Vehicle veh) {
		if (util::network::isSessionActive()) {
			DECORATOR::DECOR_SET_INT(veh, xorstr_("MPBitset"), 0);
			ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(veh, true);
			auto netId = NETWORK::VEH_TO_NET(veh);
			if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
				NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(netId, true);
			VEHICLE::SET_VEHICLE_IS_STOLEN(veh, false);
			return true;
		}
		return false;
	}
	Ped spawnPed(Player player, Hash model) {
		STREAMING::HAS_MODEL_LOADED(model);
		if (!STREAMING::HAS_MODEL_LOADED(model)) {
			STREAMING::REQUEST_MODEL(model);
			fiber::cur()->wait(10ms);
		}
		auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(player), true);
		auto ped = PED::CREATE_PED(ePedType::PED_TYPE_CRIMINAL, model, pos, 0.f, true, false);
		return ped;
	}
	bool setNetParamsPed(Ped ped) {
		if (util::network::isSessionActive()) {
			DECORATOR::DECOR_SET_INT(ped, xorstr_("MPBitset"), 0);
			ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(ped, true);
			auto netId = NETWORK::PED_TO_NET(ped);
			if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(ped))
				NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(netId, true);
			return true;
		}
		return false;
	}
	Object spawnObj(Player player, Hash model) {
		STREAMING::HAS_MODEL_LOADED(model);
		if (!STREAMING::HAS_MODEL_LOADED(model)) {
			STREAMING::REQUEST_MODEL(model);
			fiber::cur()->wait(10ms);
		}
		auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(player), true);
		auto obj = OBJECT::CREATE_OBJECT(model, pos, false, true, false);
		return obj;
	}
	bool setNetParamsObj(Object obj) {
		if (util::network::isSessionActive()) {
			DECORATOR::DECOR_SET_INT(obj, xorstr_("MPBitset"), 0);
			ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(obj, true);
			auto netId = NETWORK::OBJ_TO_NET(obj);
			if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(obj))
				NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(netId, true);
			return true;
		}
		return false;
	}
	//Network
	namespace scapi {
		void stringToRid(std::string str, std::string notifyTitle, uint64_t& rid) {
			if (isNumber(str)) {
				rid = stoi(str);
				if (!rid) {
					g_dxUiManager.sendNotify(notifyTitle, xorstr_("Please enter a valid RID!"));
					return;
				}
			}
			else {
				if (str.empty()) {
					g_dxUiManager.sendNotify(notifyTitle, xorstr_("Please enter a name, the value cannot be empty"));
					return;
				}
				if (str.size() >= 20) {
					g_dxUiManager.sendNotify(notifyTitle, xorstr_("The name you entered is above 20 characters. Please enter a name within the required length"));
					return;
				}
				rid = features::scapi::nameToRid(str);
				if (!rid) {
					g_dxUiManager.sendNotify(notifyTitle, xorstr_("There is no Rockstar ID linked to the name '{}'. Please enter a valid name"), str);
					return;
				}
			}
			if (!features::scapi::isRidOnline(rid))
				rid = NULL;
			if (!rid) {
				g_dxUiManager.sendNotify(notifyTitle, xorstr_("The RID you entered is invalid"));
				return;
			}
		}
		nlohmann::json sendRequest(nlohmann::json body, std::string endpoint) {
			curlWrapper curl{};
			std::vector<std::string> headers = {
				"X-Requested-With: XMLHttpRequest",
				"Content-Type: application/json; charset=utf-8",
				std::format("Authorization: SCAUTH val=\"{}\"", pointers::g_socialClubInfo->m_ticket)
			};
			std::string response{ curl.post(endpoint, body.dump(), headers) };
			nlohmann::json j{ nlohmann::json::parse(response) };
			if (j["Status"]) {
				return j;
			}
			return {};
		}
		bool isRidOnline(uint64_t rid) {
			return false;
		}
		std::string ridToName(uint64_t rid) {
			if (nlohmann::json json = sendRequest({ { xorstr_("RockstarId"), std::to_string(rid) } }, xorstr_("https://scui.rockstargames.com/api/friend/getprofile")); !json.is_null()) {
				for (auto& acc : json[xorstr_("Accounts")]) {
					if (auto& r_acc = acc[xorstr_("RockstarAccount")]; !r_acc.is_null()) {
						if (rid == r_acc[xorstr_("RockstarId")].get<uint64_t>()) {
							return r_acc[xorstr_("Name")].get<std::string>();
						}
					}
				}
			}
			else {
				g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Account wasn't found! Please enter a valid RID"));
			}
			return {};
		}
		std::pair<std::vector<account>, result> findPlayer(std::string name, int page) {
			return {};
		}
		uint64_t nameToRid(std::string name) {
			nlohmann::json json = sendRequest({ { xorstr_("searchNickname"), name } }, xorstr_("https://scui.rockstargames.com/api/friend/accountsearch"));
			if (name.size() <= 20) {
				if (auto numAccs = json[xorstr_("Total")].get<int>(); numAccs > 0) {
					return json[xorstr_("Accounts")][0][xorstr_("RockstarId")].get<uint64_t>();
				}
				else {
					g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("{} wasn't found. Please ensure there are no spelling mistakes"), name);
				}
			}
			else {
				g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("The character count cannot exceed 20, please shorten the value"));
			}
			return 0;
		}
	}
	void getSessionFromGamerHandle(rage::rlGamerHandle& handle, std::function<void(rage::rlSessionByGamerTaskResult, bool)> callback) {
		g_fiberPool.queue([&handle, &callback] {
			bool success = false;
			auto state = NULL;
			rage::rlSessionByGamerTaskResult taskRes;
			if (pointers::g_getSessionByGamerHandle(NULL, &handle, 1, &taskRes, 1, &success, &state)) {
				while (state == 1)
					fiber::cur()->wait(5ms);
				callback(taskRes, state == 3 && success ? true : false);
			}
		});
	}
	std::string getGsInfo() {
		char output[0x100]{};
		if (util::network::isSessionActive()) {
			base::pointers::g_getGsInfoFromSessionInfo(&util::network::session::get().m_rline_session.m_session_info, output, 0x7D, nullptr);
		}
		return output;
	}
	void ridJoinJBGSI(char const* gsinfo, bool encrypted) {
		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("maintransition"_joaat) != 0 || STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS()) {
			g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Failed to join session due to you already being in transistion"));
			return;
		}
		g_fiberPool.queue([gsinfo, encrypted] {
			rage::rlSessionInfo session{};
			features::m_data.isQueued = gsinfo;
			if (!features::m_data.isQueued) {
				g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Failed to join session due to the sessin info decode failing"));
				return;
			}
			features::m_data.sessionInfo = session;
			features::sessionJoin(1);
			fiber::cur()->wait(350ms);
			if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("maintransition"_joaat) == 0) {
				features::m_data.isQueued = false;
				g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Failed to join session due to maintransition failing to launch"));
				return;
			}
		});
	}
	void ridJoinSIAPE(uint64_t rid) {
		m_data.gamerHandle = { rid };
		//static auto playerName = scapi::ridToName(rid);
		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("maintransition"_joaat) != 0 || STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS()) {
			g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Failed to join due to you already being in transistion"));
			return;
		}
		bool wasSuccess{};
		getSessionFromGamerHandle(m_data.gamerHandle, [&](rage::rlSessionByGamerTaskResult result, bool success) {
			if (success) {
				rage::rlPresenceEventInviteAccepted invite(util::player::g_handle->m_gamer_info, result.m_session_info, result.m_gamer_handle);
				pointers::g_sendInviteAcceptedPresenceEvent(pointers::g_presenceStruct, &invite, 0x21);
				while (!HUD::IS_WARNING_MESSAGE_ACTIVE()) {
					fiber::cur()->wait(10ms);
				}
				PAD::SET_CONTROL_VALUE_NEXT_FRAME(2, static_cast<int>(eControllerInputs::INPUT_FRONTEND_ACCEPT), 1.f);
				wasSuccess = true;
			}
		});
		if (!wasSuccess)
			return;
		//if (scapi::isRidOnline(rid))
		//	g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Failed to join {} due to unknown reasons"), playerName);
		//else
		//	g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Failed to join {} due to the player being offline"), playerName);
	}
	void ridJoinJBSI(uint64_t rid) {
		m_data.gamerHandle = { rid };
		static auto playerName = scapi::ridToName(rid);
		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("maintransition"_joaat) != 0 || STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS()) {
			g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Failed to join {} due to you already being in transistion"), playerName);
			return;
		}
		bool wasSuccess{};
		getSessionFromGamerHandle(m_data.gamerHandle, [&](rage::rlSessionByGamerTaskResult result, bool success) {
			if (success) {
				m_data.isQueued = true;
				m_data.sessionInfo = result.m_session_info;
				sessionJoin(1);
				fiber::cur()->wait(350ms);
				if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH("maintransition"_joaat) == 0) {
					m_data.isQueued = false;
					g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Failed to join {} due to maintransition failing to launch"), playerName);
				}
				wasSuccess = true;
			}
		});
		if (!wasSuccess)
			return;
		if (scapi::isRidOnline(rid))
			g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Failed to join {} due to unknown reasons"), playerName);
		else
			g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Failed to join {} due to the player being offline"), playerName);
	}
	uint64_t g_tmpRid{};
	std::string g_tmpFbrName{};
	void ridJoinCFR(uint64_t rid) {

	}
	void ridJoin(uint64_t rid, int type) {
		switch (type) {
		case 0: {
			ridJoinJBSI(rid);
		} break;
		case 1: {
			ridJoinSIAPE(rid);
		} break;
		case 2: {
			ridJoinCFR(rid);
		} break;
		}
	}
	void sessionJoin(int type, bool correctPassedType) {
		if (type == 11) {
			NETWORK::NETWORK_BAIL(0, 0, 16);
			return;
		}
		static int correctedType{ type };
		if (correctPassedType) {
			switch (correctedType) {
			case 4: correctedType = 6; break;
			case 5: correctedType = 9; break;
			case 6: correctedType = 10; break;
			case 7: correctedType = 11; break;
			case 8: correctedType = 12; break;
			case 9: correctedType = 13; break;
			case 10: correctedType = -1; break;
			default: correctedType = type; break;
			}
		}
		g_fiberPool.queue([] {
			global(2695915).Value()->Int = correctedType == 13;
			global(correctedType == -1 ? 1574589 + 2 : 1575017).Value()->Int = correctedType;
			global(1574589).Value()->Int = true;
			fiber::cur()->wait(200ms);
			global(1574589).Value()->Int = false;
		});
	}
	int region(int type, bool correctPassedType) {
		static int correctedType{ type };
		if (correctPassedType) {
			switch (correctedType) {
			case 0: correctedType = 2; break;
			case 1: correctedType = 6; break;
			case 2: correctedType = 3; break;
			case 3: correctedType = 5; break;
			case 5: correctedType = 7; break;
			case 6: correctedType = 0; break;
			default: correctedType = type; break;
			}
		}
		return correctedType;
	}
	int language(int type, bool correctPassedType) {
		static int correctedType{ type };
		if (correctPassedType) {
			switch (correctedType) {
			case 3: correctedType = 11; break;
			case 5: correctedType = 9; break;
			case 6: correctedType = 12; break;
			case 7: correctedType = 5; break;
			case 8: correctedType = 10; break;
			case 9: correctedType = 7; break;
			case 10: correctedType = 8; break;
			case 11: correctedType = 6; break;
			case 12: correctedType = 3; break;
			default: correctedType = type; break;
			}
		}
		return correctedType;
	}
	void drawChat(char const* msg, uint32_t playerId, bool teamChat, RGBA color) {

	}
	void chatSender(std::string message, bool isTeam) {

	}
	void chatSpam() {
		if (chatSpamBool)
			chatSender(chatSpamMsg, chatSpamInTeamBool);
	}
	void chatAsSomeoneElse() {
		if (chatAsSomeoneElseBool[g_selectedPlayer])
			personToSpoofAs = g_selectedPlayer;
		else
			personToSpoofAs = util::player::g_entityHandle;
	}
	namespace matchmaking {
		bool triggerSessionFind(std::optional<uint32_t> constraint) {
			for (auto& session : sessions)
				session.valid = true;
			NetworkGameFilterMatchmakingComponent component{};
			strcpy(component.m_filter_name, xorstr_("Group"));
			component.m_game_mode = 0;
			component.m_num_parameters = 0;
			if (filters::region::toggle)
				component.SetParameter(xorstr_("MMATTR_REGION"), 0, filters::region::value);
			if (constraint)
				component.SetParameter(xorstr_("MMATTR_DISCRIMINATOR"), 1, *constraint);
			int state{};
			static rage::rlSessionInfo results[1400]{};
			findingSessions = true;
			if (g_hooking->m_startMatchmakingFindSessionsHk.getOg<funcTypes::startMatchmakingFindSessionsT>()(0, 1, &component, 1400, results, &numberOfFoundSessions, &state)) {
				while (state == 1)
					fiber::cur()->wait(1ms);
				if (state == 3) {
					for (uint32_t i{}; i != numberOfFoundSessions; ++i) {
						sessions[i].info = results[i];
						if (constraint && sessions[i].attributes.playerCount >= 30)
							sessions[i].valid = false;
						if (filters::language::toggle && sessions[i].attributes.language != filters::language::value)
							sessions[i].valid = false;
						if (!filters::playerCount::toggle) {
							if (filters::playerCount::min::toggle && sessions[i].attributes.playerCount < filters::playerCount::min::value)
								sessions[i].valid = false;
							if (filters::playerCount::max::toggle && sessions[i].attributes.playerCount > filters::playerCount::max::value)
								sessions[i].valid = false;
						}
						else {
							if (sessions[i].attributes.playerCount != filters::playerCount::value)
								sessions[i].valid = false;
						}
					}
					findingSessions = false;
					return true;
				}
			}
			findingSessions = false;
			return false;
		}
	}
	// Selected Player
	void spectatePlayer() {
		if (util::network::isSessionActive()) {
			if (auto cNetGamePlayer = util::network::getCNetGamePlayerViaConnectionId(g_selectedPlayer); cNetGamePlayer) {
				if (auto cPlayerInfo = cNetGamePlayer->m_player_info; cPlayerInfo) {
					if (auto cPed = cPlayerInfo->m_ped; cPed) {
						global(2815059).Add(6753).Value()->Int = spectatePlayerBool[g_selectedPlayer];
						NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(spectatePlayerBool[g_selectedPlayer], util::entity::getEntityFromHandle(cPed));
						HUD::SET_MINIMAP_IN_SPECTATOR_MODE(spectatePlayerBool[g_selectedPlayer], util::entity::getEntityFromHandle(cPed));
					}
				}
			}
		}
	}
	constexpr rage::joaat_t customPUHash = 0x2C014CA6ui32;
	constexpr rage::joaat_t customMoneyDropHash = 0xE175C698ui32;
	void drop() {
		static timer t(0ms);
		t.setDelay(dropDelay * 1000.0);
		if (dropBool[g_selectedPlayer] && dropHash != NULL && t.update()) {
			if (auto cNetGamePlayer = util::network::getCNetGamePlayerViaConnectionId(g_selectedPlayer); cNetGamePlayer) {
				if (auto cPlayerInfo = cNetGamePlayer->m_player_info; cPlayerInfo) {
					if (auto cPed = cPlayerInfo->m_ped; cPed) {
						auto pos_NonSerialised = cPed->get_position();
						OBJECT::CREATE_AMBIENT_PICKUP(dropType ? customPUHash : customMoneyDropHash, Vector3(pos_NonSerialised.serialize()), NULL, dropValue, dropHash, false, true);
					}
				}
			}
		}
	}
	bool passHostAroundLikeACheerleader(CNetGamePlayer* player, uint32_t hash, std::function<void(CGameScriptHandlerNetComponent*)> onHostGive, std::function<void()> onHostGiven) {
		return false;
	}
	// All Players
	void dropAll() {
		static timer t(0ms);
		t.setDelay(dropAllDelay * 1000.0);
		if (dropAllBool && dropAllHash != NULL && t.update()) {
			if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr && cNetworkPlayerMgr->m_player_count) {
				for (auto& cNetGamePlayer : cNetworkPlayerMgr->m_player_list) {
					if (cNetGamePlayer && cNetGamePlayer->IsConnected()) {
						if (auto cPlayerInfo = cNetGamePlayer->m_player_info; cPlayerInfo) {
							if (auto cPed = cPlayerInfo->m_ped; cPed) {
								auto pos_NonSerialised = cPed->get_position();
								OBJECT::CREATE_AMBIENT_PICKUP(dropAllType ? customPUHash : customMoneyDropHash, Vector3(pos_NonSerialised.serialize()), NULL, dropValue, dropHash, false, true);
							}
						}
					}
				}
			}
		}
	}
	//Miscellaneous
	void setDispatchServiceToggle(eDispatchType service, bool toggle) {
		if (pointers::g_dispatchSerivceTable) {
			auto table = *reinterpret_cast<CDispatch*>(uint64_t(pointers::g_dispatchSerivceTable) + (static_cast<uint64_t>(service) * 8));
			table.m_is_active = toggle;
		}
	}
	void tickDispatchDisables() {
		setDispatchServiceToggle(eDispatchType::PoliceAutomobile, policeAutomobileDispatch);
		setDispatchServiceToggle(eDispatchType::PoliceHelicopter, policeHelicopterDispatch);
		setDispatchServiceToggle(eDispatchType::FireDepartment, fireDepartmentDispatch);
		setDispatchServiceToggle(eDispatchType::SwatAutomobile, swatAutomobileDispatch);
		setDispatchServiceToggle(eDispatchType::AmbulanceDepartment, ambulanceDepartmentDispatch);
		setDispatchServiceToggle(eDispatchType::PoliceRiders, policeRidersDispatch);
		setDispatchServiceToggle(eDispatchType::PoliceVehicleRequest, policeVehicleRequestDispatch);
		setDispatchServiceToggle(eDispatchType::PoliceRoadBlock, policeRoadBlockDispatch);
		setDispatchServiceToggle(eDispatchType::PoliceAutomobileWaitPulledOver, policeAutomobileWaitPulledOverDispatch);
		setDispatchServiceToggle(eDispatchType::PoliceAutomobileWaitCruising, policeAutomobileWaitCruisingDispatch);
		setDispatchServiceToggle(eDispatchType::Gangs, gangsDispatch);
		setDispatchServiceToggle(eDispatchType::SwatHelicopter, swatHelicopterDispatch);
		setDispatchServiceToggle(eDispatchType::PoliceBoat, policeBoatDispatch);
		setDispatchServiceToggle(eDispatchType::ArmyVehicle, armyVehicleDispatch);
		setDispatchServiceToggle(eDispatchType::BikerBackup, bikerBackupDispatch);
	}
	void mobileRadio() {
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(mobileRadioBool);
		AUDIO::SET_MOBILE_PHONE_RADIO_STATE(mobileRadioBool);
		AUDIO::SET_USER_RADIO_CONTROL_ENABLED(mobileRadioBool);
	}
	void thermalVision() {
		GRAPHICS::SET_SEETHROUGH(thermalVisionBool);
	}
	void bypassFiredLimit() {
		if (bypassStickyLimitBool) {
			util::ped::g_handle->m_fired_sticky_bombs = NULL;
		}
		if (bypassFlareLimitBool) {
			util::ped::g_handle->m_fired_flares = NULL;
		}
	}
	rage::pgDictionary* createDictionary() {
		return nullptr;
	}
	void addDictionaryToPool(rage::pgDictionary* dictionary, std::string name) {
	}
	rage::pgDictionary* findDictionary(std::string dictionary) {
		return nullptr;
	}
	rage::grcTexture* findTexture(std::string dictionary, std::string name) {
		return nullptr;
	}
	rage::grcTexture* createTexture(fs::path filepath) {
		rage::grcTexture* texture{};
		return texture;
	}
	auto g_requestDictionary = [](std::string dict) {
	};
	void handleTextureReplacements() {
	}
	void instantExit() {
		if (HUD::IS_WARNING_MESSAGE_ACTIVE()) {
			switch (HUD::GET_WARNING_SCREEN_MESSAGE_HASH()) {
			case "EXIT_SURE"_joaat: {
				if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, static_cast<int>(eControllerInputs::INPUT_FRONTEND_ACCEPT))) {
					LOG_DEBUG("Game exit requested");
					exit(0);
				}
			} break;
			case "EXIT_SURE_2"_joaat: {
				if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, static_cast<int>(eControllerInputs::INPUT_FRONTEND_ACCEPT))) {
					LOG_DEBUG("Game exit requested");
					exit(0);
				}
			} break;
			default: {} break;
			}
		}
	}
	//Tick
	void tick() {
		g_fiberMgr.add(xorstr_("cS"), [] {
			while (true) {
				features::chatSpam();
				fiber::cur()->wait(std::chrono::milliseconds(features::chatSpamDelayInMs));
			}
		});
		for (int i{}; i != 32; ++i) {
			chatAsSomeoneElseBool[i] = false;
			msgSyncWithPerson[i] = true;
			spectatePlayerBool[i] = false;
		}
		while (true) {
			g_isUsingKeyboard = PAD::IS_USING_KEYBOARD_AND_MOUSE(2);
			//Self
			godMode();
			neverWanted();
			invisibility();
			offRadar();
			instantRespawn();
			wantedLevel();
			fastSwim();
			fastRun();
			noRagdoll();
			noClip();
			superRun();
			superJump();
			//Weapon
			infiniteAmmo();
			deleteGun();
			teleportGun();
			explosiveAmmo();
			oneHitGun();
			customrapidFire();
			airstrikeGun();
			driveitGun();
			customExplosionGun();
			//Vehicle
			vehGodMode();
			vehInvisibility();
			fixLoop();
			hornBoost();
			speedLimit();
			instantBrake();
			drift();
			rainbowVehicle();
			customAcrobatics();
			//Network
			//chatAsSomeoneElse();
			// Selected Player
			spectatePlayer();
			drop();
			// All Players
			dropAll();
			//Miscellaneous
			tickDispatchDisables();
			mobileRadio();
			bypassFiredLimit();
			handleTextureReplacements();
			instantExit();
			fiber::cur()->wait();
		}
	}
}