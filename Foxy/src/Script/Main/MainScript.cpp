#include "MainScript.hpp"
#include "Features/Features.hpp"
#include "Hooking/Hooking.hpp"
#include "Util/Globals.hpp"
#include "R.A.G.Engine/HashKeys.hpp"
#include "R.A.G.Engine/Rage.hpp"
#include "R.A.G.Engine/grcTextureStore.hpp"
#include "R.A.G.Engine/grcTextureFactory.hpp"
#include "R.A.G.Engine/fwuiInputMessageBase.hpp"
#include "UI/Manager/DXUiManager.hpp"
#include "Util/Chat.hpp"
#include "Features/Lua/Lua.hpp"
#include "Features/YSC/YSCLoader.hpp"
#include "Util/Packet.hpp"
#include "Util/Tunnel.hpp"
#include "Discord/RPC.hpp"
#include "Fibers/DXFiberMgr.hpp"
#include "UI/Information/Box.hpp"
#include "ScriptHookV/ScriptHookVLoader.hpp"
#include <Mmsystem.h>
#define INPUT_MAX 1024

namespace base {
	using namespace uiTypes;
	void inputText(sub* sub, std::function<void(std::string)> buttonFn, int textLimit = INPUT_MAX, char const* initialValue = "") {
		std::thread thr([=] {
			std::this_thread::sleep_for(100ms);
			static auto txt = xorstr_("_") + sub->getName();
			g_rendererQueue.add(txt, [=] {
				if (sub->getName() == g_dxUiManager.m_submenuStack.top()->getName()) {
					if (ImGui::Begin(txt.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground)) {
						ImGui::SetWindowSize(g_dxUiManager.getPixelMathType({ g_dxUiManager.m_width + 0.01f, 0.2f }));
						ImGui::SetWindowPos(g_dxUiManager.getPixelMathType({ g_dxUiManager.m_pos.x - 0.042f, g_dxUiManager.m_drawBase + 0.002f }));
						ImGui::SetWindowFontScale(g_dxUiManager.m_scale);
						g_dxUiManager.m_forceLockInput = true;
						static char textBuf[INPUT_MAX]{};
						if (textBuf != initialValue)
							strcpy(textBuf, initialValue);
						ImGui::SetNextItemWidth(g_dxUiManager.m_width * g_dxUiManager.m_displaySize.x);
						ImGui::SetKeyboardFocusHere(0);
						ImGui::InputText((xorstr_("##") + txt).c_str(), textBuf, textLimit);
						if (g_dxUiManager.checkIfPressed(VK_RETURN)) {
							std::thread thr([=] {
								if (!std::string(textBuf).empty()) {
									buttonFn(textBuf);
									ZeroMemory(textBuf, sizeof(textBuf));
								}
								g_dxUiManager.m_forceLockInput = false;
								g_rendererQueue.remove(txt);
							});
							thr.detach();
						}
						else if (g_dxUiManager.checkIfPressed(VK_ESCAPE)) {
							g_dxUiManager.m_forceLockInput = false;
							g_rendererQueue.remove(txt);
						}
						ImGui::End();
					}
				}
			});
		});
		thr.detach();
	}
	void drawPlayerinfo(CPlayerInfo* plyr, int playerId, bool disablePlayerinfo = true) {
		if (!disablePlayerinfo) {
			ui::informational::box m_player{ { plyr->m_gamer_info.m_name }, g_dxUiManager.m_pos, g_dxUiManager.m_width + 0.005f };
			using singleLine = ui::informational::singleLine;
			using doubleLine = ui::informational::doubleLine;
			static const std::string m_format{ "{}: {}" };
			static const std::string m_addressFormat{ "{}: 0x{:X}" };
			static const std::string m_percentFormat{ "{}: {}/{} ({}%)" };
			static const std::string m_coordFormat{ "{}: {}, {}, {}" };
			static const std::string m_ipPortFormat{ "{}: {].{}.{}.{}:{}" };
			auto rlGamerInfo = &plyr->m_gamer_info;
			if (auto network = util::network::get(); network && util::network::isSessionActive()) {
				auto remotePlyrData = network->m_remote_player_data;
				if (auto playerInf = plyr; playerInf) {
					if (auto ped = playerInf->m_ped; ped) {
						auto pos = ped->get_position();
						auto pubAddr = rlGamerInfo->m_public_address;
						auto localAddr = rlGamerInfo->m_local_address;
						auto relayAddr = rlGamerInfo->m_relay_address;
						auto pubIp = pubAddr.m_ip;
						auto locIp = localAddr.m_ip;
						auto reIp = relayAddr.m_ip;
						std::string finalLabel{};
						if (ped->m_vehicle) {
							auto modelInfo = reinterpret_cast<rage::CVehicleModelInfo*>(ped->m_vehicle->m_model_info);
							auto manufacturerLabel = util::localization::getGxtLabel(modelInfo->m_name);
							auto nameLabel = util::localization::getGxtLabel(modelInfo->m_manufacturer);
							finalLabel = std::format("{}{}", (manufacturerLabel != xorstr_("NULL") || manufacturerLabel.empty() ? manufacturerLabel + xorstr_(" ") : xorstr_("")), nameLabel);
						}
						m_player.add(doubleLine({ m_format, "RID", rlGamerInfo->m_unk_gamer_handle.m_rockstar_id }, { m_format, "Slot", playerId }));
						m_player.add(doubleLine({ m_format, "Rank", remotePlyrData.m_data[playerId].m_rank }, { m_format, "Modder", "Unknown" }));
						m_player.add(doubleLine({ m_format, "God Mode", *(uint8_t*)(uint64_t(ped) + (offsetof(CPed, m_damage_bits) + 0x1)) ? xorstr_("Yes") : xorstr_("No") }, { m_percentFormat, "Health", ped->m_health, ped->m_maxhealth, ped->m_health / ped->m_maxhealth }));
						m_player.add(doubleLine({ m_percentFormat, "Armor", ped->m_armor, 255, ped->m_armor / 255 }, { "{}: {}/5", "Wanted Level", playerInf->m_wanted_level }));
						m_player.add(doubleLine({ m_format, "Vehicle", ped->m_vehicle ? finalLabel.c_str() : xorstr_("Not inside a vehicle") }, { m_coordFormat, "Position", std::round(pos.x), std::round(pos.y), std::round(pos.z) }));
						m_player.add(doubleLine({ m_addressFormat, "ID", rlGamerInfo->m_peer_id }, { m_addressFormat, "Address", rlGamerInfo->m_peer_address }));
						m_player.add(singleLine({ m_ipPortFormat, "Public", pubIp.m_field1, pubIp.m_field2, pubIp.m_field3, pubIp.m_field4, pubAddr.m_port }));
						m_player.add(singleLine({ m_ipPortFormat, "Local", locIp.m_field1, locIp.m_field2, locIp.m_field3, locIp.m_field4, localAddr.m_port }));
						m_player.add(singleLine({ m_ipPortFormat, "Relay", reIp.m_field1, reIp.m_field2, reIp.m_field3, reIp.m_field4, relayAddr.m_port }));
					}
				}
			}
			else {
				m_player.add(doubleLine({ m_format, "RID", "N/A" }, { m_format, "Slot", "N/A" }));
				m_player.add(doubleLine({ m_format, "Rank", "N/A" }, { m_format, "Modder", "Unknown" }));
				m_player.add(doubleLine({ m_format, "God Mode", "Unknown" }, { m_format, "Health", "UNK/UNK (0%)" }));
				m_player.add(doubleLine({ m_format, "Armor", "UNK/UNK (0%)" }, { m_format, "Wanted Level", "0/5" }));
				m_player.add(doubleLine({ m_format, "Vehicle", "Unknown" }, { m_format, "Position", "0.00, 0.00, 0.00" }));
				m_player.add(doubleLine({ m_format, "ID", "0x0000000000000000" }, { m_format, "Address", "0x0000000000000000" }));
				m_player.add(singleLine({ m_format, "Public", "0.0.0.0" }));
				m_player.add(singleLine({ m_format, "Local", "0.0.0.0" }));
				m_player.add(singleLine({ m_format, "Relay", "0.0.0.0" }));
			}
			m_player.call();
		}
	}
	int32_t numOfTooltips{ -1 };
	bool pullFromFile{};
	features::assetData textureSwapAsset{};
	auto g_requestDictionary = [](std::string dict) {
		if (dict.empty())
			return;
		if (!dict.c_str())
			return;
		g_fiberPool.queue([=]() {
			int32_t tries{};
			while (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(dict.c_str()) && tries != 35) {
				GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(dict.c_str(), FALSE);
				tries++;
			}
		});
	};
	void mainScript::initUi() {
		static auto rlGamerInfo{ util::ped::getLocalCPed()->m_player_info->m_gamer_info };
		g_dxUiManager.addSub(sub().setName(xorstr_("Home")).setId("home"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Self")).setId("home.self"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Weapons")).setId("home.weapons"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Vehicle")).setId("home.vehicle"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Network")).setId("home.network"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Miscellaneous")).setId("home.miscellaneous"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Protections")).setId("home.protections"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Script Loader")).setId("home.script_loader"_id));
			#ifdef DEBUG_MODE
				sub->addOption(subOpt().setLeft(xorstr_("Debug")).setId("home.debug"_id));
			#endif
			sub->addOption(subOpt().setLeft(xorstr_("Settings")).setId("home.settings"_id));
		}));
		#ifdef DEBUG_MODE
		g_dxUiManager.addSub(sub().setName(xorstr_("Exception Types"), false).setId("home.debug.tests.exception_types"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("mov sqword")).setAction([&] {
				*static_cast<uint64_t*>(nullptr) = 0;
				uint64_t i = *static_cast<uint64_t*>(nullptr);
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("mov 0xDEADBEEF")).setAction([&] {
				*((unsigned int*)0) = 0xDEADBEEF;
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("null func")).setAction([&] {
				void (*myfunc)() = NULL;
				myfunc();
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Features"), false).setId("home.debug.tests.features"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Get gsinfo")).setAction([&] {
				printf("gsinfo: %s\n", features::getGsInfo().c_str());
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("SHV Load Test")).setAction([&] {
				g_scriptHookV.load(fs::path("C:\\Users\\iiFaint\\Desktop\\bin\\NativeTrainer.asi"));
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("SHV Load Test (BP)")).setAction([&] {
				g_scriptHookV.load(fs::path("C:\\Users\\iiFaint\\Desktop\\bin\\Menyoo.asi"));
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("SHV Unload Test")).setAction([&] {
				g_scriptHookV.unload("NativeTrainer.asi");
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("SHV Clear Test")).setAction([&] {
				g_scriptHookV.clear();
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Tests"), false).setId("home.debug.tests"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Exception Types")).setId("home.debug.tests.exception_types"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Features")).setId("home.debug.tests.features"_id));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Debug"), false).setId("home.debug"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Tests")).setId("home.debug.tests"_id));
			sub->addOption(regularOpt().setLeft(xorstr_("Print base address")).setAction([&] {
				printf("base: 0x%llX\n", (uint64_t)GetModuleHandleA(NULL));
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Print SCAUTH header")).setAction([&] {
				printf("Authorization: SCAUTH val=\"%s\"\n", pointers::g_socialClubInfo->m_ticket);
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("sndPlaySoundA(SND_FILENAME | SND_ASYNC)")).setAction([=] {
				g_fiberPool.queue([=] {
					fs::path path{ std::getenv(xorstr_("appdata")) };
					path.append(xorstr_(BRANDING_NAME "\\Sounds\\NUT.wav"));
					g_dxUiManager.m_sounds.enter.enabled = false;
					sndPlaySoundA(path.string().c_str(), SND_FILENAME | SND_ASYNC);
					fiber::cur()->wait(100ms);
					g_dxUiManager.m_sounds.enter.enabled = true;
				});
			}));
		}));
		#endif
		g_dxUiManager.addSub(sub().setName(xorstr_("Movement")).setId("home.self.movement"_id).setAction([](sub* sub) {
			sub->addOption(numBoolOpt<float>().setLeft(xorstr_("No-Clip")).setDesc(xorstr_("Fly around like a stick figure")).setMinMax(0.f, 5.f).setStep(0.5f).setNum(features::noClipMultiplier).setBool(features::noClipBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Super Run")).setBool(features::superRunBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Super Jump")).setBool(features::superJumpBool));
			sub->addOption(numBoolOpt<float>().setLeft(xorstr_("Run speed")).setMinMax(1.f, 5.f).setStep(0.1f).setNum(features::fastRunSpeed).setBool(features::fastRunBool));
			sub->addOption(numBoolOpt<float>().setLeft(xorstr_("Swim speed")).setMinMax(1.f, 5.f).setStep(0.1f).setNum(features::fastSwimSpeed).setBool(features::fastSwimBool));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Health")).setId("home.self.health"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Max health")).setDesc(xorstr_("Sets your health to your max health")).setAction([] { util::ped::g_handle->m_health = util::ped::g_handle->m_maxhealth; }));
			sub->addOption(regularOpt().setLeft(xorstr_("Max armor")).setDesc(xorstr_("Sets your armor to 355")).setAction([] { util::ped::g_handle->m_armor = 50; }));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Self")).setId("home.self"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Movement")).setId("home.self.movement"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Health")).setId("home.self.health"_id));
			sub->addOption(boolOpt().setLeft(xorstr_("God Mode")).setDesc(xorstr_("Makes your player never die")).setBool(features::godModeBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Never Wanted")).setDesc(xorstr_("Spin the ops with no effort")).setBool(features::neverWantedBool));
			sub->addOption(boolOpt().setLeft(xorstr_("No Ragdoll")).setDesc(xorstr_("Become one with the tryhards")).setBool(features::noRagdollBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Invisibility")).setDesc(xorstr_("John Cena is now in GTA")).setBool(features::invisibilityBool));
			if (features::invisibilityBool)
				sub->addOption(boolOpt().setLeft(xorstr_("Locally Visble")).setDesc(xorstr_("How John Cena views himself")).setBool(features::locallyVisbleBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Off Radar")).setDesc(xorstr_("Become one with the tryhards")).setBool(features::offRadarBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Instant Respawn")).setDesc(xorstr_("Wasted screem begone")).setBool(features::instantRespawnBool));
			sub->addOption(numOpt<uint32_t>().setLeft(xorstr_("Wanted Level")).setDesc(xorstr_("Choose how black you are on a scale of 1 to 6")).setMinMax(0, 5).setStep(1).setNum(features::wantedLevelNumber));
			sub->addOption(regularOpt().setLeft(xorstr_("Suicide")).setDesc(xorstr_("Become the emo kid")).setAction([] { util::ped::g_handle->m_health = 0; }));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Tints")).setId("home.weapons.ammu_nation.tints"_id).setAction([](sub* sub) {

		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Ammu-Nation")).setId("home.weapons.ammu_nation"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Tints")).setId("home.weapons.ammu_nation.tints"_id));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Weapons")).setId("home.weapons"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Ammu-Nation")).setId("home.weapons.ammu_nation"_id));
			sub->addOption(boolOpt().setLeft(xorstr_("Random Rapid Fire")).setBool(features::customrapidFireBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Rapid Fire")).setBool(features::rapidFireBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Infinite Ammo")).setBool(features::infiniteAmmoBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Explosive Ammo")).setBool(features::explosiveAmmoBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Delete Gun")).setBool(features::deleteGunBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Teleport Gun")).setBool(features::teleportGunBool));
			sub->addOption(boolOpt().setLeft(xorstr_("One Hit Gun")).setBool(features::oneHitGunBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Airstrike Gun")).setBool(features::airstrikeGunBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Drive Tt Gun")).setBool(features::driveitGunBool));
			sub->addOption(numBoolOpt<int>().setLeft(xorstr_("Explosion Gun")).setMinMax(1, 97).setStep(1).setNum(features::explosionType).setBool(features::customExplosionGunBool));

			sub->addOption(regularOpt().setLeft(xorstr_("Give Weapons")).setDesc(xorstr_("Become the emo kid (American school edition)")).setAction([] {
				for (auto name : hashes::weaponHashes)
					WEAPON::GIVE_WEAPON_TO_PED(util::ped::g_entityHandle, rage::atStringHash(name), 9999, FALSE, TRUE);
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Remove Weapons")).setAction([] {
				WEAPON::REMOVE_ALL_PED_WEAPONS(util::ped::g_entityHandle, true);
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Refill Ammo")).setAction([] {
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
				}));

		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("InvalidVehicle")).setId("home.vehicle.spawner.selected.selected"_id).setAction([](sub* sub) {
			sub->addOption(boolOpt().setLeft(xorstr_("Plate Text")).setDesc(xorstr_("Sets the plate text to your custom text")).setBool(features::withPlateText));
			if (features::withPlateText) {
				sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(features::plateText).setAction([=] {
					inputText(sub, [](std::string str) {
						features::plateText = str.c_str();
					});
				}));
			}
			sub->addOption(boolOpt().setLeft(xorstr_("Networked")).setDesc(xorstr_("This is recommended if you wish to use it in Online")).setBool(features::withNetParamaters));
			sub->addOption(boolOpt().setLeft(xorstr_("Owned")).setDesc(xorstr_("Allows you to store it in a garage (garage must be full)")).setBool(features::asOwnedVehicle));
			sub->addOption(boolOpt().setLeft(xorstr_("In Vehicle")).setDesc(xorstr_("Your ped is placed inside the vehicle when you spawn it")).setBool(features::spawnInVehicle));
			sub->addOption(regularOpt().setLeft(xorstr_("Spawn")).setAction([=] {
				models::dataStoring::getTable(g_selectedVehicleClass, [=](int index, std::string modelClass, features::vehicleData data) {
					if (g_selectedVehicleFromClass == index) {
						features::spawnVehicle(data);
					}
				});
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("InvalidClass")).setId("home.vehicle.spawner.selected"_id).setAction([](sub* sub) {
			models::dataStoring::getTable(g_selectedVehicleClass, [=](int index, std::string modelClass, features::vehicleData data) {
				sub->addOption(subOpt().setLeft(data.finalLabel).setId("home.vehicle.spawner.selected.selected"_id).setAction([&] {
					g_selectedVehicleFromClass = index;
					g_dxUiManager.getSubData("home.vehicle.spawner.selected.selected"_id)->setName(data.finalLabel).setSelectedOpt(0);
				}), false);
			});
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Spawner")).setId("home.vehicle.spawner"_id).setAction([](sub* sub) {
			for (int i{}; i != models::dataStoring::g_modelClasses.size(); ++i) {
				sub->addOption(subOpt().setLeft(models::dataStoring::g_modelClasses[i]).setId("home.vehicle.spawner.selected"_id).setAction([=] {
					g_selectedVehicleClass = i;
					g_dxUiManager.getSubData("home.vehicle.spawner.selected"_id)->setName(models::dataStoring::g_modelClasses[i]);
				}), false);
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Vehicle")).setId("home.vehicle"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Spawner")).setId("home.vehicle.spawner"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Acrobatic's")).setId("home.vehicle.Acrobatics"_id));
			sub->addOption(numBoolOpt<float>().setLeft(xorstr_("Horn Boost")).setBool(features::hornBoostBool).setMinMax(0.f, 70.f).setStep(0.5f).setNum(features::hornBoostSpeed));
			sub->addOption(numBoolOpt<float>().setLeft(xorstr_("Speed Limit")).setBool(features::speedLimitBool).setMinMax(0.f, 300.f).setStep(1.f).setNum(features::speedLimitSpeed));
			sub->addOption(boolOpt().setLeft(xorstr_("God Mode")).setBool(features::vehGodModeBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Invisibility")).setBool(features::vehInvisibilityBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Fix Loop")).setBool(features::fixLoopBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Instant Brake")).setBool(features::instantBrakeBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Drift")).setBool(features::driftBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Rainbow")).setBool(features::rainbowVehicleBool));
			sub->addOption(regularOpt().setLeft(xorstr_("Max")).setAction([=] {
				Vehicle vehicle = util::vehicle::g_entityHandle;
				VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, 18, 1);
				VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, 22, 1);
				VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, 20, 1);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 16, 5, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 12, 2, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 11, 3, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 14, 14, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 15, 3, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 13, 2, 0);
				VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 6);
				VEHICLE::SET_VEHICLE_WINDOW_TINT(vehicle, 5);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 23, 19, 1);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 0, 1, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 1, 1, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 2, 1, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 3, 1, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 4, 1, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 5, 1, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 6, 1, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 7, 1, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 8, 1, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 9, 1, 0);
				VEHICLE::SET_VEHICLE_MOD(vehicle, 10, 1, 0);
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Fix & Wash")).setAction([=] {
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						VEHICLE::SET_VEHICLE_FIXED(vehicle);
						VEHICLE::SET_VEHICLE_DIRT_LEVEL(vehicle, 0.0f);
						ENTITY::SET_ENTITY_RENDER_SCORCHED(vehicle, FALSE);
						GRAPHICS::WASH_DECALS_FROM_VEHICLE(vehicle, 100.0f);
					}
				}
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Flip Vehicle")).setAction([=] {
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle, false);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Open All Vehicle Door's")).setAction([=] {
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						VEHICLE::SET_VEHICLE_DOOR_OPEN(vehicle, 0, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(vehicle, 1, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(vehicle, 2, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(vehicle, 3, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(vehicle, 4, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(vehicle, 5, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(vehicle, 6, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(vehicle, 7, true, false);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Close All Vehicle Door's")).setAction([=] {
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						VEHICLE::SET_VEHICLE_DOORS_SHUT(vehicle, true);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Set A Random Tire Smoke Color")).setAction([=] {
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						VEHICLE::TOGGLE_VEHICLE_MOD(PED::GET_VEHICLE_PED_IS_IN(vehicle, 0), 20, 1);
						VEHICLE::SET_VEHICLE_MOD_KIT(PED::GET_VEHICLE_PED_IS_IN(vehicle, 0), 0);
						VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255));

					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Set Vehicle License Plate To Foxy")).setAction([=] {
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicle, "Foxy");
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Heel Flip")).setAction([=] {
				rage::vector3 Force{ 0, 0, 5.0f, };
				rage::vector3 OffForce{ -2.0f, 0, 0 };
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, { 0, 0, 5.0f, }, { -2.0f, 0, 0 }, true, true, true, true, false, true);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Kick Flip")).setAction([=] {
				rage::vector3 Force{ 0,0,10.0f };
				rage::vector3 OffForce{ 20.0f, 0.0f, 0.0f };
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, { 0,0,10.0f }, { 20.0f, 0.0f, 0.0f }, true, true, true, true, false, true);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Back Flip")).setAction([=] {
				rage::vector3 Force{ 0, 0, 6.0f };
				rage::vector3 OffForce{ 0, 2.0f, 0 };
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, { 0, 0, 6.0f }, { 0, 2.0f, 0 }, true, true, true, true, false, true);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Double Back Flip")).setAction([=] {
				rage::vector3 Force{ 0, 0, 12.0f };
				rage::vector3 OffForce{ 0, 4.0f, 0 };
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, { 0, 0, 12.0f }, { 0, 4.0f, 0 }, true, true, true, true, false, true);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("360 Launch")).setAction([=] {
				rage::vector3 Force{ 0, 20.f, 0.0f };
				rage::vector3 OffForce{ 20.f, 0.f, 0.f };
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, { 0, 20.f, 0.0f }, { 20.f, 0.f, 0.f }, true, true, true, true, false, true);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Spinning Jump")).setAction([=] {
				rage::vector3 Force{ 0, 20.f, 20.0f };
				rage::vector3 OffForce{ 0, 0, 20.0f };
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, { 0, 20.f, 20.0f }, { 0, 0, 20.0f }, true, true, true, true, false, true);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Bunny Hop")).setAction([=] {
				rage::vector3 Force{ 0, 0, 7.0f };
				rage::vector3 OffForce{ 0, 0, 0.0f };
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, { 0, 0, 7.0f }, { 0, 0, 0.0f }, true, true, true, true, false, true);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Super Bunny Hop")).setAction([=] {
				rage::vector3 Force{ 0, 0, 40.0f };
				rage::vector3 OffForce{ 0, 0, 0.0f };
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, { 0, 0, 40.0f }, { 0, 0, 0.0f }, true, true, true, true, false, true);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Space-X")).setAction([=] {
				rage::vector3 Force{ 0, 0, 1000.0f };
				rage::vector3 OffForce{ 0, 0, 0.0f };
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, { 0, 0, 1000.0f }, { 0, 0, 0 }, true, true, true, true, false, true);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Boost Forward")).setAction([=] {
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, 500);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Boost Backward")).setAction([=] {
				rage::vector3 Force{ 0, 0, 1000.0f };
				rage::vector3 OffForce{ 0, 0, 0.0f };
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, -500);
					}
				}
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Stop  Vehicle")).setAction([=] {
				rage::vector3 Force{ 0, 0, 1000.0f };
				rage::vector3 OffForce{ 0, 0, 0.0f };
				Vehicle vehicle = util::vehicle::g_entityHandle;
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), FALSE)) {
					if (ENTITY::DOES_ENTITY_EXIST(vehicle)) {
						VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, 0);
					}
				}
				}));


			/*static ImU32 primaryColor{};
			sub->addOption(colorOpt().setLeft(xorstr_("Primary")).setColor(&primaryColor).setAction([=] {
				auto float4Col = ImGui::ColorConvertU32ToFloat4(primaryColor);
				RGBA col = RGBA(uint8_t(float4Col.x * 255.f), uint8_t(float4Col.y * 255.f), uint8_t(float4Col.z * 255.f), uint8_t(float4Col.w * 255.f));
				features::setPrimaryColor(col);
			}));
			static ImU32 secondaryColor{};
			sub->addOption(colorOpt().setLeft(xorstr_("Secondary")).setColor(&secondaryColor).setAction([=] {
				auto float4Col = ImGui::ColorConvertU32ToFloat4(secondaryColor);
				RGBA col = RGBA(uint8_t(float4Col.x * 255.f), uint8_t(float4Col.y * 255.f), uint8_t(float4Col.z * 255.f), uint8_t(float4Col.w * 255.f));
				features::setPrimaryColor(col);
			}));*/
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Value Range")).setId("home.network.sessions.filters.player_count.range"_id).setAction([](sub* sub) {
			sub->addOption(numBoolOpt<uint8_t>().setLeft(xorstr_("Minimum Value")).setMinMax(1, 32).setStep(1).setNum(features::matchmaking::filters::playerCount::min::value).setBool(features::matchmaking::filters::playerCount::min::toggle));
			sub->addOption(numBoolOpt<uint8_t>().setLeft(xorstr_("Maximum Value")).setMinMax(1, 32).setStep(1).setNum(features::matchmaking::filters::playerCount::max::value).setBool(features::matchmaking::filters::playerCount::max::toggle));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Specific Value")).setId("home.network.sessions.filters.player_count.specific"_id).setAction([](sub* sub) {
			sub->addOption(numOpt<uint8_t>().setLeft(xorstr_("Value:")).setNum(features::matchmaking::filters::playerCount::value).setStep(1).setMinMax(1, 32));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::matchmaking::filters::playerCount::toggle));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Player Count")).setId("home.network.sessions.filters.player_count"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Value Range")).setId("home.network.sessions.filters.player_count.range"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Specific Value")).setId("home.network.sessions.filters.player_count.specific"_id));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Filters")).setId("home.network.sessions.filters"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Player Count")).setId("home.network.sessions.filters.player_count"_id));
			sub->addOption(vecBoolOpt<char const*, int>().setLeft(xorstr_("Region")).setArray(hashes::regionTypes).setPos(hashes::regionTypesPos).setBool(features::matchmaking::filters::region::toggle).setAction([] {
				features::matchmaking::filters::region::value = features::region(hashes::regionTypesPos, true);
			}));
			sub->addOption(vecBoolOpt<char const*, int>().setLeft(xorstr_("Language")).setArray(hashes::languageTypes).setPos(hashes::languageTypesPos).setBool(features::matchmaking::filters::language::toggle).setAction([] {
				features::matchmaking::filters::language::value = features::language(hashes::languageTypesPos, true);
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Sessions")).setId("home.network.sessions"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Filters")).setId("home.network.sessions.filters"_id));
			sub->addOption(regularOpt().setLeft(xorstr_("Find sessions")).setAction([=] {
				features::matchmaking::triggerSessionFind();
			}));
			if (features::matchmaking::numberOfFoundSessions) {
				for (int32_t i{}; i != features::matchmaking::numberOfFoundSessions; ++i) {
					auto& session = features::matchmaking::sessions[i];
					auto& sessionInfo = session.info;
					auto& hostData = sessionInfo.m_peer_info;
					if (session.valid) {
						sub->addOption(subOpt().setLeft(std::format("{} | Player count: {}", sessionInfo.m_session_token, session.attributes.playerCount)).setId("home.network.sessions.selected"_id).setAction([=] {
							g_selectedSession = i;
							g_dxUiManager.getSubData("home.network.sessions.selected"_id)->setName(std::format("{} | Player count: {}", sessionInfo.m_session_token, session.attributes.playerCount)).setSelectedOpt(0);
						}), false);
					}
				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("InvalidSession")).setId("home.network.sessions.selected"_id).setAction([](sub* sub) {
			auto& session = features::matchmaking::sessions[g_selectedSession];
			auto& info = session.info;
			auto& attributes = session.attributes;
			auto& hostData = info.m_peer_info;
			auto hostName = features::scapi::ridToName(hostData.m_gamer_handle.m_rockstar_id);
			sub->addOption(vecOpt<char const*, int>().setLeft(xorstr_("Join")).setArray(hashes::ridJoinTypes).setPos(hashes::ridJoinTypesPos).setAction([&] {
				bool isSessionFreeAim = attributes.discriminator & (1 << 17);
				bool isLocalGameUsingFreeAim = PAD::GET_LOCAL_PLAYER_GAMEPAD_AIM_STATE() > 1;
				if (isSessionFreeAim != isLocalGameUsingFreeAim)
					PLAYER::SET_PLAYER_TARGETING_MODE(isSessionFreeAim ? 3 : 1);
				features::ridJoin(hostData.m_gamer_handle.m_rockstar_id, hashes::ridJoinTypesPos);
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Copy Hosts RID")).setAction([&] {
				auto rid = std::to_string(hostData.m_gamer_handle.m_rockstar_id);
				ImGui::SetClipboardText(rid.c_str());
				g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Set clipboard to ") + rid + xorstr_("."));
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Copy Hosts Name")).setAction([&] {
				ImGui::SetClipboardText(hostName.c_str());
				g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Set clipboard to ") + hostName + xorstr_("."));
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Players")).setId("home.network.players"_id).setAction([](sub* sub) {
			if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr && cNetworkPlayerMgr->m_player_count != 0) {
				for (auto& cNetGamePlayer : cNetworkPlayerMgr->m_player_list) {
					if (cNetGamePlayer && cNetGamePlayer->IsConnected()) {
						if (sub->getSelectedOpt() == cNetGamePlayer->m_player_id)
							drawPlayerinfo(cNetGamePlayer->m_player_info, cNetGamePlayer->m_player_id);
						sub->addOption(subOpt().setLeft(cNetGamePlayer->GetName()).setId("home.network.players.selected"_id).setAction([=] {
							g_selectedPlayer = cNetGamePlayer->m_player_id;
							g_dxUiManager.getSubData("home.network.players.selected"_id)->setName(cNetGamePlayer->GetName()).setSelectedOpt(0);
						}), false);
					}
				}
			}
			else {
				drawPlayerinfo(util::player::g_handle, 0);
				g_selectedPlayer = 0;
				sub->addOption(subOpt().setLeft(rlGamerInfo.m_name).setId("home.network.players.selected"_id).setAction([] {
					g_dxUiManager.getSubData("home.network.players.selected"_id)->setName(rlGamerInfo.m_name).setSelectedOpt(0);
				}), false);
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("RID Tools")).setId("home.network.players.selected.rid_tools"_id).setAction([](sub* sub) {
			if (auto netPlyrMgr = *pointers::g_networkPlayerMgr; netPlyrMgr && netPlyrMgr->m_player_count != 0) {
				if (auto netPlayer = netPlyrMgr->m_player_list[g_selectedPlayer]; netPlayer && netPlayer->IsConnected()) {
					drawPlayerinfo(netPlayer->m_player_info, netPlayer->m_player_id);
					sub->addOption(regularOpt().setLeft(xorstr_("Set Spoofing Profile To Their RID")).setAction([=] {
						features::spoofing::rid.first = netPlayer->GetGamerInfo()->m_unk_gamer_handle.m_rockstar_id;
					}));
					sub->addOption(regularOpt().setLeft(xorstr_("Copy RID")).setAction([=] {
						auto rid = std::to_string(netPlayer->GetGamerInfo()->m_unk_gamer_handle.m_rockstar_id);
						ImGui::SetClipboardText(rid.c_str());
						g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Set clipboard to ") + rid + xorstr_("."));
					}));
					sub->addOption(regularOpt().setLeft(xorstr_("Copy Name")).setAction([=] {
						ImGui::SetClipboardText(netPlayer->GetName());
						g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Set clipboard to ") + std::string(netPlayer->GetName()) + xorstr_("."));
					}));
					sub->addOption(regularOpt().setLeft(xorstr_("Open SCUI")).setAction([] {
						int32_t size = 13;
						auto buffer = std::make_unique<Any>(size);
						NETWORK::NETWORK_HANDLE_FROM_PLAYER(g_selectedPlayer, buffer.get(), size);
						NETWORK::NETWORK_SHOW_PROFILE_UI(buffer.get());
					}));
				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("InvalidPlayer")).setId("home.network.players.selected"_id).setAction([](sub* sub) {
			if (auto netPlyrMgr = *pointers::g_networkPlayerMgr; netPlyrMgr && netPlyrMgr->m_player_count != 0) {
				if (auto netPlayer = netPlyrMgr->m_player_list[g_selectedPlayer]; netPlayer && netPlayer->IsConnected()) {
					drawPlayerinfo(netPlayer->m_player_info, netPlayer->m_player_id);
					sub->addOption(subOpt().setLeft(xorstr_("Teleports")).setId("home.network.players.selected.teleports"_id));
					if (rage::atStringHash(netPlayer->GetName()) == rage::atStringHash(util::player::g_networkHandle->GetName()))
						sub->addOption(regularOpt().setLeft(xorstr_("Greifing")).setAction([] {
							g_dxUiManager.sendNotify(xorstr_("Players"), xorstr_("You cannot greif yourself"));
						}));
					else
						sub->addOption(subOpt().setLeft(xorstr_("Greifing")).setId("home.network.players.selected.greifing"_id));
					sub->addOption(subOpt().setLeft(xorstr_("Friendly")).setId("home.network.players.selected.friendly"_id));
					sub->addOption(subOpt().setLeft(xorstr_("RID Tools")).setId("home.network.players.selected.rid_tools"_id));
					sub->addOption(boolOpt().setLeft(xorstr_("Spectate")).setBool(features::spectatePlayerBool[g_selectedPlayer]).setAction([&] {
						for (int8_t i{ 31 }; i; --i) {
							if (g_selectedPlayer != i)
								if (features::spectatePlayerBool[g_selectedPlayer])
									features::spectatePlayerBool[i] = false;
						}
						if (features::spectatePlayerBool[g_selectedPlayer])
							STREAMING::SET_FOCUS_ENTITY(PLAYER::GET_PLAYER_PED(g_selectedPlayer));
						else
							STREAMING::CLEAR_FOCUS();
					}));
					sub->addOption(boolOpt().setLeft(xorstr_("Sync chat")).setBool(features::msgSyncWithPerson[g_selectedPlayer]));
					sub->addOption(regularOpt().setLeft(xorstr_("Add to blacklisted players")).setAction([&] {
						if (auto plyr = netPlyrMgr->m_player_list[g_selectedPlayer]; plyr && plyr->IsConnected()) {
							protections::g_arrayOfBlacklistedPlayers.push_back({ plyr, xorstr_("Manual Add") });
						}
					}));
				}
				else {
					g_dxUiManager.sendNotify(xorstr_("Players"), xorstr_("{} left the session"), g_dxUiManager.getSubData("home.network.players.selected"_id)->getName());
					g_dxUiManager.switchToSub("home.network.players"_id);
					return;
				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Teleports")).setId("home.network.players.selected.teleports"_id).setAction([](sub* sub) {
			if (auto netPlyrMgr = *pointers::g_networkPlayerMgr; netPlyrMgr && netPlyrMgr->m_player_count != 0) {
				if (auto netPlayer = netPlyrMgr->m_player_list[g_selectedPlayer]; netPlayer && netPlayer->IsConnected()) {
					drawPlayerinfo(netPlayer->m_player_info, netPlayer->m_player_id);
					sub->addOption(regularOpt().setLeft(xorstr_("Teleport to player")).setAction([] {
						auto playerPed = PLAYER::GET_PLAYER_PED(g_selectedPlayer);
						auto coords = ENTITY::GET_ENTITY_COORDS(playerPed, false);
						auto handle = PED::IS_PED_IN_ANY_VEHICLE(util::ped::g_entityHandle, false) ? PED::GET_VEHICLE_PED_IS_USING(util::ped::g_entityHandle) : util::ped::g_entityHandle;
						ENTITY::SET_ENTITY_COORDS(handle, coords, true, true, true, false);
					}));
					sub->addOption(regularOpt().setLeft(xorstr_("Teleport into vehicle")).setAction([] {
						auto playerPed = PLAYER::GET_PLAYER_PED(g_selectedPlayer);
						auto coords = ENTITY::GET_ENTITY_COORDS(playerPed, false);
						auto veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
						for (int i{ 16 }; i; --i) {
							if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, i, false)) {
								ENTITY::SET_ENTITY_COORDS(util::ped::g_entityHandle, coords, false, false, false, false);
								PED::SET_PED_INTO_VEHICLE(util::ped::g_entityHandle, veh, i);
							}
						}
					}));
				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Crashes")).setId("home.network.players.selected.greifing.crashes"_id).setAction([](sub* sub) {
			if (auto netPlyrMgr = *pointers::g_networkPlayerMgr; netPlyrMgr && netPlyrMgr->m_player_count != 0) {
				if (auto netPlayer = netPlyrMgr->m_player_list[g_selectedPlayer]; netPlayer && netPlayer->IsConnected()) {
					drawPlayerinfo(netPlayer->m_player_info, netPlayer->m_player_id);

				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Kicks")).setId("home.network.players.selected.greifing.kicks"_id).setAction([](sub* sub) {
			if (auto netPlyrMgr = *pointers::g_networkPlayerMgr; netPlyrMgr && netPlyrMgr->m_player_count != 0) {
				if (auto netPlayer = netPlyrMgr->m_player_list[g_selectedPlayer]; netPlayer && netPlayer->IsConnected()) {
					drawPlayerinfo(netPlayer->m_player_info, netPlayer->m_player_id);
					sub->addOption(regularOpt().setLeft(xorstr_("Desync Kick")).setAction([&] {
						netPlyrMgr->RemovePlayer(netPlayer);
						WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED(g_selectedPlayer), true);
						protections::g_arrayOfBlacklistedPlayers.push_back({ netPlayer, "Kicked Player" });
					}));
					sub->addOption(regularOpt().setLeft(xorstr_("PTSD Kick")).setAction([&] {
						auto rlGamerInfo = netPlayer->GetGamerInfo();
						auto complaintMgr = util::network::get()->m_game_complaint_mgr;
						complaintMgr.raise_complaint(rlGamerInfo->m_peer_address, true, true);
						protections::g_arrayOfBlacklistedPlayers.push_back({ netPlayer, "Kicked Player" });
					}));
					sub->addOption(regularOpt().setLeft(xorstr_("Slow Inc. Kick")).setAction([&] {
						packet msg{ rage::eNetMessage::CMsgLostConnectionToHost };
						msg.Write<uint64_t>(util::network::session::get().m_rline_session.m_session_id, 0x40);
						if (auto gamerInfo = netPlayer->GetGamerInfo(); gamerInfo)
							gamerInfo->m_unk_gamer_handle.serialize(msg);
						protections::g_arrayOfBlacklistedPlayers.push_back({ netPlayer, "Kicked Player" });
						for (auto& plyr : netPlyrMgr->m_player_list) {
							if (plyr && plyr->IsConnected()) {
								if (plyr->IsNetworkHost())
									msg.send(uint32_t(util::network::session::player::getViaConnectionId(plyr->m_player_id)->m_msg_id));
							}
						}
					}));
				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Greifing")).setId("home.network.players.selected.greifing"_id).setAction([](sub* sub) {
			if (auto netPlyrMgr = *pointers::g_networkPlayerMgr; netPlyrMgr && netPlyrMgr->m_player_count != 0) {
				if (auto netPlayer = netPlyrMgr->m_player_list[g_selectedPlayer]; netPlayer && netPlayer->IsConnected()) {
					drawPlayerinfo(netPlayer->m_player_info, netPlayer->m_player_id);
					sub->addOption(subOpt().setLeft(xorstr_("Kicks")).setId("home.network.players.selected.greifing.kicks"_id));
					sub->addOption(subOpt().setLeft(xorstr_("Crashes")).setId("home.network.players.selected.greifing.crashes"_id));
					sub->addOption(regularOpt().setLeft(xorstr_("Blame")).setAction([&] {
						pointers::g_blameExplode->apply();
						FIRE::ADD_OWNED_EXPLOSION(PLAYER::GET_PLAYER_PED(g_selectedPlayer), ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(g_selectedPlayer), true), 1, 1000, true, false, 0.1f);
						pointers::g_blameExplode->restore();
					}));
					sub->addOption(regularOpt().setLeft(xorstr_("Explode")).setAction([&] {
						FIRE::ADD_EXPLOSION(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(g_selectedPlayer), true), 1, 1000.f, true, false, 0.1f, false);
					}));
					sub->addOption(regularOpt().setLeft(xorstr_("Clone")).setAction([&] {
						PED::CLONE_PED(PLAYER::GET_PLAYER_PED(g_selectedPlayer), true, true, true);
					}));
					sub->addOption(regularOpt().setLeft(xorstr_("Remove Weapons")).setAction([&] {
						WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED(g_selectedPlayer), true);
					}));
				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Commands")).setId("home.network.players.selected.friendly.commands"_id).setAction([](sub* sub) {
			if (auto netPlyrMgr = *pointers::g_networkPlayerMgr; netPlyrMgr && netPlyrMgr->m_player_count != 0) {
				if (auto netPlayer = netPlyrMgr->m_player_list[g_selectedPlayer]; netPlayer && netPlayer->IsConnected()) {
					drawPlayerinfo(netPlayer->m_player_info, netPlayer->m_player_id);
					sub->addOption(regularOpt().setLeft(xorstr_("Toggle All")).setAction([] {
						for (auto& cmd : g_cmdArr[g_selectedPlayer].cmdMap)
							cmd.toggle ^= true;
					}));
					for (auto& cmd : g_cmdArr[g_selectedPlayer].cmdMap)
						sub->addOption(boolOpt().setLeft(cmd.cmdName).setBool(cmd.toggle));
				}
			}
		}));
		static std::string hashStr{};
		g_dxUiManager.addSub(sub().setName(xorstr_("Hash Presets")).setId("ome.network.players.selected.friendly.drops.hash_presets"_id).setAction([](sub* sub) {
			if (auto netPlyrMgr = *pointers::g_networkPlayerMgr; netPlyrMgr && netPlyrMgr->m_player_count != 0) {
				if (auto netPlayer = netPlyrMgr->m_player_list[g_selectedPlayer]; netPlayer && netPlayer->IsConnected()) {
					drawPlayerinfo(netPlayer->m_player_info, netPlayer->m_player_id);
					for (auto& v : hashes::hashPresets) {
						sub->addOption(iconOpt().setLeft(v).setIcon(hashStr == v ? 'g' : '\0').setAction([=] {
							hashStr = v;
							if (containsANumber(hashStr))
								features::dropHash = std::stoull(hashStr, 0, 16);
						}));
					}
				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Drops")).setId("home.network.players.selected.friendly.drops"_id).setAction([](sub* sub) {
			if (auto netPlyrMgr = *pointers::g_networkPlayerMgr; netPlyrMgr && netPlyrMgr->m_player_count != 0) {
				if (auto netPlayer = netPlyrMgr->m_player_list[g_selectedPlayer]; netPlayer && netPlayer->IsConnected()) {
					drawPlayerinfo(netPlayer->m_player_info, netPlayer->m_player_id);
					sub->addOption(subOpt().setLeft(xorstr_("Hash Presets")).setId("home.network.players.selected.friendly.drops.hash_presets"_id));
					sub->addOption(regularOpt().setLeft(xorstr_("Hash:")).setRight(hashStr).setAction([=] {
						inputText(sub, [](std::string str) {
							hashStr = str;
						});
					}));
					sub->addOption(numOpt<double>().setLeft(xorstr_("Delay")).setDesc(xorstr_("2.5s is recommended to get rid of transaction errors")).setMinMax(0, 10).setStep(0.1).setNum(features::dropDelay));
					sub->addOption(vecOpt<char const*, int>().setLeft(xorstr_("Type")).setArray(hashes::dropTypes).setPos(features::dropType));
					sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::dropBool[g_selectedPlayer]).setAction([=] {
						for (int8_t i{ 31 }; i; --i) {
							if (g_selectedPlayer != i)
								if (features::dropBool[g_selectedPlayer])
									features::dropBool[i] = false;
						}
					}));
				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Friendly")).setId("home.network.players.selected.friendly"_id).setAction([](sub* sub) {
			if (auto netPlyrMgr = *pointers::g_networkPlayerMgr; netPlyrMgr && netPlyrMgr->m_player_count != 0) {
				if (auto netPlayer = netPlyrMgr->m_player_list[g_selectedPlayer]; netPlayer && netPlayer->IsConnected()) {
					drawPlayerinfo(netPlayer->m_player_info, netPlayer->m_player_id);
					sub->addOption(subOpt().setLeft(xorstr_("Commands")).setId("home.network.players.selected.friendly.commands"_id));
					sub->addOption(subOpt().setLeft(xorstr_("Drop")).setId("home.network.players.selected.friendly.drops"_id));
					sub->addOption(vecOpt<char const*, int>().setLeft(xorstr_("Give Weapon")).setArray(hashes::weaponHashes).setPos(hashes::weaponHashesPos).setAction([&] {
						WEAPON::GIVE_WEAPON_TO_PED(util::entity::getEntityFromHandle(netPlayer->m_player_info->m_ped), rage::atStringHash(hashes::weaponHashes[hashes::weaponHashesPos]), 9999, false, true);
					}));
					sub->addOption(regularOpt().setLeft(xorstr_("Give Weapons")).setAction([&] {
						for (auto name : hashes::weaponHashes)
							WEAPON::GIVE_WEAPON_TO_PED(util::entity::getEntityFromHandle(netPlayer->m_player_info->m_ped), rage::atStringHash(name), 9999, false, true);
					}));
				}
			}
		}));
		static std::string allHashStr{};
		g_dxUiManager.addSub(sub().setName(xorstr_("Hash Presets")).setId("home.network.all_players.drops.friendly.hash_presets"_id).setAction([](sub* sub) {
			if (auto netPlyrMgr = *pointers::g_networkPlayerMgr; netPlyrMgr && netPlyrMgr->m_player_count != 0) {
				if (auto netPlayer = netPlyrMgr->m_player_list[g_selectedPlayer]; netPlayer && netPlayer->IsConnected()) {
					drawPlayerinfo(netPlayer->m_player_info, netPlayer->m_player_id);
					for (auto& v : hashes::hashPresets) {
						sub->addOption(iconOpt().setLeft(v).setIcon(allHashStr == v ? 'g' : '\0').setAction([=] {
							allHashStr = v;
							if (containsANumber(allHashStr))
								features::dropAllHash = std::stoul(allHashStr, 0, 16);
						}));
					}
				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Drops")).setId("home.network.all_players.friendly.drops"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Hash Presets")).setId("home.network.all_players.friendly.drops.hash_presets"_id));
			sub->addOption(regularOpt().setLeft(xorstr_("Hash:")).setRight(allHashStr).setAction([=] {
				inputText(sub, [](std::string str) {
					allHashStr = str;
					if (containsANumber(allHashStr))
						features::dropAllHash = std::stoul(allHashStr, 0, 16);
				});
			}));
			sub->addOption(numOpt<double>().setLeft(xorstr_("Delay")).setDesc(xorstr_("2.5s is recommended to get rid of transaction errors")).setNum(features::dropAllDelay).setStep(0.1).setMinMax(0, 10));
			sub->addOption(vecOpt<char const*, int>().setLeft(xorstr_("Type")).setArray(hashes::dropTypes).setPos(features::dropAllType));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::dropAllBool));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Commands")).setId("home.network.all_players.friendly.commands"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Toggle All")).setAction([] {
				if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
					for (uint16_t i{}; i != cNetworkPlayerMgr->m_player_count; ++i) {
						for (auto& cmd : g_cmdArr[i].cmdMap)
							cmd.toggle ^= true;
					}
				}
			}));
			for (auto& cmd : g_cmdArr[0].cmdMap) {
				sub->addOption(boolOpt().setLeft(cmd.cmdName).setBool(cmd.toggle).setAction([=] {
					if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
						for (uint16_t i{}; i != cNetworkPlayerMgr->m_player_count; ++i) {
							g_cmdArr[i].cmdMap[cmd.index].toggle = cmd.toggle;
						}
					}
				}));
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Friendly")).setId("home.network.all_players.friendly"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Drops")).setId("home.network.all_players.friendly.drops"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Commands")).setId("home.network.all_players.friendly.commands"_id));
			sub->addOption(vecOpt<char const*, int>().setLeft(xorstr_("Give Weapon")).setArray(hashes::weaponHashes).setPos(hashes::weaponHashesPos).setAction([] {
				if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
					for (uint16_t i{}; i != cNetworkPlayerMgr->m_player_count; ++i) {
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED(i), rage::atStringHash(hashes::weaponHashes[hashes::weaponHashesPos]), 9999, false, true);
					}
				}
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Give Weapons")).setAction([] {
				if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
					for (uint16_t i{}; i != cNetworkPlayerMgr->m_player_count; ++i) {
						for (auto name : hashes::weaponHashes)
							WEAPON::GIVE_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED(i), rage::atStringHash(name), 9999, false, true);
					}
				}
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Kicks")).setId("home.network.all_players.greifing.kicks"_id).setAction([](sub* sub) {

		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Crashes")).setId("home.network.all_players.greifing.crashes"_id).setAction([](sub* sub) {

		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Greifing")).setId("home.network.all_players.greifing"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Kicks")).setId("home.network.all_players.greifing.kicks"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Crashes")).setId("home.network.all_players.greifing.crashes"_id));
			sub->addOption(regularOpt().setLeft(xorstr_("Explode")).setAction([] {
				if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
					for (uint16_t i{}; i != cNetworkPlayerMgr->m_player_count; ++i) {
						auto playerCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(i), true);
						FIRE::ADD_EXPLOSION(playerCoords, 1, 1000, true, false, 0.1f, false);
					}
				}
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Clone")).setAction([] {
				if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
					for (uint16_t i{}; i != cNetworkPlayerMgr->m_player_count; ++i) {
						PED::CLONE_PED(PLAYER::GET_PLAYER_PED(i), true, true, true);
					}
				}
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Remove Weapons")).setAction([] {
				if (auto cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
					for (uint16_t i{}; i != cNetworkPlayerMgr->m_player_count; ++i) {
						WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED(i), true);
					}
				}
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("All Players")).setId("home.network.all_players"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Friendly")).setId("home.network.all_players.friendly"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Greifing")).setId("home.network.all_players.greifing"_id));
		}));
		static auto friendReg = pointers::g_friendRegistry;
		g_dxUiManager.addSub(sub().setName(xorstr_("InvalidPlayer")).setId("home.network.friends.selected"_id).setAction([](sub* sub) {
			auto data = friendReg->get(g_selectedFriend);
			if (data->m_is_joinable) {
				sub->addOption(vecOpt<char const*, int>().setLeft(xorstr_("Join")).setArray(hashes::ridJoinTypes).setPos(hashes::ridJoinTypesPos).setAction([] {
					auto data = friendReg->get(g_selectedFriend);
					features::ridJoin(data->m_rockstar_id, hashes::ridJoinTypesPos);
				}));
			}
			sub->addOption(regularOpt().setLeft(xorstr_("Set Spoofing Profile To Their RID")).setAction([] {
				auto data = friendReg->get(g_selectedFriend);
				features::spoofing::rid.first = data->m_rockstar_id;
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Copy RID")).setAction([] {
				auto data = friendReg->get(g_selectedFriend);
				auto rid = std::to_string(data->m_rockstar_id);
				ImGui::SetClipboardText(rid.c_str());
				g_dxUiManager.sendNotify(xorstr_("Friendlist"), xorstr_("Set clipboard to ") + rid + xorstr_("."));
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Copy Name")).setAction([] {
				auto data = friendReg->get(g_selectedFriend);
				ImGui::SetClipboardText(data->m_name);
				g_dxUiManager.sendNotify(xorstr_("Friendlist"), xorstr_("Set clipboard to ") + std::string(data->m_name) + xorstr_("."));
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Friends")).setId("home.network.friends"_id).setAction([](sub* sub) {
			for (int i{}; i != friendReg->m_friend_count; i++) {
				auto offlineStr = " " + getFriendStateStr(friendReg->get(i)->m_friend_state, friendReg->get(i)->m_is_joinable);
				sub->addOption(subOpt().setLeft(friendReg->get(i)->m_name + offlineStr).setId("home.network.friends.selected"_id).setAction([=] {
					g_dxUiManager.getSubData("home.network.friends.selected"_id)->setName(friendReg->get(i)->m_name);
					g_selectedFriend = i;
				}), false);
			}
		}));
		static features::scapi::account selectedResult{};
		g_dxUiManager.addSub(sub().setName(xorstr_("InvalidPlayer")).setId("home.network.rid_tools.player_search.selected_player"_id).setAction([](sub* sub) {
			if (selectedResult.online)
				sub->addOption(vecOpt<char const*, int>().setLeft(xorstr_("Join")).setArray(hashes::ridJoinTypes).setPos(hashes::ridJoinTypesPos).setAction([] {
					features::ridJoin(selectedResult.rid, hashes::ridJoinTypesPos);
				}));
			sub->addOption(regularOpt().setLeft(xorstr_("Set Spoofing Profile To Their RID")).setAction([] {
				features::spoofing::rid.first = selectedResult.rid;
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Copy RID")).setAction([] {
				auto rid = std::to_string(selectedResult.rid);
				ImGui::SetClipboardText(rid.c_str());
				g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Set clipboard to ") + rid + xorstr_("."));
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Copy Name")).setAction([] {
				ImGui::SetClipboardText(selectedResult.name.c_str());
				g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Set clipboard to ") + selectedResult.name + xorstr_("."));
			}));
		}));
		static std::pair<std::vector<features::scapi::account>, features::scapi::result> res{};
		static size_t page{};
		static std::string nm{};
		g_dxUiManager.addSub(sub().setName(xorstr_("Player Search")).setId("home.network.rid_tools.player_search"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Next Page")).setAction([=] {
				page++;
				std::thread t([=] {
					res = features::scapi::findPlayer(nm, page);
				});
				t.detach();
			}));
			if (page) {
				sub->addOption(regularOpt().setLeft(xorstr_("Previous Page")).setAction([=] {
					page--;
					std::thread t([=] {
						res = features::scapi::findPlayer(nm, page);
					});
					t.detach();
				}));
			}
			sub->addOption(breakOpt().setLeft(xorstr_("Results")));
			for (auto& plyr : res.first) {
				sub->addOption(subOpt().setLeft(plyr.name).setId("home.network.rid_tools.player_search.selected_player"_id).setAction([=] {
					selectedResult = plyr;
					g_dxUiManager.getSubData("home.network.rid_tools.player_search.selected_player"_id)->setName(std::format("{} | Online: {}", plyr.name, plyr.online ? "Yes" : "No"));
				}), false);
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("RID Tools")).setId("home.network.rid_tools"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Player search")).setId("home.network.rid_tools.player_search"_id).setAction([=] {
				std::thread t([=] {
					inputText(sub, [=](std::string str) {
						if (isNumber(str)) {
							nm = features::scapi::ridToName(stoi(str));
						}
						else {
							nm = str;
						}
						res = features::scapi::findPlayer(nm, page);
					}, 20);
					while (nm.empty()) {
						std::this_thread::sleep_for(1ms);
					}
				});
				t.detach();
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Convert")).setAction([=] {
				inputText(sub, [](std::string str) {
					std::string value{};
					if (isNumber(str))
						value = features::scapi::ridToName(stoi(str));
					else
						value = std::to_string(features::scapi::nameToRid(str));
					ImGui::SetClipboardText(value.c_str());
					g_dxUiManager.sendNotify(xorstr_("RID Tools"), xorstr_("Set clipboard to ") + std::string(value) + xorstr_("."));
				});
			}));
			sub->addOption(vecOpt<char const*, int>().setLeft(xorstr_("Join")).setArray(hashes::ridJoinTypes).setPos(hashes::ridJoinTypesPos).setAction([=] {
				inputText(sub, [](std::string str) {
					uint64_t rid{ stoull(str) };
					//features::scapi::stringToRid(str, xorstr_("RID Join"), rid);
					features::ridJoin(rid, hashes::ridJoinTypesPos);
				});
			}));
		}));
		//TODO: Spam chat with multiple strings, like arrays of strings
		g_dxUiManager.addSub(sub().setName(xorstr_("Message Presets")).setId("home.network.chat.spammer.msg_presets"_id).setAction([](sub* sub) {

		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Spammer")).setId("home.network.chat.spammer"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Message Presets")).setId("home.network.chat.spammer.msg_presets"_id));
			sub->addOption(regularOpt().setLeft(xorstr_("Message:")).setRight(features::chatSpamMsg).setAction([=] {
				inputText(sub, [](std::string str) {
					features::chatSpamMsg = str;
				});
			}));
			if (!features::chatSpamMsg.empty()) {
				sub->addOption(boolOpt().setLeft(xorstr_("Team Chat")).setBool(features::chatSpamInTeamBool));
				sub->addOption(numOpt<int>().setLeft(xorstr_("Delay (MS)")).setMinMax(0, 50 * 1000).setStep(5).setNum(features::chatSpamDelayInMs));
				sub->addOption(boolOpt().setLeft(xorstr_("Chat Spam")).setBool(features::chatSpamBool));
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Chat")).setId("home.network.chat"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Spammer")).setId("home.network.chat.spammer"_id));
			static std::string chatMsg{};
			sub->addOption(regularOpt().setLeft(xorstr_("Set message")).setDesc(chatMsg.empty() ? xorstr_("No current message") : std::format("Current message is '{}'", chatMsg)).setAction([=] {
				inputText(sub, [](std::string str) {
					chatMsg = str;
				});
			}));
			if (!chatMsg.empty()) {
				static bool teamChat{};
				sub->addOption(boolOpt().setLeft(xorstr_("Team Chat")).setBool(teamChat));
				sub->addOption(regularOpt().setLeft(xorstr_("Send")).setAction([] {
					features::chatSender(chatMsg, teamChat);
				}));
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Name")).setId("home.network.spoofing.clan.name"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::clan::name.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					features::spoofing::clan::name.first = str;
				}, 20);
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::clan::name.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Tag")).setId("home.network.spoofing.clan.tag"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::clan::tag.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					features::spoofing::clan::tag.first = str;
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::clan::tag.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Motto")).setId("home.network.spoofing.clan.motto"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::clan::motto.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					features::spoofing::clan::motto.first = str;
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::clan::motto.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Rank Name")).setId("home.network.spoofing.clan.rank_name"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::clan::rankName.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					features::spoofing::clan::rankName.first = str;
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::clan::rankName.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Rank Title")).setId("home.network.spoofing.clan.rank_title"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::clan::rankTitle.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					features::spoofing::clan::rankTitle.first = str;
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::clan::rankTitle.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Color")).setId("home.network.spoofing.clan.color"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::clan::color.first)).setLeft(xorstr_("Set")).setAction([=] {
				inputText(sub, [](std::string str) {
					if (isNumber(str))
						features::spoofing::clan::color.first = std::stoi(str);
					else
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} contains a string, the required input type is a integer"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::clan::color.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Clan ID")).setId("home.network.spoofing.clan.clan_id"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::clan::clanId.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (isNumber(str))
						features::spoofing::clan::clanId.first = std::stoi(str);
					else
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} contains a string, the required input type is a integer"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::clan::clanId.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Creation Time")).setId("home.network.spoofing.clan.creation_time"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::clan::creationTime.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (isNumber(str))
						features::spoofing::clan::creationTime.first = std::stoi(str);
					else
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} contains a string, the required input type is a integer"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::clan::creationTime.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Member ID")).setId("home.network.spoofing.clan.member_id"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::clan::memberId.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (isNumber(str))
						features::spoofing::clan::memberId.first = std::stoi(str);
					else
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} contains a string, the required input type is a integer"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::clan::memberId.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Member Count")).setId("home.network.spoofing.clan.member_count"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::clan::memberCount.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (isNumber(str))
						features::spoofing::clan::memberCount.first = std::stoi(str);
					else
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} contains a string, the required input type is a integer"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::clan::memberCount.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Clan")).setId("home.network.spoofing.clan"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Name")).setId("home.network.spoofing.clan.name"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Tag")).setId("home.network.spoofing.clan.tag"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Motto")).setId("home.network.spoofing.clan.motto"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Rank Name")).setId("home.network.spoofing.clan.rank_name"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Rank Title")).setId("home.network.spoofing.clan.rank_title"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Color")).setId("home.network.spoofing.clan.color"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Clan ID")).setId("home.network.spoofing.clan.clan_id"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Creation Time")).setId("home.network.spoofing.clan.creation_time"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Member ID")).setId("home.network.spoofing.clan.member_id"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Member Count")).setId("home.network.spoofing.clan.member_count"_id));
			sub->addOption(boolOpt().setLeft(xorstr_("Private Crew")).setBool(features::spoofing::clan::privateCrew));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Name")).setId("home.network.spoofing.name"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::name.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					features::spoofing::name.first = str;
				}, 20);
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::name.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("IP")).setId("home.network.spoofing.socket_address.relay.ip"_id).setAction([](sub* sub) {
			auto& addr = features::spoofing::socketAddress.relayAddr;
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}.{}.{}.{}", addr.first.m_ip.m_field1, addr.first.m_ip.m_field2, addr.first.m_ip.m_field3, addr.first.m_ip.m_field4)).setAction([=] {
				inputText(sub, [=](std::string str) {
					auto& addr = features::spoofing::socketAddress.relayAddr;
					auto fields = g_splitStr(str, '.');
					for (int8_t i{}; i != fields.size(); ++i) {
						if (isNumber(fields[i])) {
							if (i > 255) {
								g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("Field {} is too big, the value cannot be bigger than 255"), i);
								return;
							}
							switch (i) {
							case 0: addr.first.m_ip.m_field1 = (uint8_t)std::stoi(fields[i]); break;
							case 1: addr.first.m_ip.m_field2 = (uint8_t)std::stoi(fields[i]); break;
							case 2: addr.first.m_ip.m_field3 = (uint8_t)std::stoi(fields[i]); break;
							case 3: addr.first.m_ip.m_field4 = (uint8_t)std::stoi(fields[i]); break;
							}
						}
						else {
							g_dxUiManager.sendNotify(xorstr_("Spoofing:"), xorstr_("Field {} contains a string, the required input type is a integer"), i);
						}
					}
					if (fields.size() != 4) {
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} only contains {} fields, the required amount of fields is 4"), str, fields.size());
						return;
					}
				}, 15);
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(addr.second.first));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Port")).setId("home.network.spoofing.socket_address.relay.port"_id).setAction([](sub* sub) {
			auto& addr = features::spoofing::socketAddress.relayAddr;
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", addr.first.m_port)).setAction([=] {
				inputText(sub, [=](std::string str) {
					auto& addr = features::spoofing::socketAddress.relayAddr;
					if (isNumber(str))
						addr.first.m_port = (uint16_t)std::stoi(str);
					else
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} contains a string, the required input type is a integer"), str);
				}, 5);
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(addr.second.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Relay")).setId("home.network.spoofing.socket_address.relay"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("IP")).setId("home.network.spoofing.socket_address.relay.ip"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Port")).setId("home.network.spoofing.socket_address.relay.port"_id));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("IP")).setId("home.network.spoofing.socket_address.public.ip"_id).setAction([](sub* sub) {
			auto& addr = features::spoofing::socketAddress.publicAddr;
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}.{}.{}.{}", addr.first.m_ip.m_field1, addr.first.m_ip.m_field2, addr.first.m_ip.m_field3, addr.first.m_ip.m_field4)).setAction([=] {
				inputText(sub, [=](std::string str) {
					auto& addr = features::spoofing::socketAddress.publicAddr;
					auto fields = g_splitStr(str, '.');
					for (int8_t i{}; i != fields.size(); ++i) {
						if (isNumber(fields[i])) {
							if (i > 255) {
								g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("Field {} is too big, the value cannot be bigger than 255"), i);
								return;
							}
							switch (i) {
							case 0: addr.first.m_ip.m_field1 = (uint8_t)std::stoi(fields[i]); break;
							case 1: addr.first.m_ip.m_field2 = (uint8_t)std::stoi(fields[i]); break;
							case 2: addr.first.m_ip.m_field3 = (uint8_t)std::stoi(fields[i]); break;
							case 3: addr.first.m_ip.m_field4 = (uint8_t)std::stoi(fields[i]); break;
							}
						}
						else {
							g_dxUiManager.sendNotify(xorstr_("Spoofing:"), xorstr_("Field {} contains a string, the required input type is a integer"), i);
						}
					}
					if (fields.size() != 4) {
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} only contains {} fields, the required amount of fields is 4"), str, fields.size());
						return;
					}
				}, 15);
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(addr.second.first));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Port")).setId("home.network.spoofing.socket_address.public.port"_id).setAction([](sub* sub) {
			auto& addr = features::spoofing::socketAddress.publicAddr;
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", addr.first.m_port)).setAction([=] {
				inputText(sub, [=](std::string str) {
					auto& addr = features::spoofing::socketAddress.publicAddr;
					if (isNumber(str))
						addr.first.m_port = (uint16_t)std::stoi(str);
					else
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} contains a string, the required input type is a integer"), str);
				}, 5);
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(addr.second.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Public")).setId("home.network.spoofing.socket_address.public"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("IP")).setId("home.network.spoofing.socket_address.public.ip"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Port")).setId("home.network.spoofing.socket_address.public.port"_id));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("IP")).setId("home.network.spoofing.socket_address.local.ip"_id).setAction([](sub* sub) {
			auto& addr = features::spoofing::socketAddress.localAddr;
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}.{}.{}.{}", addr.first.m_ip.m_field1, addr.first.m_ip.m_field2, addr.first.m_ip.m_field3, addr.first.m_ip.m_field4)).setAction([=] {
				inputText(sub, [=](std::string str) {
					auto& addr = features::spoofing::socketAddress.localAddr;
					auto fields = g_splitStr(str, '.');
					for (int8_t i{}; i != fields.size(); ++i) {
						if (isNumber(fields[i])) {
							if (i > 255) {
								g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("Field {} is too big, the value cannot be bigger than 255"), i);
								return;
							}
							auto data = uint8_t(std::stoi(fields[i]));
							switch (i) {
							case 0: addr.first.m_ip.m_field1 = data; break;
							case 1: addr.first.m_ip.m_field2 = data; break;
							case 2: addr.first.m_ip.m_field3 = data; break;
							case 3: addr.first.m_ip.m_field4 = data; break;
							}
						}
						else {
							g_dxUiManager.sendNotify(xorstr_("Spoofing:"), xorstr_("Field {} contains a string, the required input type is a integer"), i);
						}
					}
					if (fields.size() != 4) {
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} only contains {} fields, the required amount of fields is 4"), str, fields.size());
						return;
					}
				}, 15);
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(addr.second.first));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Port")).setId("home.network.spoofing.socket_address.local.port"_id).setAction([](sub* sub) {
			auto& addr = features::spoofing::socketAddress.localAddr;
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", addr.first.m_port)).setAction([=] {
				inputText(sub, [=](std::string str) {
					auto& addr = features::spoofing::socketAddress.localAddr;
					if (isNumber(str))
						addr.first.m_port = (uint16_t)std::stoi(str);
					else
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} contains a string, the required input type is a integer"), str);
				}, 5);
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(addr.second.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Local")).setId("home.network.spoofing.socket_address.local"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("IP")).setId("home.network.spoofing.socket_address.local.ip"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Port")).setId("home.network.spoofing.socket_address.local.port"_id));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Socket Address")).setId("home.network.spoofing.socket_address"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Relay")).setId("home.network.spoofing.socket_address.relay"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Public")).setId("home.network.spoofing.socket_address.public"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Local")).setId("home.network.spoofing.socket_address.local"_id));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("RID")).setId("home.network.spoofing.rid"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("{}", features::spoofing::rid.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (isNumber(str))
						features::spoofing::rid.first = (uint64_t)std::stoll(str);
					else
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} contains a string, the required input type is a integer"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::rid.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Peer Address")).setId("home.network.spoofing.peer_address"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Value:")).setRight(std::format("0x{:X}", features::spoofing::peerAddress.first)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (containsANumber(str))
						features::spoofing::peerAddress.first = (uint64_t)std::stoull(str, 0, 16);
					else
						g_dxUiManager.sendNotify(xorstr_("Spoofing"), xorstr_("The value {} contains a string, the required input type is a integer"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(features::spoofing::peerAddress.second));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Flags")).setId("home.network.spoofing.flags"_id).setAction([](sub* sub) {
			sub->addOption(boolOpt().setLeft(xorstr_("R* Dev")).setBool(features::spoofing::flags::rockstarDev));
			sub->addOption(boolOpt().setLeft(xorstr_("R* QA")).setBool(features::spoofing::flags::rockstarQA));
			sub->addOption(boolOpt().setLeft(xorstr_("Is Cheater")).setBool(features::spoofing::flags::isCheater));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Spoofing")).setId("home.network.spoofing"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Clan")).setId("home.network.spoofing.clan"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Name")).setId("home.network.spoofing.name"_id));
			//sub->addOption(subOpt().setLeft(xorstr_("Socket Address")).setId("home.network.spoofing.socket_address"_id));
			//sub->addOption(subOpt().setLeft(xorstr_("RID")).setId("home.network.spoofing.rid"_id));
			//sub->addOption(subOpt().setLeft(xorstr_("Peer Address")).setId("home.network.spoofing.peer_address"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Flags")).setId("home.network.spoofing.flags"_id));
			sub->addOption(boolOpt().setLeft(xorstr_("Hide Entity Proofs")).setBool(features::spoofing::hideEntityProofs));
			sub->addOption(boolOpt().setLeft(xorstr_("Hide Player Spectating")).setBool(features::spoofing::hidePlayerSpectating));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Tunnel")).setId("home.network.tunnel"_id).setAction([](sub* sub) {
			static uint64_t rid{};
			sub->addOption(regularOpt().setLeft(xorstr_("RID")).setDesc(xorstr_("Enter the RID you wish to create a tunnel to")).setRight(std::format("{}", rid)).setAction([=] {
				inputText(sub, [=](std::string str) {
					rid = std::stoull(str, 0, 16);
					features::scapi::stringToRid(str, xorstr_("RID Tunneling"), rid);
				});
			}));
			if (rid) {
				auto tun = std::make_unique<tunnel>(rid);
				static auto connectionData{ std::make_pair<int32_t, int32_t>(-1, -1) };
				sub->addOption(vecOpt<const char*, int>().setLeft(xorstr_("Net Message")).setDesc(xorstr_("Type of net message to send via the tunnel")).setArray(hashes::netMessages).setPos(hashes::netMessagesPos));
				sub->addOption(breakOpt().setLeft(xorstr_("Connection Data")));
				sub->addOption(numOpt<int32_t>().setLeft(xorstr_("Type")).setDesc(xorstr_("It is recommended that you leave these values to '-1'")).setMinMax(-1, 5).setStep(1).setNum(connectionData.first));
				sub->addOption(numOpt<int32_t>().setLeft(xorstr_("Reason")).setDesc(xorstr_("It is recommended that you leave these values to '-1")).setMinMax(-1, 1).setStep(1).setNum(connectionData.second));
				sub->addOption(breakOpt().setLeft(xorstr_("Sender")));
				sub->addOption(regularOpt().setLeft(xorstr_("Send Packet")).setAction([&] {
					if (connectionData.first == -1 && connectionData.second == -1) {
						for (int i{}; i != 6; ++i) {
							for (int j{}; j != 2; ++j) {
								tun->establish(j, i);
								tun->handleTunnel(hashes::netMessagesPos);
							}
						}
					}
					else {
						tun->establish(connectionData.first, connectionData.second);
						tun->handleTunnel(hashes::netMessagesPos);
					}
				}));
				sub->addOption(regularOpt().setLeft(xorstr_("Close Tunnel")).setAction([&] {
					tun.reset();
				}));
				sub->addOption(regularOpt().setLeft(xorstr_("Test")).setAction([&] {
					remoteKick(rid, connectionData.first, connectionData.second);
				}));
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Network")).setId("home.network"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Sessions")).setDesc(xorstr_("Custom Matchmaking is required for this feature")).setId("home.network.sessions"_id).setAction([=] {
				if (!features::matchmaking::replaceRSMatchmaking) {
					g_dxUiManager.sendNotify(xorstr_("Session Browser"), xorstr_("You cannot use the session browser without using the custom matchmaking system. Please enable the feature below"));
					g_dxUiManager.switchToSub("home.network"_id);
					return;
				}
			}));
			sub->addOption(subOpt().setLeft(xorstr_("Players")).setId("home.network.players"_id));
			sub->addOption(subOpt().setLeft(xorstr_("All Players")).setId("home.network.all_players"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Friends")).setId("home.network.friends"_id));
			sub->addOption(subOpt().setLeft(xorstr_("RID Tools")).setId("home.network.rid_tools"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Chat")).setId("home.network.chat"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Spoofing")).setId("home.network.spoofing"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Tunnel")).setId("home.network.tunnel"_id));
			sub->addOption(boolOpt().setLeft(xorstr_("Custom Matchmaking")).setBool(features::matchmaking::replaceRSMatchmaking));
			sub->addOption(boolOpt().setLeft(xorstr_("Join inside SCTV-Only Slots")).setBool(features::joinInsideSCTVSlots));
			sub->addOption(boolOpt().setLeft(xorstr_("R* Dev DLC")).setBool(features::loadDevDLC));
			sub->addOption(boolOpt().setLeft(xorstr_("Chat Filter")).setBool(features::chatFilter));	
			sub->addOption(vecOpt<char const*, int>().setLeft(xorstr_("Region")).setArray(hashes::regionTypes).setPos(hashes::regionTypesPos).setAction([] {
				*pointers::g_region = features::region(hashes::regionTypesPos, true);
				g_dxUiManager.sendNotify(xorstr_("Region"), xorstr_("Region set to {}{}"), NETWORK::NETWORK_IS_SESSION_ACTIVE() ? ", please join a new session to apply the changes" : "");
			}));
			sub->addOption(vecOpt<char const*, int>().setLeft(xorstr_("Session Starter")).setArray(hashes::sessionTypes).setPos(hashes::sessionTypesPos).setAction([] {
				if (!features::loadDevDLC && hashes::sessionTypesPos == 9) {
					g_dxUiManager.sendNotify(xorstr_("Session Starter"), xorstr_("You cannot join a SCTV-Only session without the Developer DLC loaded. Please either load the DLC or chose another option"));
					return;	
				}
				features::sessionJoin(hashes::sessionTypesPos, true);
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Join via gsinfo")).setDesc(xorstr_("This allows you to join a players session without having their RID or info")).setAction([=] {
				inputText(sub, [](std::string str) {
					features::ridJoinJBGSI(str.c_str());
				});
            }));
			if (util::network::isSessionActive()) {
				sub->addOption(regularOpt().setLeft(xorstr_("Copy gsinfo")).setAction([=] {
					ImGui::SetClipboardText(features::getGsInfo().c_str());
				}));
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("World")).setId("home.miscellaneous.world"_id).setAction([](sub* sub) {

		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Dispatch")).setId("home.miscellaneous.dispatch"_id).setAction([](sub* sub) {
			sub->addOption(boolOpt().setLeft(xorstr_("Police Automobile Dispatch")).setBool(features::policeAutomobileDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Police Dispatch")).setBool(features::policeHelicopterDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Fire Department Dispatch")).setBool(features::fireDepartmentDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Swat Automobile Dispatch")).setBool(features::swatAutomobileDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Ambulance Department Dispatch")).setBool(features::ambulanceDepartmentDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Police Riders Dispatch")).setBool(features::policeRidersDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Police Vehicle Request Dispatch")).setBool(features::policeVehicleRequestDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Police Dispatch")).setBool(features::policeRoadBlockDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Police Automobile Wait Pulled Over Dispatch")).setBool(features::policeAutomobileWaitPulledOverDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Police Automobile Wait Curising Dispatch")).setBool(features::policeAutomobileWaitCruisingDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Gangs Dispatch")).setBool(features::gangsDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Swat Helicopter Dispatch")).setBool(features::swatHelicopterDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Police Boat Dispatch")).setBool(features::policeBoatDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Army Vehicle Dispatch")).setBool(features::armyVehicleDispatch));
			sub->addOption(boolOpt().setLeft(xorstr_("Biker Backup Dispatch")).setBool(features::bikerBackupDispatch));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Radio")).setId("home.miscellaneous.radio"_id).setAction([](sub* sub) {
			sub->addOption(boolOpt().setLeft(xorstr_("Mobile Radio")).setBool(features::mobileRadioBool));
			sub->addOption(regularOpt().setLeft(xorstr_("Skip current song")).setAction([] {
				AUDIO::SKIP_RADIO_FORWARD();
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Visions")).setId("home.miscellaneous.visions"_id).setAction([](sub* sub) {
			sub->addOption(boolOpt().setLeft(xorstr_("Thermal Vision")).setBool(features::thermalVisionBool));
			sub->addOption(regularOpt().setLeft("Reset").setAction([=] {
				GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
				if (features::thermalVisionBool)
					features::thermalVisionBool = false;
			}));
			sub->addOption(breakOpt().setLeft(xorstr_("Visions")));
			for (size_t i{}; i != SIZEOF(hashes::visions); ++i) {
				sub->addOption(iconOpt().setLeft(util::localization::getGxtLabel(hashes::visions[i])).setIcon(hashes::visionsPos == i ? 'g' : '\0').setAction([=] {
					hashes::visionsPos = i;
					GRAPHICS::SET_TIMECYCLE_MODIFIER(hashes::visions[hashes::visionsPos]);
				}), false);
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Textures")).setId("home.miscellaneous.texture_replacement.add_replacement.textures"_id).setAction([](sub* sub) {
			auto filePath = fs::path(std::getenv(xorstr_("appdata"))).append(xorstr_(BRANDING_NAME)).append(xorstr_("Textures"));
			if (fs::exists(filePath)) {
				fs::directory_iterator dirIt{ filePath.string() };
				static std::string hdrName{};
				for (auto&& dirEntry : dirIt) {
					if (dirEntry.is_regular_file()) {
						auto path = dirEntry.path();
						if (path.has_filename()) {
							if (images::g_containsPicExtension(path)) {
								sub->addOption(iconOpt().setLeft(path.filename().string()).setIcon(g_selectedTextureReplacement == path ? 'g' : '\0').setAction([=] {
									g_selectedTextureReplacement = path;
								}), false);
							}
						}
					}
				}
			}
		}));
		static bool replaceAll{};
		g_dxUiManager.addSub(sub().setName(xorstr_("Add Replacement")).setId("home.miscellaneous.texture_replacement.add_replacement"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Dictionary")).setRight(textureSwapAsset.dictionary).setDesc(xorstr_("Native dictionary name (ex: commonmenu)")).setAction([=] {
				inputText(sub, [=](std::string str) {
					if (str.empty())
						g_dxUiManager.sendNotify(xorstr_("Textures"), xorstr_("The dictionary cannot be empty. Please input a value"));
					textureSwapAsset.dictionary = str;
					for (uint8_t i{}; i != 2; ++i) {
						g_requestDictionary(str);
					}
					bool found{};
					for (uint8_t i{ 3 }; i; --i) {
						if (auto pgDictionary = features::findDictionary(textureSwapAsset.dictionary); pgDictionary) {
							found = true;
						}
					}
					if (!found)
						g_dxUiManager.sendNotify(xorstr_("Textures"), xorstr_("Failed to find the dictionary '{}'"), textureSwapAsset.dictionary);
				});
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Name")).setRight(textureSwapAsset.name).setDesc(xorstr_("Name in dictionary (ex: shop_arrows_upanddown)")).setAction([=] {
				inputText(sub, [=](std::string str) {
					textureSwapAsset.name = str;
					if (str.empty())
						g_dxUiManager.sendNotify(xorstr_("Textures"), xorstr_("The name cannot be empty. Please input a value"));
					bool found{ features::findTexture(textureSwapAsset.dictionary, textureSwapAsset.name) ? true : false};
					if (!found)
						g_dxUiManager.sendNotify(xorstr_("Textures"), xorstr_("Failed to find '[]' inside the dictionary '{}'"), textureSwapAsset.name, textureSwapAsset.dictionary);
				});
			}));
			sub->addOption(breakOpt().setLeft(xorstr_("Replacement")));
			sub->addOption(subOpt().setLeft(xorstr_("Textures")).setId("home.miscellaneous.texture_replacement.add_replacement.textures"_id));
			sub->addOption(regularOpt().setLeft(xorstr_("Replace")).setAction([=] {
				auto createdTexture = features::createTexture(g_selectedTextureReplacement);
				if (auto grcTexture = features::findTexture(textureSwapAsset.dictionary, textureSwapAsset.name)) {
					features::textureReplace replacement(textureSwapAsset, true);
					replacement.m_texture = std::make_pair(createdTexture, g_selectedTextureReplacement.stem().string());
					replacement.m_original = grcTexture->CreateCopy();
					features::textureReplacements.push_back(replacement);
				}
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Replace All In Dictionary")).setAction([=] {
				auto texture = features::createTexture(g_selectedTextureReplacement);
				if (auto pgDictionary = features::findDictionary(textureSwapAsset.dictionary)) {
					for (auto& t : pgDictionary->GetAllTextureNames()) {
						features::textureReplace replacement(features::assetData(textureSwapAsset.dictionary, t), true, true);
						auto grcTexture = pgDictionary->FindTexture(t);
						replacement.m_texture = std::make_pair(texture, g_selectedTextureReplacement.stem().string());
						replacement.m_original = grcTexture->CreateCopy();
						features::textureReplacements.push_back(replacement);
					}
				}
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Replace All")).setBool(replaceAll).setAction([=] {
				if (!replaceAll) {
					for (auto& t : features::textureReplacements) {
						t.m_swap = false;
						t.m_active = replaceAll;
					}
					return;
				}
				auto texture = features::createTexture(g_selectedTextureReplacement);
				if (const auto grcTextureStore = pointers::g_textureStore; grcTextureStore && replaceAll) {
					for (auto& d : grcTextureStore->GetAllDictionaries()) {
						if (auto& pgDictionary = d.second) {
							for (auto& t : pgDictionary->GetAllTextureNames()) {
								features::textureReplace replacement(features::assetData(d.first, t), true, true);
								auto grcTexture = pgDictionary->FindTexture(t);
								replacement.m_texture = std::make_pair(texture, g_selectedTextureReplacement.stem().string());
								replacement.m_original = grcTexture->CreateCopy();
								features::textureReplacements.push_back(replacement);
							}
						}
					}
				}
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Texture Replacement")).setId("home.miscellaneous.texture_replacement"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Add Replacement")).setId("home.miscellaneous.texture_replacement.add_replacement"_id));
			if (!replaceAll) {
				sub->addOption(breakOpt().setLeft(xorstr_("Replacements")));
				for (auto& t : features::textureReplacements) {
					sub->addOption(boolOpt().setLeft(std::format("{}:{}", t.m_asset.dictionary, t.m_asset.name)).setDesc(std::format("Swapped with {}", t.m_texture.second)).setBool(t.m_active), false);
				}
			}
			else {
				sub->addOption(breakOpt().setLeft(xorstr_("Replace all is active")));
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Miscellaneous")).setId("home.miscellaneous"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("World")).setId("home.miscellaneous.world"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Dispatch")).setId("home.miscellaneous.dispatch"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Radio")).setId("home.miscellaneous.radio"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Visions")).setId("home.miscellaneous.visions"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Texture Replacement")).setId("home.miscellaneous.texture_replacement"_id));
			sub->addOption(boolOpt().setLeft(xorstr_("Bypass sticky limit")).setBool(features::bypassStickyLimitBool));
			sub->addOption(boolOpt().setLeft(xorstr_("Bypass flare limit")).setBool(features::bypassFlareLimitBool));
			sub->addOption(regularOpt().setLeft(xorstr_("Bail")).setDesc(xorstr_("Bail to story mode")).setAction([] {
				NETWORK::NETWORK_BAIL(0, 0, 0);
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Force quit to story")).setAction([] {
				NETWORK::SHUTDOWN_AND_LOAD_MOST_RECENT_SAVE();
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Reset streaming focus")).setAction([] {
				STREAMING::CLEAR_FOCUS();
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Script Events")).setId("home.protections.script_events"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Toggle all")).setAction([] {
				for (auto& evnt : protections::m_scriptEvents)
					evnt.toggle = { !evnt.toggle.toggle, tog_notifyAndBlock };
			}));
			for (auto& evnt : protections::m_scriptEvents)
				sub->addOption(vecBoolOpt<std::string, int>().setLeft(evnt.name).setArray(m_toggleTypes).setPos(evnt.toggle.toggleType).setBool(evnt.toggle.toggle));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Network Events")).setId("home.protections.network_events"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Toggle all")).setAction([] {
				protections::g_blockExplosions = { !protections::g_blockExplosions.toggle, tog_notifyAndBlock };
				protections::g_blockFreeze = { !protections::g_blockFreeze.toggle, tog_notifyAndBlock };
				protections::g_blockRequestControl = { !protections::g_blockRequestControl.toggle, tog_notifyAndBlock };
				protections::g_blockGiveWeapon = { !protections::g_blockGiveWeapon.toggle, tog_notifyAndBlock };
				protections::g_blockRemoveWeapon = { !protections::g_blockRemoveWeapon.toggle, tog_notifyAndBlock };
				protections::g_blockRemoveAllWeapons = { !protections::g_blockRemoveAllWeapons.toggle, tog_notifyAndBlock };
				protections::g_blockVoteKicks = { !protections::g_blockVoteKicks.toggle, tog_notifyAndBlock };
				protections::g_blockAlterWantedLvl = { !protections::g_blockAlterWantedLvl.toggle, tog_notifyAndBlock };
			}));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Explosions")).setArray(m_toggleTypes).setPos(protections::g_blockExplosions.toggleType).setBool(protections::g_blockExplosions.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Freeze")).setArray(m_toggleTypes).setPos(protections::g_blockFreeze.toggleType).setBool(protections::g_blockFreeze.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Request Control")).setArray(m_toggleTypes).setPos(protections::g_blockRequestControl.toggleType).setBool(protections::g_blockRequestControl.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Give Weapon")).setArray(m_toggleTypes).setPos(protections::g_blockGiveWeapon.toggleType).setBool(protections::g_blockGiveWeapon.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Remove Weapon")).setArray(m_toggleTypes).setPos(protections::g_blockRemoveWeapon.toggleType).setBool(protections::g_blockRemoveWeapon.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Remove All Weapons")).setArray(m_toggleTypes).setPos(protections::g_blockRemoveAllWeapons.toggleType).setBool(protections::g_blockRemoveAllWeapons.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Vote kicks")).setArray(m_toggleTypes).setPos(protections::g_blockVoteKicks.toggleType).setBool(protections::g_blockVoteKicks.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Alter Wanted Lvl")).setArray(m_toggleTypes).setPos(protections::g_blockAlterWantedLvl.toggleType).setBool(protections::g_blockAlterWantedLvl.toggle));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Crashes")).setId("home.protections.crashes"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Crash Models")).setId("home.protections.crashes.crash_models"_id));
			sub->addOption(regularOpt().setLeft(xorstr_("Toggle all")).setAction([] {
				protections::g_blockHostCrash = { !protections::g_blockHostCrash.toggle, tog_notifyAndBlock };
				protections::g_blockInvalidAttachmentCrash = { !protections::g_blockInvalidAttachmentCrash.toggle, tog_notifyAndBlock };
				protections::g_blockNextGenVehicleCrash = { !protections::g_blockNextGenVehicleCrash.toggle, tog_notifyAndBlock };
				protections::g_blockNextGenPedCrash = { !protections::g_blockNextGenPedCrash.toggle, tog_notifyAndBlock };
				protections::g_blockSyncMismatchCrash = { !protections::g_blockSyncMismatchCrash.toggle, tog_notifyAndBlock };
				protections::g_blockOutOfRangeSyncCrash = { !protections::g_blockOutOfRangeSyncCrash.toggle, tog_notifyAndBlock };
				protections::g_blockOutOfRangeSyncCrash = { !protections::g_blockOutOfRangeSyncCrash.toggle, tog_notifyAndBlock };
				protections::g_blockVehicleTaskCrash = { !protections::g_blockVehicleTaskCrash.toggle, tog_notifyAndBlock };
				protections::g_blockGiveControlCrash = { !protections::g_blockGiveControlCrash.toggle, tog_notifyAndBlock };
			}));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Host")).setArray(m_toggleTypes).setPos(protections::g_blockHostCrash.toggleType).setBool(protections::g_blockHostCrash.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Invalid Attachment")).setArray(m_toggleTypes).setPos(protections::g_blockInvalidAttachmentCrash.toggleType).setBool(protections::g_blockInvalidAttachmentCrash.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Next Gen (Vehicle)")).setArray(m_toggleTypes).setPos(protections::g_blockNextGenVehicleCrash.toggleType).setBool(protections::g_blockNextGenVehicleCrash.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Next Gen (Ped)")).setArray(m_toggleTypes).setPos(protections::g_blockNextGenPedCrash.toggleType).setBool(protections::g_blockNextGenPedCrash.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Sync Mismatch")).setArray(m_toggleTypes).setPos(protections::g_blockSyncMismatchCrash.toggleType).setBool(protections::g_blockSyncMismatchCrash.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Out Of Range Sync")).setArray(m_toggleTypes).setPos(protections::g_blockOutOfRangeSyncCrash.toggleType).setBool(protections::g_blockOutOfRangeSyncCrash.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Vehicle Task")).setArray(m_toggleTypes).setPos(protections::g_blockVehicleTaskCrash.toggleType).setBool(protections::g_blockVehicleTaskCrash.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Give Control")).setArray(m_toggleTypes).setPos(protections::g_blockGiveControlCrash.toggleType).setBool(protections::g_blockGiveControlCrash.toggle));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Crash Models")).setId("home.protections.crashes.crash_models"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Toggle all")).setAction([] {
				protections::g_blockSlodCrash = { !protections::g_blockSlodCrash.toggle, tog_notifyAndBlock };
			}));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Slod")).setArray(m_toggleTypes).setPos(protections::g_blockSlodCrash.toggleType).setBool(protections::g_blockSlodCrash.toggle));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Kicks")).setId("home.protections.kicks"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Toggle all")).setAction([] {
				protections::g_blockRemovePlayerKick = { !protections::g_blockRemovePlayerKick.toggle, tog_notifyAndBlock };
				protections::g_blockRaiseComplaintKick = { !protections::g_blockRaiseComplaintKick.toggle, tog_notifyAndBlock };
			}));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Remove Player")).setArray(m_toggleTypes).setPos(protections::g_blockRemovePlayerKick.toggleType).setBool(protections::g_blockRemovePlayerKick.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Raise Complaint")).setArray(m_toggleTypes).setPos(protections::g_blockRaiseComplaintKick.toggleType).setBool(protections::g_blockRaiseComplaintKick.toggle));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Protections")).setId("home.protections"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Toggle all")).setAction([] {
				//Script Events
				for (auto& evnt : protections::m_scriptEvents)
					evnt.toggle = { !evnt.toggle.toggle, tog_notifyAndBlock };
				//Network Events
				protections::g_blockExplosions = { !protections::g_blockExplosions.toggle, tog_notifyAndBlock };
				protections::g_blockFreeze = { !protections::g_blockFreeze.toggle, tog_notifyAndBlock };
				protections::g_blockRequestControl = { !protections::g_blockRequestControl.toggle, tog_notifyAndBlock };
				protections::g_blockGiveWeapon = { !protections::g_blockGiveWeapon.toggle, tog_notifyAndBlock };
				protections::g_blockRemoveWeapon = { !protections::g_blockRemoveWeapon.toggle, tog_notifyAndBlock };
				protections::g_blockRemoveAllWeapons = { !protections::g_blockRemoveAllWeapons.toggle, tog_notifyAndBlock };
				protections::g_blockVoteKicks = { !protections::g_blockVoteKicks.toggle, tog_notifyAndBlock };
				protections::g_blockAlterWantedLvl = { !protections::g_blockAlterWantedLvl.toggle, tog_notifyAndBlock };
				//Crashes
				protections::g_blockHostCrash = { !protections::g_blockHostCrash.toggle, tog_notifyAndBlock };
				protections::g_blockInvalidAttachmentCrash = { !protections::g_blockInvalidAttachmentCrash.toggle, tog_notifyAndBlock };
				protections::g_blockNextGenVehicleCrash = { !protections::g_blockNextGenVehicleCrash.toggle, tog_notifyAndBlock };
				protections::g_blockNextGenPedCrash = { !protections::g_blockNextGenPedCrash.toggle, tog_notifyAndBlock };
				protections::g_blockSyncMismatchCrash = { !protections::g_blockSyncMismatchCrash.toggle, tog_notifyAndBlock };
				protections::g_blockOutOfRangeSyncCrash = { !protections::g_blockOutOfRangeSyncCrash.toggle, tog_notifyAndBlock };
				protections::g_blockVehicleTaskCrash = { !protections::g_blockVehicleTaskCrash.toggle, tog_notifyAndBlock };
				protections::g_blockGiveControlCrash = { !protections::g_blockGiveControlCrash.toggle, tog_notifyAndBlock };
				//Crash Models
				protections::g_blockSlodCrash = { !protections::g_blockSlodCrash.toggle, tog_notifyAndBlock };
				//Reports
				protections::g_blockReports = { !protections::g_blockReports.toggle, tog_notifyAndBlock };
			}));
			sub->addOption(subOpt().setLeft(xorstr_("Script Events")).setId("home.protections.script_events"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Network Events")).setId("home.protections.network_events"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Crashes")).setId("home.protections.crashes"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Kicks")).setId("home.protections.kicks"_id));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Reports")).setArray(m_toggleTypes).setPos(protections::g_blockReports.toggleType).setBool(protections::g_blockReports.toggle));
			sub->addOption(vecBoolOpt<std::string, int>().setLeft(xorstr_("Block joins from blacklisted players")).setArray(m_toggleTypes).setPos(protections::g_playerBlacklist.toggleType).setBool(protections::g_playerBlacklist.toggle));
			sub->addOption(boolOpt().setLeft(xorstr_("Block Joins In Private Sessions")).setDesc(xorstr_("Blocks players from RID joining you when you're inside a private session type (invite, friends, crew and solo)")).setBool(protections::g_blockPlayersFromRIDJoiningInPrivateSessions));
			sub->addOption(boolOpt().setLeft(xorstr_("Log script events")).setBool(protections::g_logScriptEvents));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("InvalidScript")).setId("home.script_loader.lua.selected"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Execute")).setAction([] {
				g_lua.executeLua(g_selectedLua);
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Lua")).setId("home.script_loader.lua"_id).setAction([](sub* sub) {
			auto filePath = fs::path(std::getenv(xorstr_("appdata"))).append(xorstr_(BRANDING_NAME)).append(xorstr_("Scripts"));
			if (fs::exists(filePath)) {
				sub->addOption(regularOpt().setLeft(xorstr_("Refresh")).setAction([] {
					for (auto& s : g_dxFiberMgr.m_fibers) {
						auto& f{ s.second };
						if (std::string(f->m_name).find("lua_") != -1)
							g_dxFiberMgr.remove(f->m_name);
					}
					for (auto& s : g_fiberMgr.m_fibers) {
						auto& f{ s.second };
						if (std::string(f->m_name).find("lua_") != -1)
							g_fiberMgr.remove(f->m_name);
					}
				}));
				fs::directory_iterator dirIt{ filePath.string() };
				for (auto&& dirEntry : dirIt) {
					if (dirEntry.is_regular_file()) {
						auto path = dirEntry.path();
						if (path.has_filename()) {
							if (path.extension() == ".lua") {
								sub->addOption(subOpt().setLeft(path.filename().string()).setId("home.script_loader.lua.selected"_id).setAction([=] {
									g_filePath = path;
									g_selectedLua = g_filePath.string();
									g_dxUiManager.getSubData("home.script_loader.lua.selected"_id)->setName(g_filePath.stem().string());
								}), false);
							}
						}
					}
				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Script Loader")).setId("home.script_loader"_id).setAction([](sub* sub) {
			auto filePath = fs::path(std::getenv(xorstr_("appdata"))).append(xorstr_(BRANDING_NAME)).append(xorstr_("Scripts"));
			if (fs::exists(filePath)) {
				sub->addOption(regularOpt().setLeft(xorstr_("Open folder")).setAction([filePath] {
					std::system(std::format("explorer.exe {}", filePath.string()).c_str());
				}));
			}
			sub->addOption(subOpt().setLeft(xorstr_("Lua")).setId("home.script_loader.lua"_id));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Colors")).setId("home.settings.ui.open_tooltip.colors"_id).setAction([](sub* sub) {
			sub->addOption(colorOpt().setLeft(xorstr_("Tooltip")).setColor(&g_dxUiManager.m_openTooltipTextColor));
			sub->addOption(colorOpt().setLeft(xorstr_("Open Key")).setColor(&g_dxUiManager.m_openTooltipKeyTextColor));
		}));
		static std::vector<std::string> tooltipsToAdd{};
		g_dxUiManager.addSub(sub().setName(xorstr_("InvalidTooltip")).setId("home.settings.ui.open_tooltip.tooltips.add_tooltips.tooltips.selected"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Replace")).setAction([=] {
				inputText(sub, [](std::string str) {
					if (str.compare(tooltipsToAdd[g_selectedCustomTooltip].c_str()))
						tooltipsToAdd[g_selectedCustomTooltip] = str;
				}, INPUT_MAX, tooltipsToAdd[g_selectedCustomTooltip].c_str());
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Remove")).setAction([=] {
				tooltipsToAdd.erase(tooltipsToAdd.begin() + g_selectedCustomTooltip);
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Tooltips")).setId("home.settings.ui.open_tooltip.tooltips.add_tooltips.tooltips"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Add")).setAction([=] {
				inputText(sub, [](std::string str) {
					if (str.empty()) {
						g_dxUiManager.sendNotify(xorstr_("Tooltips"), xorstr_("You cannot add a empty tooltip"));
						return;
					}
					tooltipsToAdd.push_back(str);
				});
			}));
			sub->addOption(breakOpt().setLeft(xorstr_("Tooltips")));
			for (size_t i{}; i != tooltipsToAdd.size(); ++i) {
				sub->addOption(subOpt().setLeft(tooltipsToAdd[i]).setId("home.settings.ui.open_tooltip.tooltips.add_tooltips.tooltips.selected"_id).setAction([=] {
					g_dxUiManager.getSubData("home.settings.ui.open_tooltip.tooltips.add_tooltips.tooltips.selected"_id)->setName(tooltipsToAdd[i]);
					g_selectedCustomTooltip = i;
				}), false);
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Add Tooltips")).setId("home.settings.ui.open_tooltip.tooltips.add_tooltips"_id).setAction([](sub* sub) {
			static fs::path path{ std::getenv(xorstr_("appdata")) };
			path /= xorstr_(BRANDING_NAME);
			sub->addOption(boolOpt().setLeft(xorstr_("Get tooltips from file")).setBool(pullFromFile));
			if (pullFromFile) {
				sub->addOption(numOpt<int32_t>().setLeft(xorstr_("Number of tooltips")).setDesc(xorstr_("Leave the value on 0 if you wish to add all tooltips from 'tooltips.txt'")).setMinMax(0, 255).setStep(1).setNum(numOfTooltips));
				sub->addOption(regularOpt().setLeft(xorstr_("Push")).setAction([=] {
					ui::addTooltipsFromFile(path, numOfTooltips);
				}));
			}
			else {
				sub->addOption(subOpt().setLeft(xorstr_("Tooltips")).setId("home.settings.ui.open_tooltip.tooltips.add_tooltips.tooltips"_id));
				sub->addOption(regularOpt().setLeft(xorstr_("Push")).setAction([=] {
					for (auto& t : tooltipsToAdd) {
						ui::addTooltip(t);
					}
				}));
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("InvalidTooltip")).setId("home.settings.ui.open_tooltip.tooltips.selected"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Replace")).setAction([=] {
				inputText(sub, [](std::string str) {
					if (str.compare(ui::g_tooltips[g_selectedTooltip].data()))
						ui::g_tooltips[g_selectedTooltip] = str;
				}, INPUT_MAX, ui::g_tooltips[g_selectedTooltip].data());
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Remove")).setAction([=] {
				ui::g_tooltips.erase(ui::g_tooltips.begin() + g_selectedTooltip);
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Tooltips")).setId("home.settings.ui.open_tooltip.tooltips"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Add Tooltips")).setId("home.settings.ui.open_tooltip.tooltips.add_tooltips"_id));
			sub->addOption(breakOpt().setLeft(xorstr_("Tooltips")));
			for (size_t i{}; i != ui::g_tooltips.size(); ++i) {
				sub->addOption(subOpt().setLeft(ui::g_tooltips[i].data()).setId("home.settings.ui.open_tooltip.tooltips.selected"_id).setAction([=] {
					g_dxUiManager.getSubData("home.settings.ui.open_tooltip.tooltips.selected"_id)->setName(ui::g_tooltips[i].data());
					g_selectedTooltip = i;
				}), false);
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Open Tooltip")).setId("home.settings.ui.open_tooltip"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Colors")).setId("home.settings.ui.open_tooltip.colors"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Tooltips")).setId("home.settings.ui.open_tooltip.tooltips"_id));
			sub->addOption(regularOpt().setLeft(xorstr_("Value")).setRight(g_dxUiManager.m_tooltip).setAction([=] {
				inputText(sub, [](std::string str) {
					g_dxUiManager.m_tooltip = str;
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(g_dxUiManager.m_drawOpenTooltip));
			sub->addOption(regularOpt().setLeft(xorstr_("Refresh tooltip")).setAction([=] {
				g_dxUiManager.m_tooltip.clear();
				static_cast<regularOpt*>(sub->getOpt(0))->setRight(g_dxUiManager.m_tooltip);
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("InvalidImage")).setId("home.settings.ui.header.image.selected"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Set")).setAction([=] {
				g_dxUiManager.m_hasHeaderLoaded = false;
				std::thread t([] {
					g_renderer->m_header.clear();
					g_renderer->m_headerFrame = 0;
					if (g_filePath.extension().string() == ".gif") {
						g_renderer->m_header.clear();
						g_renderer->m_headerFrame = 0;
						g_renderer->m_header = g_renderer->createGifTexture(g_filePath);
					}
					else {
						g_renderer->m_header.clear();
						g_renderer->m_headerFrame = 0;
						g_renderer->m_header.push_back({ NULL, g_renderer->createTexture(g_filePath).first });
					}
				});
				t.detach();
				g_dxUiManager.m_hasHeaderLoaded = true;
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Image")).setId("home.settings.ui.header.image"_id).setAction([](sub* sub) {
			auto filePath = fs::path(std::getenv(xorstr_("appdata"))).append(xorstr_(BRANDING_NAME)).append(xorstr_("Headers"));
			if (fs::exists(filePath)) {
				sub->addOption(regularOpt().setLeft(xorstr_("Open folder")).setAction([filePath] {
					std::system(std::format("explorer.exe {}", filePath.string()).c_str());
				}));
				fs::directory_iterator dirIt{ filePath.string() };
				static std::string hdrName{};
				for (auto&& dirEntry : dirIt) {
					if (dirEntry.is_regular_file()) {
						auto path = dirEntry.path();
						if (path.has_filename()) {
							if (images::g_containsPicExtension(path)) {
								sub->addOption(subOpt().setLeft(path.filename().string()).setId("home.settings.ui.header.image.selected"_id).setAction([path] {
									g_filePath = path;
									g_selectedImage = g_filePath.filename().string();
									g_dxUiManager.getSubData("home.settings.ui.header.image.selected"_id)->setName(g_selectedImage);
								}), false);
							}
						}
					}
				}
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Rect")).setId("home.settings.ui.header.rect"_id).setAction([](sub* sub) {
			sub->addOption(colorOpt().setLeft(xorstr_("Color")).setColor(&g_dxUiManager.m_headerRectColor));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(g_dxUiManager.m_drawHeaderRect));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Text")).setId("home.settings.ui.header.text"_id).setAction([](sub* sub) {
			sub->addOption(colorOpt().setLeft(xorstr_("Color")).setColor(&g_dxUiManager.m_headerTextColor));
			sub->addOption(boolOpt().setLeft(xorstr_("Toggle")).setBool(g_dxUiManager.m_drawHeaderText));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Header")).setId("home.settings.ui.header"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Image")).setId("home.settings.ui.header.image"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Rect")).setId("home.settings.ui.header.rect"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Text")).setId("home.settings.ui.header.text"_id));
			sub->addOption(regularOpt().setLeft(xorstr_("Clear header")).setAction([=] {
				g_dxUiManager.m_hasHeaderLoaded = false;
				g_renderer->m_header.clear();
				g_renderer->m_headerFrame = 0;
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Positions")).setId("home.settings.ui.positions"_id).setAction([](sub* sub) {
			sub->addOption(numOpt<float>().setLeft(xorstr_("X")).setMinMax(0.f, 1.f).setStep(0.01f).setNum(g_dxUiManager.m_pos.x));
			sub->addOption(numOpt<float>().setLeft(xorstr_("Y")).setMinMax(0.f, 1.f).setStep(0.01f).setNum(g_dxUiManager.m_pos.y));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("UI")).setId("home.settings.ui"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Open Tooltip")).setId("home.settings.ui.open_tooltip"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Header")).setDesc(xorstr_("Set your header to the default or a custom header")).setId("home.settings.ui.header"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Positions")).setDesc(xorstr_("Change the position of the menu")).setId("home.settings.ui.positions"_id));
			sub->addOption(numOpt<float>().setLeft(xorstr_("Width")).setDesc(xorstr_("Changes the menu width")).setMinMax(0.01f, 1.f).setStep(0.01f).setNum(g_dxUiManager.m_width));
			sub->addOption(numOpt<float>().setLeft(xorstr_("Scale")).setDesc(xorstr_("Changes the menu scale (size, makes it bigger)")).setMinMax(0.5f, 5.f).setStep(0.1f).setNum(g_dxUiManager.m_scale));
			sub->addOption(numOpt<float>().setLeft(xorstr_("Scroll Speed")).setDesc(xorstr_("Changes the smoothness of the option scroller")).setMinMax(0.f, 3.f).setStep(0.1f).setNum(g_dxUiManager.m_scrollSpeed));
			sub->addOption(regularOpt().setLeft(xorstr_("Menu Spin")).setDesc(xorstr_("Does a barrel roll")).setAction([=] { g_fiberPool.queue([] { g_dxUiManager.m_doABarrelRoll = true; fiber::cur()->wait(2s); g_dxUiManager.m_doABarrelRoll = false; }); }));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Input")).setId("home.settings.input"_id).setAction([](sub* sub) {
			sub->addOption(numOpt<size_t>().setLeft(xorstr_("Open Delay")).setMinMax(0, 50).setStep(5).setNum(g_dxUiManager.m_openDelay));
			sub->addOption(numOpt<size_t>().setLeft(xorstr_("Back Delay")).setMinMax(0, 50).setStep(5).setNum(g_dxUiManager.m_backDelay));
			sub->addOption(numOpt<size_t>().setLeft(xorstr_("Enter Delay")).setMinMax(0, 50).setStep(5).setNum(g_dxUiManager.m_enterDelay));
			sub->addOption(numOpt<size_t>().setLeft(xorstr_("Vertical Delay")).setMinMax(0, 50).setStep(5).setNum(g_dxUiManager.m_verticalDelay));
			sub->addOption(numOpt<size_t>().setLeft(xorstr_("Horizontal Delay")).setMinMax(0, 50).setStep(5).setNum(g_dxUiManager.m_horizontalDelay));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Open")).setId("home.settings.sounds.open"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Sound ID:")).setRight(std::string(g_dxUiManager.m_sounds.open.id)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.open.id = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Sound Set:")).setRight(std::string(g_dxUiManager.m_sounds.open.set)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.open.set = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Enabled")).setBool(g_dxUiManager.m_sounds.open.enabled));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Close")).setId("home.settings.sounds.close"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Sound ID:")).setRight(std::string(g_dxUiManager.m_sounds.close.id)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.close.id = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Sound Set:")).setRight(std::string(g_dxUiManager.m_sounds.close.set)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.close.set = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Enabled")).setBool(g_dxUiManager.m_sounds.close.enabled));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Enter")).setId("home.settings.sounds.enter"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Sound ID:")).setRight(std::string(g_dxUiManager.m_sounds.enter.id)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.enter.id = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Sound Set:")).setRight(std::string(g_dxUiManager.m_sounds.open.set)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.enter.set = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Enabled")).setBool(g_dxUiManager.m_sounds.enter.enabled));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Back")).setId("home.settings.sounds.back"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Sound ID:")).setRight(std::string(g_dxUiManager.m_sounds.back.id)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.back.id = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Sound Set:")).setRight(std::string(g_dxUiManager.m_sounds.back.set)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.back.set = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Enabled")).setBool(g_dxUiManager.m_sounds.back.enabled));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Left")).setId("home.settings.sounds.left"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Sound ID:")).setRight(std::string(g_dxUiManager.m_sounds.left.id)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.left.id = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Sound Set:")).setRight(std::string(g_dxUiManager.m_sounds.left.set)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.left.set = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Enabled")).setBool(g_dxUiManager.m_sounds.left.enabled));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Right")).setId("home.settings.sounds.right"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Sound ID:")).setRight(std::string(g_dxUiManager.m_sounds.right.id)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.right.id = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Sound Set:")).setRight(std::string(g_dxUiManager.m_sounds.right.set)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.right.set = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Enabled")).setBool(g_dxUiManager.m_sounds.right.enabled));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Up")).setId("home.settings.sounds.up"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Sound ID:")).setRight(std::string(g_dxUiManager.m_sounds.up.id)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.up.id = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Sound Set:")).setRight(std::string(g_dxUiManager.m_sounds.up.set)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.up.set = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Enabled")).setBool(g_dxUiManager.m_sounds.up.enabled));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Down")).setId("home.settings.sounds.down"_id).setAction([](sub* sub) {
			sub->addOption(regularOpt().setLeft(xorstr_("Sound ID:")).setRight(std::string(g_dxUiManager.m_sounds.down.id)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.down.id = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(regularOpt().setLeft(xorstr_("Sound Set:")).setRight(std::string(g_dxUiManager.m_sounds.down.set)).setAction([=] {
				inputText(sub, [](std::string str) {
					if (!isNumber(str))
						g_dxUiManager.m_sounds.down.set = str.c_str();
					else
						g_dxUiManager.sendNotify(xorstr_("Sounds"), xorstr_("The value {} contains a number, the required input type is a string"), str);
				});
			}));
			sub->addOption(boolOpt().setLeft(xorstr_("Enabled")).setBool(g_dxUiManager.m_sounds.down.enabled));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Sounds")).setId("home.settings.sounds"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Open")).setId("home.settings.sounds.open"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Close")).setId("home.settings.sounds.close"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Enter")).setId("home.settings.sounds.enter"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Back")).setId("home.settings.sounds.back"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Left")).setId("home.settings.sounds.left"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Right")).setId("home.settings.sounds.right"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Up")).setId("home.settings.sounds.up"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Down")).setId("home.settings.sounds.down"_id));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Translations")).setId("home.settings.translations"_id).setAction([](sub* sub) {
			auto filePath = fs::path(std::getenv(xorstr_("appdata"))).append(xorstr_(BRANDING_NAME)).append("Translations");
			if (fs::exists(filePath)) {
				fs::directory_iterator dirIt{ filePath.string() };
				for (auto&& dirEntry : dirIt) {
					if (dirEntry.is_regular_file()) {
						auto path = dirEntry.path();
						if (path.has_filename()) {
							if (path.extension() == ".json") {
								sub->addOption(iconOpt().setLeft(path.stem().string()).setIcon(g_config.m_currentLanguage == g_filePath.stem().string() ? 'g' : '\0').setAction([=] {
									g_filePath = path;
									g_config.m_currentLanguage = g_filePath.stem().string();
									g_config.load(g_config.m_currentLanguage);
								}), false);
							}
						}
					}
				}
			}
		}));
		static std::pair<uint64_t, DiscordJoinRequest> request{};
		g_dxUiManager.addSub(sub().setName(xorstr_("InvalidRequest")).setId("home.settings.discord_rpc.join_requests.selected"_id).setAction([](sub* sub) {
			sub->addOption(vecOpt<char const*, int>().setLeft(xorstr_("Reply Type")).setArray(hashes::discordReplyType).setPos(hashes::discordReplyTypePos));
			sub->addOption(regularOpt().setLeft(xorstr_("Reply")).setAction([&] {
				g_discordRPC.respondToRequest(request, hashes::discordReplyTypePos);
			}));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Join Requests")).setId("home.settings.discord_rpc.join_requests"_id).setAction([](sub* sub) {
			for (auto& r : g_discordRPC.m_requests) {
				sub->addOption(subOpt().setLeft(std::format("Username: {} | ID: {}", r.second.username, r.second.userId)).setId("home.settings.discord_rpc.join_requests.selected"_id).setAction([&] {
					request = r;
					g_dxUiManager.getSubData("home.settings.discord_rpc.join_requests.selected"_id)->setName(r.second.username).setSelectedOpt(0);
				}));
			}
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Discord RPC")).setId("home.settings.discord_rpc"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("Join Requests")).setId("home.settings.discord_rpc.join_requests"_id));
			sub->addOption(boolOpt().setLeft(xorstr_("Join Button")).setBool(g_discordRPC.m_joinButton));
			sub->addOption(boolOpt().setLeft(xorstr_("Active")).setBool(g_discordRPC.m_active));
		}));
		g_dxUiManager.addSub(sub().setName(xorstr_("Settings")).setId("home.settings"_id).setAction([](sub* sub) {
			sub->addOption(subOpt().setLeft(xorstr_("UI")).setId("home.settings.ui"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Sounds")).setId("home.settings.sounds"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Input")).setId("home.settings.input"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Translations")).setId("home.settings.translations"_id));
			sub->addOption(subOpt().setLeft(xorstr_("Discord RPC")).setId("home.settings.discord_rpc"_id));
			sub->addOption(regularOpt().setLeft(xorstr_("Unload")).setDesc(xorstr_("Unloads the menu")).setAction([] { g_running = false; }));
			sub->addOption(regularOpt().setLeft(xorstr_("Exit")).setDesc(xorstr_("Exits the game")).setAction([] { exit(0); }));
		}));
	}
	void mainScript::tick() {
		g_config.load(std::string());
		//models::dataStoring::cacheModelTable();
		g_mainScript.initUi();
		ui::addTooltipsFromFile(fs::path(std::getenv(xorstr_("appdata"))).append(xorstr_(BRANDING_NAME"\\tooltips.txt")));
		while (true) {
			//Check for input
			g_dxUiManager.inputCheckHandler();
			//Handle the keypresses
			g_dxUiManager.keypressHandler();
			for (int i{ 2 }; i; --i) {
				//Open keys
				PAD::DISABLE_CONTROL_ACTION(i, 227, true);
				PAD::DISABLE_CONTROL_ACTION(i, 175, true);
				//Phone and other game keys
				if (g_dxUiManager.m_open) {
					PAD::DISABLE_CONTROL_ACTION(i, 27, true);
					PAD::SET_INPUT_EXCLUSIVE(i, static_cast<int>(eControllerInputs::INPUT_FRONTEND_ACCEPT));
					PAD::SET_INPUT_EXCLUSIVE(i, static_cast<int>(eControllerInputs::INPUT_FRONTEND_CANCEL));
					PAD::SET_INPUT_EXCLUSIVE(i, static_cast<int>(eControllerInputs::INPUT_FRONTEND_DOWN));
					PAD::SET_INPUT_EXCLUSIVE(i, static_cast<int>(eControllerInputs::INPUT_FRONTEND_UP));
					PAD::SET_INPUT_EXCLUSIVE(i, static_cast<int>(eControllerInputs::INPUT_FRONTEND_LEFT));
					PAD::SET_INPUT_EXCLUSIVE(i, static_cast<int>(eControllerInputs::INPUT_FRONTEND_RIGHT));
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_VEH_SELECT_NEXT_WEAPON), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_FRONTEND_RIGHT), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_FRONTEND_DOWN), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_FRONTEND_LEFT), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_ARREST), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_CONTEXT), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_MELEE_BLOCK), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_HUD_SPECIAL), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_VEH_CIN_CAM), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_VEH_HEADLIGHT), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_FRONTEND_RDOWN), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_CHARACTER_WHEEL), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_VEH_RADIO_WHEEL), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_FRONTEND_CANCEL), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_MELEE_ATTACK_LIGHT), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_MELEE_ATTACK_HEAVY), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_SELECT_CHARACTER_MICHAEL), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_SELECT_CHARACTER_FRANKLIN), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_SELECT_CHARACTER_TREVOR), true);
					PAD::DISABLE_CONTROL_ACTION(i, static_cast<int>(eControllerInputs::INPUT_SELECT_CHARACTER_MULTIPLAYER), true);
				}
			}
			if (g_dxUiManager.m_forceLockInput)
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
			fiber::cur()->wait();
		}
	}
}