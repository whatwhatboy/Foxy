#pragma once
#include <string>
#include <vector>

namespace hashes {
	inline int weaponHashesPos{};
	inline const char* weaponHashes[] = {
		"weapon_dagger", "weapon_bat", "weapon_bottle", "weapon_crowbar",
		"weapon_unarmed", "weapon_flashlight", "weapon_golfclub", "weapon_hammer",
		"weapon_hatchet", "weapon_knuckle", "weapon_knife", "weapon_machete",
		"weapon_switchblade", "weapon_nightstick", "weapon_wrench", "weapon_battleaxe",
		"weapon_poolcue", "weapon_stone_hatchet", "weapon_pistol", "weapon_pistol_mk2",
		"weapon_combatpistol", "weapon_appistol", "weapon_stungun", "weapon_pistol50",
		"weapon_snspistol", "weapon_snspistol_mk2", "weapon_heavypistol", "weapon_vintagepistol",
		"weapon_flaregun", "weapon_marksmanpistol", "weapon_revolver", "weapon_revolver_mk2",
		"weapon_doubleaction", "weapon_raypistol", "weapon_ceramicpistol", "weapon_navyrevolver",
		"weapon_microsmg", "weapon_smg", "weapon_smg_mk2", "weapon_assaultsmg",
		"weapon_combatpdw", "weapon_machinepistol", "weapon_minismg", "weapon_raycarbine",
		"weapon_pumpshotgun", "weapon_pumpshotgun_mk2", "weapon_sawnoffshotgun", "weapon_assaultshotgun",
		"weapon_bullpupshotgun", "weapon_musket", "weapon_heavyshotgun", "weapon_dbshotgun",
		"weapon_autoshotgun", "weapon_assaultrifle", "weapon_assaultrifle_mk2", "weapon_carbinerifle",
		"weapon_carbinerifle_mk2", "weapon_advancedrifle", "weapon_specialcarbine", "weapon_specialcarbine_mk2",
		"weapon_bullpuprifle", "weapon_bullpuprifle_mk2", "weapon_compactrifle", "weapon_mg",
		"weapon_combatmg", "weapon_combatmg_mk2", "weapon_gusenberg", "weapon_sniperrifle",
		"weapon_heavysniper", "weapon_heavysniper_mk2", "weapon_marksmanrifle", "weapon_marksmanrifle_mk2",
		"weapon_rpg", "weapon_grenadelauncher", "weapon_grenadelauncher_smoke", "weapon_minigun",
		"weapon_firework", "weapon_railgun", "weapon_hominglauncher", "weapon_compactlauncher",
		"weapon_rayminigun", "weapon_grenade", "weapon_bzgas", "weapon_smokegrenade",
		"weapon_flare", "weapon_molotov", "weapon_stickybomb", "weapon_proxmine",
		"weapon_snowball", "weapon_pipebomb", "weapon_ball", "weapon_petrolcan",
		"weapon_fireextinguisher", "weapon_parachute", "weapon_hazardcan", "weapon_militaryrifle",
		"weapon_combatshotgun", "weapon_gadgetpistol"
	};
	inline int RangeweaponHashesPos{};
	inline const char* RangeweaponHashes[] = { "weapon_pistol", "weapon_pistol_mk2",
		"weapon_combatpistol", "weapon_appistol", "weapon_stungun", "weapon_pistol50",
		"weapon_snspistol", "weapon_snspistol_mk2", "weapon_heavypistol", "weapon_vintagepistol",
		"weapon_flaregun", "weapon_marksmanpistol", "weapon_revolver", "weapon_revolver_mk2",
		"weapon_doubleaction", "weapon_raypistol", "weapon_ceramicpistol", "weapon_navyrevolver",
		"weapon_microsmg", "weapon_smg", "weapon_smg_mk2", "weapon_assaultsmg",
		"weapon_combatpdw", "weapon_machinepistol", "weapon_minismg", "weapon_raycarbine",
		"weapon_pumpshotgun", "weapon_pumpshotgun_mk2", "weapon_sawnoffshotgun", "weapon_assaultshotgun",
		"weapon_bullpupshotgun", "weapon_musket", "weapon_heavyshotgun", "weapon_dbshotgun",
		"weapon_autoshotgun", "weapon_assaultrifle", "weapon_assaultrifle_mk2", "weapon_carbinerifle",
		"weapon_carbinerifle_mk2", "weapon_advancedrifle", "weapon_specialcarbine", "weapon_specialcarbine_mk2",
		"weapon_bullpuprifle", "weapon_bullpuprifle_mk2", "weapon_compactrifle", "weapon_mg",
		"weapon_combatmg", "weapon_combatmg_mk2", "weapon_gusenberg", "weapon_sniperrifle",
		"weapon_heavysniper", "weapon_heavysniper_mk2", "weapon_marksmanrifle", "weapon_marksmanrifle_mk2",
		"weapon_rpg", "weapon_grenadelauncher", "weapon_grenadelauncher_smoke", "weapon_minigun",
		"weapon_firework", "weapon_railgun", "weapon_hominglauncher", "weapon_compactlauncher",
		"weapon_rayminigun", "weapon_grenade", "weapon_bzgas", "weapon_smokegrenade",
		"weapon_flare", "weapon_molotov", "weapon_stickybomb", "weapon_proxmine",
		"weapon_snowball", "weapon_pipebomb", "weapon_ball", "weapon_hazardcan", "weapon_militaryrifle",
		"weapon_combatshotgun", "weapon_gadgetpistol"
	};

	inline int AmmoNamesHashesPos{};
	inline const char* AmmoNames[] =
	{
		"PICKUP_AMMO_FIREWORK", "PICKUP_AMMO_FLAREGUN", "PICKUP_AMMO_MINIGUN", "PICKUP_AMMO_RPG",
		"PICKUP_AMMO_GRENADELAUNCHER", "PICKUP_AMMO_SNIPER", "PICKUP_AMMO_SHOTGUN", "PICKUP_AMMO_MG",
		"PICKUP_AMMO_RIFLE", "PICKUP_AMMO_SMG", "PICKUP_AMMO_PISTOL",
	};

	inline int sessionTypesPos{};
	inline const char* sessionTypes[] = {
		"Join a public session", //0
		"Start a solo public session", //1
		"Start a closed crew session", //2
		"Start a solo crew session", //3
		"Start a closed friend session", //4 | 6
		"Find friends in session", //5 | 9
		"Start a solo session", //6 | 10
		"Start a invite-only session", //7 | 11
		"Find crew memebers in session", //8 | 12
		"Start a SCTV session", //8 | 13
		"Leave online", //9 | -1
		"Bail from online" //10
	};
	inline int ridJoinTypesPos{};
	inline const char* ridJoinTypes[] = {
		"Ninja Method", //0
		"Presence Method", //1
		"Friend Method", //2
		"Invite Method", //3
	};
	inline int regionTypesPos{};
	inline const char* regionTypes[] = {
		"US-East", //0 | 2
		"US-West", //1 | 6
		"Europe", //2 | 3
		"Australia", //3 | 5
		"China", //4 | 4
		"Japan", //5 | 7
		"CIS", //6 | 0
	};
	inline int languageTypesPos{};
	inline const char* languageTypes[] = {
		"English", //0 | 0
		"French", //1 | 1
		"German", //2 | 2
		"Spanish (Mexico)", //3 | 11
		"Spanish (Spain)", //4 | 4
		"Chinese (Traditional)", //5 | 9
		"Chinese (Simplified)", //6 | 12
		"Portuguese (Brazil)", //7 | 5
		"Japanese", //8 | 10
		"Russian", //9 | 7
		"Korean", //10 | 8
		"Polish", //11 | 6
		"Italian", //12 | 3
	};
	inline char const* dropTypes[] = {
		"Money",
		"Pickup"
	};
	inline char const* hashPresets[] = {
		"vw_prop_vw_colle_alien",
		"vw_prop_vw_colle_imporage",
		"vw_prop_vw_colle_beast",
		"vw_prop_vw_colle_pogo",
		"vw_prop_vw_colle_prbubble",
		"vw_prop_vw_colle_rsrcomm",
		"vw_prop_vw_colle_rsrgeneric",
		"vw_prop_vw_colle_sasquatch",
		"vw_prop_vw_lux_card_01a",
	};
	inline int discordReplyTypePos{};
	inline char const* discordReplyType[] = {
		"Yes",
		"No",
		"Ignore Request"
	};
	inline int netMessagesPos{};
	inline char const* netMessages[] = {
		"Breakup",
		"Slow Inc.",
		"PTSD",
		"Pipebomb"
	};
	inline int visionsPos{};
	inline char const* visions[] = {
		"chop",
		"water_silverlake",
		"whitenightlighting",
		"whiteout",
		"camera_bw",
		"camera_secuirity",
		"camera_secuirity_fuzz",
		"canyon_mission",
		"cops",
		"damage",
		"bank_hlwd",
		"mp_death_grade",
		"drug_drive_blend01",
		"drug_drive_blend02",
		"drug_flying_01",
		"prologue",
		"prologue_reflection_opt",
		"prologue_shootout",
		"prologue_shootout_opt",
		"cinema",
		"cinema_001",
		"pulse",
		"raceturbodark",
		"raceturboflash",
		"raceturbolight",
		"ranch",
		"redmist",
		"redmist_blend",
		"ufo",
		"ufo_deathray",
		"underwater",
		"tunnel",
		"barry1_stoned",
		"barryfadeout",
		"bloomlight",
		"buildingtop",
		"bullettimedark",
		"bullettimelight"
	};
}


namespace Ammunation {
	inline int weaponNames_pos{};
	inline char const* weapons_names[] = {
	"dagger",
	"bat",
	"bottle",
	"crowbar",
	"unarmed",
	"flashlight",
	"golfclub",
	"hammer",
	"hatchet",
	"knuckle",
	"knife",
	"machete",
	"switchblade",
	"nightstick",
	"wrench",
	"battleaxe",
	"poolcue",
	"stone_hatchet",
	"pistol",
	"pistol_mk2",
	"combatpistol",
	"appistol",
	"stungun",
	"pistol50",
	"snspistol",
	"snspistol_mk2",
	"heavypistol",
	"vintagepistol",
	"flaregun",
	"marksmanpistol",
	"revolver",
	"revolver_mk2",
	"doubleaction",
	"raypistol",
	"ceramicpistol",
	"navyrevolver",
	"microsmg",
	"smg",
	"smg_mk2",
	"assaultsmg",
	"combatpdw",
	"machinepistol",
	"minismg",
	"raycarbine",
	"pumpshotgun",
	"pumpshotgun_mk2",
	"sawnoffshotgun",
	"assaultshotgun",
	"bullpupshotgun",
	"musket",
	"heavyshotgun",
	"dbshotgun",
	"autoshotgun",
	"assaultrifle",
	"assaultrifle_mk2",
	"carbinerifle",
	"carbinerifle_mk2",
	"advancedrifle",
	"specialcarbine",
	"specialcarbine_mk2",
	"bullpuprifle",
	"bullpuprifle_mk2",
	"compactrifle",
	"mg",
	"combatmg",
	"combatmg_mk2",
	"gusenberg",
	"sniperrifle",
	"heavysniper",
	"heavysniper_mk2",
	"marksmanrifle",
	"marksmanrifle_mk2",
	"rpg",
	"grenadelauncher",
	"grenadelauncher_smoke",
	"minigun",
	"firework",
	"railgun",
	"hominglauncher",
	"compactlauncher",
	"rayminigun",
	"grenade",
	"bzgas",
	"smokegrenade",
	"flare",
	"molotov",
	"stickybomb",
	"proxmine",
	"snowball",
	"pipebomb",
	"ball",
	"petrolcan",
	"fireextinguisher",
	"parachute",
	"hazardcan",
	"militaryrifle",
	"combatshotgun",
	"gadgetpistol"
	};

	inline const char* weapons_names_backend[] =
	{
		"weapon_dagger", "weapon_bat", "weapon_bottle", "weapon_crowbar",
		"weapon_unarmed", "weapon_flashlight", "weapon_golfclub", "weapon_hammer",
		"weapon_hatchet", "weapon_knuckle", "weapon_knife", "weapon_machete",
		"weapon_switchblade", "weapon_nightstick", "weapon_wrench", "weapon_battleaxe",
		"weapon_poolcue", "weapon_stone_hatchet", "weapon_pistol", "weapon_pistol_mk2",
		"weapon_combatpistol", "weapon_appistol", "weapon_stungun", "weapon_pistol50",
		"weapon_snspistol", "weapon_snspistol_mk2", "weapon_heavypistol", "weapon_vintagepistol",
		"weapon_flaregun", "weapon_marksmanpistol", "weapon_revolver", "weapon_revolver_mk2",
		"weapon_doubleaction", "weapon_raypistol", "weapon_ceramicpistol", "weapon_navyrevolver",
		"weapon_microsmg", "weapon_smg", "weapon_smg_mk2", "weapon_assaultsmg",
		"weapon_combatpdw", "weapon_machinepistol", "weapon_minismg", "weapon_raycarbine",
		"weapon_pumpshotgun", "weapon_pumpshotgun_mk2", "weapon_sawnoffshotgun", "weapon_assaultshotgun",
		"weapon_bullpupshotgun", "weapon_musket", "weapon_heavyshotgun", "weapon_dbshotgun",
		"weapon_autoshotgun", "weapon_assaultrifle", "weapon_assaultrifle_mk2", "weapon_carbinerifle",
		"weapon_carbinerifle_mk2", "weapon_advancedrifle", "weapon_specialcarbine", "weapon_specialcarbine_mk2",
		"weapon_bullpuprifle", "weapon_bullpuprifle_mk2", "weapon_compactrifle", "weapon_mg",
		"weapon_combatmg", "weapon_combatmg_mk2", "weapon_gusenberg", "weapon_sniperrifle",
		"weapon_heavysniper", "weapon_heavysniper_mk2", "weapon_marksmanrifle", "weapon_marksmanrifle_mk2",
		"weapon_rpg", "weapon_grenadelauncher", "weapon_grenadelauncher_smoke", "weapon_minigun",
		"weapon_firework", "weapon_railgun", "weapon_hominglauncher", "weapon_compactlauncher",
		"weapon_rayminigun", "weapon_grenade", "weapon_bzgas", "weapon_smokegrenade",
		"weapon_flare", "weapon_molotov", "weapon_stickybomb", "weapon_proxmine",
		"weapon_snowball", "weapon_pipebomb", "weapon_ball", "weapon_petrolcan",
		"weapon_fireextinguisher", "weapon_parachute", "weapon_hazardcan", "weapon_militaryrifle",
		"weapon_combatshotgun", "weapon_gadgetpistol"
	};


	inline int assaultsmg_pos{};
	inline char const* assaultsmg[] = {
	"Normal Magazine",
	"Extended Magazine",
	"Scope",
	"Flash Light",
	"Suppressor",
	"Varmod Camo",

	};

	inline char const* assaultsmg_backend[] = {
	"COMPONENT_ASSAULTSMG_CLIP_01",
	"COMPONENT_ASSAULTSMG_CLIP_02",
	"component_at_scope_macro",
	"COMPONENT_AT_AR_FLSH",
	"COMPONENT_AT_AR_SUPP_02",
	"COMPONENT_ASSAULTSMG_VARMOD_LOWRIDER",
	};



	inline int SMGMKIIAmmo_pos{};
	inline char const* SMGMKIIAmmo[] = {
	"Normal Magazine",
	"Extended Magazine",
	"Tracer",
	"Incendiary",
	"Hollow point",
	"FMJ",

	};

	inline char const* SMGMKIIAmmo_backend[] = {
	"COMPONENT_SMG_MK2_CLIP_01",
	"COMPONENT_SMG_MK2_CLIP_02",
	"COMPONENT_SMG_MK2_CLIP_TRACER",
	"COMPONENT_SMG_MK2_CLIP_INCENDIARY",
	"COMPONENT_SMG_MK2_CLIP_HOLLOWPOINT",
	"COMPONENT_SMG_MK2_CLIP_FMJ",
	};

	inline int SMGMKIIAttach_pos{};
	inline char const* SMGMKIIAttach[] = {
	"Flash Light",
	"Suppressor",
	"Holographic Sight",
	"Small Scope",
	"Medium Scope",
	};



	inline char const* SMGMKIIAttach_backend[] = {
	"COMPONENT_AT_AR_FLSH",
	"COMPONENT_AT_PI_SUPP",
	"COMPONENT_AT_SIGHTS_SMG",
	"COMPONENT_AT_SCOPE_MACRO_02_SMG_MK2",
	"COMPONENT_AT_SCOPE_SMALL_SMG_MK2",
	};

	inline int SMGMKIIMuzzle_pos{};
	inline char const* SMGMKIIMuzzle[] = {
	"Tactical Muzzle Brake",
	"Fat-End Muzzle Brake",
	"Precision Muzzle Brake",
	"Heavy Duty Muzzle Brake",
	"Slanted Muzzle Brake",
	"Slanted Muzzle Brake",
	"Split-End Muzzle Brake",
	"Normal Barrel",
	"Heavy Barrel",
	};


	inline char const* SMGMKIIMuzzle_backend[] = {
	"COMPONENT_AT_MUZZLE_01",
	"COMPONENT_AT_MUZZLE_02",
	"COMPONENT_AT_MUZZLE_03",
	"COMPONENT_AT_MUZZLE_04",
	"COMPONENT_AT_MUZZLE_05",
	"COMPONENT_AT_MUZZLE_06",
	"COMPONENT_AT_MUZZLE_07",
	"COMPONENT_AT_SB_BARREL_01",
	"COMPONENT_AT_SB_BARREL_02",
	};

	inline int SMGMKIICamo_pos{};
	inline char const* SMGMKIICamo[] = {
	"Digital Camo",
	"Brushstroke Camo",
	"Woodland Camo",
	"Skull",
	"Sessanta Nove",
	"Perseus",
	"Leopard",
	"Zebra",
	"Geometric",
	"Boom",
	"Patriotic",
	};


	inline char const* SMGMKIICamo_backend[] = {
	"COMPONENT_SMG_MK2_MK2_CAMO",
	"COMPONENT_SMG_MK2_MK2_CAMO_02",
	"COMPONENT_SMG_MK2_MK2_CAMO_03",
	"COMPONENT_SMG_MK2_MK2_CAMO_04",
	"COMPONENT_SMG_MK2_MK2_CAMO_05",
	"COMPONENT_SMG_MK2_MK2_CAMO_06",
	"COMPONENT_SMG_MK2_MK2_CAMO_07",
	"COMPONENT_SMG_MK2_MK2_CAMO_08",
	"COMPONENT_SMG_MK2_MK2_CAMO_09",
	"COMPONENT_SMG_MK2_MK2_CAMO_10",
	"COMPONENT_SMG_MK2_MK2_CAMO_IND_01",
	};




	inline int smg_pos{};
	inline char const* smg[] = {
	"Normal Magazine",
	"Extended Magazine",
	"Drum Magazine",
	"Scope",
	"Flash Light",
	"Suppressor",
	"Varmod Camo",

	};

	inline char const* smg_backend[] = {
	"component_microsmg_clip_01",
	"component_microsmg_clip_02",
	"component_smg_clip_03",
	"component_at_scope_macro",
	"COMPONENT_AT_AR_FLSH",
	"component_at_pi_supp",
	"component_smg_varmod_luxe",
	};


	inline int microsmg_pos{};
	inline char const* microsmg[] = {
	"Normal Magazine",
	"Extended Magazine",
	"Scope",
	"Flash Light",
	"Suppressor",
	"Varmod Camo",

	};

	inline char const* microsmg_backend[] = {
	"component_microsmg_clip_01",
	"component_microsmg_clip_02",
	"component_at_scope_macro",
	"component_at_pi_flsh",
	"component_at_ar_supp_02",
	"COMPONENT_MICROSMG_VARMOD_LUXE",
	};


	inline int combatpw_pos{};
	inline char const* combatpw[] = {
	"Normal Magazine",
	"Extended Magazine",
	"Drum Magazine",
	"Scope",
	"Flash Light",
	"Hand Grip",

	};

	inline char const* combatpw_backend[] = {
	"component_combatpdw_clip_01",
	"component_combatpdw_clip_02",
	"component_combatpdw_clip_03",
	"COMPONENT_AT_SCOPE_SMALL",
	"COMPONENT_AT_AR_FLSH",
	"COMPONENT_AT_AR_AFGRIP",
	};


	inline int revolverMKIIAmmo_pos{};
	inline char const* revolverMKIIAmmo[] = {
	"Normal Magazine",
	"Tracer",
	"Incendiary",
	"Hollow point",
	"FMJ",

	};

	inline char const* revolverMKIIAmmo_backend[] = {
	"COMPONENT_REVOLVER_MK2_CLIP_01",
	"COMPONENT_REVOLVER_MK2_CLIP_TRACER",
	"COMPONENT_REVOLVER_MK2_CLIP_INCENDIARY",
	"COMPONENT_REVOLVER_MK2_CLIP_HOLLOWPOINT",
	"COMPONENT_REVOLVER_MK2_CLIP_FMJ",
	};




	inline int revolverMKIIAttach_pos{};
	inline char const* revolverMKIIAttach[] = {
	"Holographic Sight",
	"Flash Light",
	"Small Scope",
	"Compensator",
	};

	inline char const* revolverMKIIAttach_backend[] = {
	"COMPONENT_AT_SIGHTS",
	"COMPONENT_AT_PI_FLSH",
	"COMPONENT_AT_SCOPE_MACRO_MK2",
	"COMPONENT_AT_PI_COMP_03",
	};


	inline int revolverMKIICamo_pos{};
	inline char const* revolverMKIICamo[] = {
	"Digital Camo",
	"Brushstroke Camo",
	"Woodland Camo",
	"Skull",
	"Sessanta Nove",
	"Perseus",
	"Leopard",
	"Zebra",
	"Geometric",
	"Boom",
	"Patriotic",
	};

	inline char const* revolverMKIICamo_backend[] = {
	"COMPONENT_REVOLVER_MK2_MK2_CAMO",
	"COMPONENT_REVOLVER_MK2_MK2_CAMO_02",
	"COMPONENT_REVOLVER_MK2_MK2_CAMO_03",
	"COMPONENT_REVOLVER_MK2_MK2_CAMO_04",
	"COMPONENT_REVOLVER_MK2_MK2_CAMO_05",
	"COMPONENT_REVOLVER_MK2_MK2_CAMO_06",
	"COMPONENT_REVOLVER_MK2_MK2_CAMO_07",
	"COMPONENT_REVOLVER_MK2_MK2_CAMO_08",
	"COMPONENT_REVOLVER_MK2_MK2_CAMO_09",
	"COMPONENT_REVOLVER_MK2_MK2_CAMO_10",
	"COMPONENT_REVOLVER_MK2_MK2_CAMO_IND_01",
	};


	inline int revolverAttach_pos{};
	inline char const* revolverAttach[] = {
	"Varmod Goon",
	"Varmod Boss",
	};

	inline char const* revolverAttach_backend[] = {
	"COMPONENT_REVOLVER_VARMOD_GOON",
	"COMPONENT_REVOLVER_VARMOD_BOSS",
	};


	inline int SNSMKIIpistolAmmo_pos{};
	inline char const* SNSMKIIpistolAmmo[] = {
	"Normal Magazine",
	"Extended Magazine",
	"Tracer",
	"Incendiary",
	"Hollow point",
	"FMJ",

	};

	inline char const* SNSMKIIpistolAmmo_backend[] = {
	"COMPONENT_SNSPISTOL_MK2_CLIP_01",
	"COMPONENT_SNSPISTOL_MK2_CLIP_02",
	"COMPONENT_SNSPISTOL_MK2_CLIP_TRACER",
	"COMPONENT_SNSPISTOL_MK2_CLIP_INCENDIARY",
	"COMPONENT_SNSPISTOL_MK2_CLIP_HOLLOWPOINT",
	"COMPONENT_SNSPISTOL_MK2_CLIP_FMJ",
	};


	inline int SNSMKIIpistolAttach_pos{};
	inline char const* SNSMKIIpistolAttach[] = {
	"Scope",
	"Flashlight",
	"Suppressor",
	"Compensator",
	};

	inline char const* SNSMKIIpistolAttach_backend[] = {
	"COMPONENT_AT_PI_RAIL_02",
	"COMPONENT_AT_PI_FLSH_03",
	"COMPONENT_AT_PI_SUPP_02",
	"COMPONENT_AT_PI_COMP_02",
	};



	inline int SNSMKIIpistoCamo_pos{};
	inline char const* SNSMKIIpistoCamo[] = {
	"Digital Camo",
	"Brushstroke Camo",
	"Woodland Camo",
	"Skull",
	"Sessanta Nove",
	"Perseus",
	"Leopard",
	"Zebra",
	"Geometric",
	"Boom",
	"Patriotic",
	};

	inline char const* SNSMKIIpistoCamo_backend[] = {
	"COMPONENT_SNSPISTOL_MK2_CAMO",
	"COMPONENT_SNSPISTOL_MK2_CAMO_02",
	"COMPONENT_SNSPISTOL_MK2_CAMO_03",
	"COMPONENT_SNSPISTOL_MK2_CAMO_04",
	"COMPONENT_SNSPISTOL_MK2_CAMO_05",
	"COMPONENT_SNSPISTOL_MK2_CAMO_06",
	"COMPONENT_SNSPISTOL_MK2_CAMO_07",
	"COMPONENT_SNSPISTOL_MK2_CAMO_08",
	"COMPONENT_SNSPISTOL_MK2_CAMO_09",
	"COMPONENT_SNSPISTOL_MK2_CAMO_10",
	"COMPONENT_SNSPISTOL_MK2_CAMO_IND_01",
	};


	inline int Heavypistol_pos{};
	inline char const* Heavypistol[] = {
	"Extended Magazine",
	"Flashlight",
	"Suppressor",
	"Varmod Camo",
	};

	inline char const* Heavypistol_backend[] = {
	"COMPONENT_HEAVYPISTOL_CLIP_02",
	"COMPONENT_AT_PI_FLSH",
	"COMPONENT_AT_PI_SUPP",
	"COMPONENT_HEAVYPISTOL_VARMOD_LUXE",
	};


	inline int Callpistol_pos{};
	inline char const* Callpistol[] = {
	"Extended Magazine",
	"Flashlight",
	"Suppressor",
	"Varmod Camo",
	};

	inline char const* Callpistol_backend[] = {
	"COMPONENT_PISTOL50_CLIP_02",
	"COMPONENT_AT_PI_FLSH",
	"COMPONENT_AT_PI_SUPP_02",
	"COMPONENT_PISTOL50_VARMOD_LUXE",
	};



	inline int Apppistol_pos{};
	inline char const* Apppistol[] = {
	"Extended Magazine",
	"Flashlight",
	"Suppressor",
	"Varmod Camo",
	};

	inline char const* Apppistol_backend[] = {
	"COMPONENT_APPISTOL_CLIP_02",
	"COMPONENT_AT_PI_FLSH",
	"COMPONENT_AT_PI_SUPP_02",
	"COMPONENT_APPISTOL_VARMOD_LUXE",
	};

	inline int combatpistol_pos{};
	inline char const* combatpistol[] = {
	"Extended Magazine",
	"Flashlight",
	"Suppressor",
	"Varmod Camo",
	};

	inline char const* combatpistol_backend[] = {
	"COMPONENT_COMBATPISTOL_CLIP_02",
	"COMPONENT_AT_PI_FLSH",
	"COMPONENT_AT_PI_SUPP_02",
	"COMPONENT_COMBATPISTOL_VARMOD_LOWRIDER",
	};


	inline int Pistol_pos{};
	inline char const* Pistol[] = {
	"Extended Magazine",
	"Flashlight",
	"Suppressor",
	"Varmod Camo",
	};

	inline char const* Pistol_backend[] = {
	"COMPONENT_PISTOL_CLIP_02",
	"COMPONENT_AT_PI_FLSH",
	"COMPONENT_AT_PI_SUPP_02",
	"COMPONENT_PISTOL_VARMOD_LUXE",
	};




	inline int SwitchBlade_pos{};
	inline char const* SwitchBlade[] = {
	"Basic Handle",
	"VIP Handle",
	"Body Guard Handle",
	};

	inline char const* SwitchBlade_backend[] = {
	"COMPONENT_SWITCHBLADE_VARMOD_BASE",
	"COMPONENT_SWITCHBLADE_VARMOD_VAR1",
	"COMPONENT_SWITCHBLADE_VARMOD_VAR2",
	};



	inline int KnuckleDuster_pos{};
	inline char const* KnuckleDuster[] = {
	"Varmod Ballas",
	"Varmod Base",
	"Varmod Diamond",
	"Varmod Dollar",
	"Varmod Hate",
	"Varmod King",
	"Varmod Love",
	"Varmod Pimp",
	"Varmod Player",
	"Varmod Vagos",
	};

	inline char const* KnuckleDuster_backend[] = {
	"COMPONENT_KNUCKLE_VARMOD_BALLAS",
	"COMPONENT_KNUCKLE_VARMOD_BASE",
	"COMPONENT_KNUCKLE_VARMOD_DIAMOND",
	"COMPONENT_KNUCKLE_VARMOD_DOLLAR",
	"COMPONENT_KNUCKLE_VARMOD_HATE",
	"COMPONENT_KNUCKLE_VARMOD_KING",
	"COMPONENT_KNUCKLE_VARMOD_LOVE",
	"COMPONENT_KNUCKLE_VARMOD_PIMP",
	"COMPONENT_KNUCKLE_VARMOD_PLAYER",
	"COMPONENT_KNUCKLE_VARMOD_VAGOS",
	};




	inline int BullUpV2_Rifle_pos{};
	inline char const* BullUpV2_Rifle[] = {
		"Extended Magazine",
		"Varmod Camo",
		"Hand Grip",
		"Scope",
		"Flash Light",
		"Suppressor",
	};

	inline char const* BullUpV2_Rifle_backend[] = {
		"component_bullpuprifle_clip_02",
		"component_bullpuprifle_varmod_low",
		"COMPONENT_AT_AR_AFGRIP",
		"COMPONENT_AT_SCOPE_SMALL",
		"COMPONENT_AT_AR_FLSH",
		"component_at_ar_supp",
	};

	inline int Military_Rifle_pos{};
	inline char const* Military_Rifle[] = {
		"Extended Magazine",
		"Normal Magazine",
		"Sight",
		"Scope",
		"Suppressor",
	};

	inline char const* Military_Rifle_backend[] = {
		"COMPONENT_MILITARYRIFLE_CLIP_02",
		"COMPONENT_MILITARYRIFLE_CLIP_01",
		"COMPONENT_MILITARYRIFLE_SIGHT_01",
		"COMPONENT_AT_SCOPE_SMALL",
		"component_at_ar_supp",
	};




	inline int Special_Carbine_pos{};
	inline char const* Special_Carbine[] = {
		"Extended Magazine",
		"Drum Magazine",
		"Scope",
		"Varmod Camo"
		"Flash Light",
		"Suppressor",
		"Grip",
	};

	inline char const* Special_Carbine_backend[] = {
		"component_specialcarbine_clip_02",
		"component_specialcarbine_clip_03",
		"component_at_scope_medium",
		"component_specialcarbine_varmod_lowrider",
		"component_at_ar_flsh",
		"component_at_ar_supp",
		"component_at_ar_afgrip",
	};




	inline int Advanced_Rifle_pos{};
	inline char const* Advanced_Rifle[] = {
		"Extended Magazine",
		"Scope",
		"Varmod Camo"
		"Flash Light",
		"Suppressor",
	};

	inline char const* Advanced_Rifle_backend[] = {
		"component_advancedrifle_clip_02",
		"COMPONENT_AT_SCOPE_SMALL",
		"component_advancedrifle_varmod_luxe",
		"component_at_ar_flsh",
		"component_at_ar_supp",
	};


	inline int Carbine_Rifle_pos{};
	inline char const* Carbine_Rifle[] = {
		"Extended Magazine",
		"Drum Magazine",
		"Scope",
		"Varmod Camo"
		"Flash Light",
		"Hand Grip",
		"Suppressor",
	};

	inline char const* Carbine_Rifle_backend[] = {
		"component_carbinerifle_clip_02",
		"component_carbinerifle_clip_03",
		"component_at_scope_medium",
		"component_carbinerifle_varmod_luxe",
		"component_at_ar_flsh",
		"component_at_ar_afgrip",
		"component_at_ar_supp",
	};


	inline int Assault_Rifle_pos{};
	inline char const* Assault_Rifle[] = {
		"Extended Magazine",
		"Drum Magazine",
		"Scope",
		"Varmod Camo"
		"Flash Light",
		"Hand Grip",
		"Suppressor",
	};

	inline char const* Assault_Rifle_backend[] = {
		"component_assaultrifle_clip_02",
		"component_assaultrifle_clip_03",
		"COMPONENT_AT_SCOPE_MACRO",
		"component_assaultrifle_varmod_luxe",
		"component_at_ar_flsh",
		"component_at_ar_afgrip",
		"component_at_ar_supp",
	};

	inline int Marks_Man_pos{};
	inline char const* Marks_Man[] = {
		"Extended Magazine",
		"Flash Light",
		"Hand Grip",
		"Suppressor",
	};

	inline char const* Marks_Man_backend[] = {
		"component_marksmanrifle_clip_02",
		"component_at_ar_flsh",
		"component_at_ar_afgrip",
		"component_at_ar_supp",
	};



	inline int Smoke_Launcher_pos{};
	inline char const* Smoke_Launcher[] = {
		"Scope",
		"Flash Light",
		"Hand Grip",
	};

	inline char const* Smoke_Launcher_backend[] = {
		"COMPONENT_AT_SCOPE_SMALL",
		"component_at_ar_flsh",
		"component_at_ar_afgrip",
	};

	inline int Ammos_pos{};
	inline const char* Ammos_Names[] =
	{
		"Firework", "Flare Gun", "Mini Gun", "RPG",
		"Grenade Launcher", "Sniper", "Shotgun", "Machine gun",
		"Assault Rifle", "SMG", "Pistol",
	};

	inline const char* Ammos_Names_backend[] =
	{
		"PICKUP_AMMO_FIREWORK", "PICKUP_AMMO_FLAREGUN", "PICKUP_AMMO_MINIGUN", "PICKUP_AMMO_RPG",
		"PICKUP_AMMO_GRENADELAUNCHER", "PICKUP_AMMO_SNIPER", "PICKUP_AMMO_SHOTGUN", "PICKUP_AMMO_MG",
		"PICKUP_AMMO_RIFLE", "PICKUP_AMMO_SMG", "PICKUP_AMMO_PISTOL",
	};

}


namespace PedModels {

	inline int Animals_pos{};
	inline char const* Animals[] = {
		"A_C_Humpback","A_C_Boar","A_C_Chickenhawk","A_C_Chimp","A_C_Chop","A_C_Cormorant","A_C_Cow","A_C_Coyote","A_C_Crow","A_C_Deer","A_C_Fish","A_C_Hen","A_C_Husky","A_C_MtLion","A_C_Pig","A_C_Pigeon","A_C_Rat","A_C_Retriever",
		"A_C_Rhesus","A_C_Rottweiler", "A_C_Pug","A_C_Poodle","A_C_Westy","A_C_Seagull","A_C_SharkTiger","A_C_Shepherd", "A_C_Cat_01", "A_C_Rabbit_01", "A_C_KillerWhale", "A_C_SharkHammer", "A_C_SharkTiger", "A_C_Dolphin", "A_C_Panther",
	};
	inline int Storycharacter_pos{};
	inline char const* Storycharacter[] = {
		"IG_TylerDix","IG_Wade","IG_Zimbor","IG_Abigail","IG_AmandaTownley","IG_Andreas","IG_Ashley","IG_BallasOG","IG_Bankman","IG_Barry","IG_BestMen","IG_Beverly","IG_Brad","IG_Bride","IG_Car3guy1","IG_Car3guy2","IG_Casey",
		"IG_Chef","IG_ChengSr","IG_ChrisFormage","IG_Clay","IG_ClayPain","IG_Cletus","IG_Dale","IG_DaveNorton","IG_Denise","IG_Devin","IG_Dom","IG_Dreyfuss","IG_DrFriedlander","IG_Fabien","IG_FBISuit_01","IG_Floyd","IG_Groom",
		"IG_Hao","IG_Hunter","IG_Janet","ig_JAY_Norris","IG_JewelAss","IG_JimmyBoston","IG_JimmyDiSanto","IG_JoeMinuteMan","ig_JohnnyKlebitz","IG_Josef","IG_Josh","IG_KerryMcIntosh","IG_LamarDavis","IG_Lazlow","IG_LesterCrest",
		"IG_LifeInvad_01","IG_LifeInvad_02","IG_Magenta","IG_Manuel","IG_Marnie","IG_MaryAnn","IG_Maude","IG_Michelle","IG_Milton","IG_Molly","IG_MRK","IG_MrsPhillips","IG_MRS_Thornhill","IG_Natalia","IG_NervousRon","IG_Nigel",
		"IG_Old_Man1A","IG_Old_Man2","IG_Omega","IG_ONeil","IG_Orleans","IG_Ortega","IG_Paper","IG_Patricia","IG_Priest","IG_ProlSec_02","IG_Ramp_Gang","IG_Ramp_Hic","IG_Ramp_Hipster","IG_Ramp_Mex","IG_RoccoPelosi","IG_RussianDrunk",
		"IG_Screen_Writer","IG_SiemonYetarian","IG_Solomon","IG_SteveHains","IG_Stretch","IG_Talina","IG_Tanisha","IG_TaoCheng","IG_TaosTranslator","ig_TennisCoach","IG_Terry","IG_TomEpsilon","IG_Tonya","IG_TracyDiSanto","IG_TrafficWarden",
		"CS_AmandaTownley","CS_Andreas", "cs_ashley","CS_Bankman","CS_Barry","CS_Beverly","CS_Brad","CS_BradCadaver","CS_Carbuyer","CS_Casey","CS_ChengSr","CS_ChrisFormage","CS_Clay","CS_Dale","CS_DaveNorton","cs_debra","cs_denise","CS_Devin","CS_Dom","CS_Dreyfuss",
		"CS_DrFriedlander","CS_Fabien","CS_FBISuit_01","CS_Floyd","CS_Guadalope","cs_gurk","CS_Hunter","CS_Janet","CS_JewelAss","CS_JimmyBoston","CS_JimmyDiSanto","CS_JoeMinuteMan","CS_JohnnyKlebitz","CS_Josef","CS_Josh","CS_LamarDavis",
		"CS_Lazlow","CS_LesterCrest","CS_LifeInvad_01","CS_Magenta","CS_Manuel","CS_Marnie","CS_MartinMadrazo","CS_MaryAnn","CS_Michelle","CS_Milton","CS_Molly","CS_MovPremF_01","CS_MovPremMale","CS_MRK","CS_MrsPhillips","CS_MRS_Thornhill",
		"CS_Natalia","CS_NervousRon","CS_Nigel","CS_Old_Man1A","CS_Old_Man2","CS_Omega","CS_Orleans","CS_Paper","CS_Patricia","CS_Priest","CS_ProlSec_02","CS_RussianDrunk","CS_SiemonYetarian","CS_Solomon","CS_SteveHains","CS_Stretch",
		"CS_Tanisha","CS_TaoCheng","CS_TaosTranslator","CS_TennisCoach","CS_Terry","CS_Tom","CS_TomEpsilon","CS_TracyDiSanto","CS_Wade","CS_Zimbor",
	};
	inline int Female_pos{};
	inline char const* Female[] = {
		"s_f_y_clubbar_02", "s_f_y_beachbarstaff_01", "s_f_y_casino_01","S_F_Y_Stripper_01","S_F_Y_AirHostess_01","S_F_Y_Bartender_01","S_F_Y_Baywatch_01","S_F_Y_Cop_01","S_F_Y_Factory_01","S_F_Y_Hooker_01","S_F_Y_Hooker_02","S_F_Y_Hooker_03","S_F_Y_Migrant_01","S_F_Y_MovPrem_01","S_F_Y_Ranger_01",
		"S_F_Y_Scrubs_01","S_F_Y_Sheriff_01","S_F_Y_Shop_LOW","S_F_Y_Shop_MID","S_F_Y_StripperLite","S_F_Y_Stripper_01","S_F_Y_Stripper_02","S_F_Y_SweatShop_01", "U_F_Y_BikerChic","U_F_Y_COMJane","U_F_Y_corpse_01","U_F_Y_corpse_02",
		"U_F_Y_HotPosh_01","U_F_Y_JewelAss_01","U_F_Y_Mistress","U_F_Y_PoppyMich","U_F_Y_Princess","U_F_Y_SpyActress","A_F_Y_Tennis_01","A_F_Y_Tourist_01","A_F_Y_Tourist_02","A_F_Y_Vinewood_01",
		"A_F_Y_Vinewood_02","A_F_Y_Vinewood_03","A_F_Y_Vinewood_04","A_F_Y_Yoga_01", "A_F_Y_BevHills_03","A_F_Y_BevHills_04","A_F_Y_Business_01","A_F_Y_Business_02","A_F_Y_Business_03","A_F_Y_Business_04","A_F_Y_EastSA_01","A_F_Y_EastSA_02","A_F_Y_EastSA_03","A_F_Y_Epsilon_01","A_F_Y_Fitness_01",
		"A_F_Y_Fitness_02","A_F_Y_GenHot_01","A_F_Y_Golfer_01","A_F_Y_Hiker_01","A_F_Y_Hippie_01","A_F_Y_Hipster_01","A_F_Y_Hipster_02","A_F_Y_Hipster_03","A_F_Y_Hipster_04","A_F_Y_Indian_01","A_F_Y_Juggalo_01","A_F_Y_Runner_01",
		"A_F_Y_RurMeth_01","A_F_Y_SCDressy_01","A_F_Y_Skater_01","A_F_Y_SouCent_01","A_F_Y_SouCent_02","A_F_Y_SouCent_03","A_F_Y_Beach_01","A_F_Y_BevHills_01","A_F_Y_BevHills_02", "A_F_M_Beach_01","A_F_M_BevHills_01","A_F_M_BevHills_02","A_F_M_BodyBuild_01","A_F_M_Business_02","A_F_M_Downtown_01","A_F_M_EastSA_01",
		"A_F_M_EastSA_02","A_F_M_FatBla_01","A_F_M_FatCult_01","A_F_M_FatWhite_01","A_F_M_KTown_01","A_F_M_KTown_02","A_F_M_ProlHost_01","A_F_M_Salton_01","A_F_M_SkidRow_01","A_F_M_SouCentMC_01","A_F_M_SouCent_01","A_F_M_SouCent_02",
		"A_F_M_Tourist_01","A_F_M_TrampBeac_01","A_F_M_Tramp_01","A_F_O_GenStreet_01","A_F_O_Indian_01","A_F_O_KTown_01","A_F_O_Salton_01","A_F_O_SouCent_01","A_F_O_SouCent_02","S_F_M_Fembarber","S_F_M_Maid_01","S_F_M_Shop_HIGH","G_F_Y_ballas_01","G_F_Y_Families_01","G_F_Y_Lost_01","G_F_Y_Vagos_01",
		"U_F_M_Corpse_01","U_F_M_Miranda","U_F_M_ProMourn_01","U_F_O_MovieStar","U_F_O_ProlHost_01","MP_F_Misty_01", "S_F_M_SweatShop_01", "MP_F_DeadHooker","MP_F_Freemode_01","MP_F_Misty_01","MP_F_StripperLite",
	};
	inline int Unclassed_pos{};
	inline char const* Unclassed[] = {
		 "U_M_Y_ImpoRage", "U_M_Y_Zombie_01", "S_M_Y_Swat_01", "S_M_Y_Pilot_01", "S_M_M_MovAlien_01", "S_M_M_HairDress_01","S_M_M_HighSec_01","S_M_M_HighSec_02","S_M_M_Janitor","S_M_M_LatHandy_01","S_M_M_LifeInvad_01","S_M_M_Linecook","S_M_M_LSMetro_01","S_M_M_Mariachi_01","S_M_M_Marine_01","S_M_M_Marine_02","S_M_M_Migrant_01",
		"S_M_M_MovAlien_01","S_M_M_MovPrem_01","S_M_M_MovSpace_01","S_M_M_Paramedic_01","S_M_M_Pilot_01","S_M_M_Pilot_02","S_M_M_Postal_01","S_M_M_Postal_02","S_M_M_PrisGuard_01","S_M_M_Scientist_01","S_M_M_Security_01","S_M_M_SnowCop_01",
		"S_M_M_StrPerf_01","S_M_M_StrPreach_01","S_M_M_StrVend_01","S_M_M_Trucker_01","S_M_M_UPS_01","S_M_M_UPS_02","S_M_O_Busker_01","S_M_Y_AirWorker","S_M_Y_AmmuCity_01","S_M_Y_ArmyMech_01","S_M_Y_Autopsy_01","S_M_Y_Barman_01","S_M_Y_BayWatch_01",
		"S_M_Y_BlackOps_01","S_M_Y_BlackOps_02","S_M_Y_BlackOps_03","S_M_Y_BusBoy_01","S_M_Y_Chef_01","S_M_Y_Clown_01","S_M_Y_Construct_01","S_M_Y_Construct_02","S_M_Y_Cop_01","S_M_Y_Dealer_01","S_M_Y_DevinSec_01","S_M_Y_DockWork_01","S_M_Y_Doorman_01",
		"S_M_Y_DWService_01","S_M_Y_DWService_02","S_M_Y_Factory_01","S_M_Y_Fireman_01","S_M_Y_Garbage","S_M_Y_Grip_01","S_M_Y_HwayCop_01","S_M_Y_Marine_01","S_M_Y_Marine_02","S_M_Y_Marine_03","S_M_Y_Mime","S_M_Y_PestCont_01",
		"S_M_Y_PrisMuscl_01","S_M_Y_Ranger_01","S_M_Y_Robber_01","S_M_Y_Sheriff_01","S_M_Y_Shop_MASK","S_M_Y_StrVend_01","S_M_Y_USCG_01","S_M_Y_Valet_01","S_M_Y_Waiter_01",
		"S_M_Y_WinClean_01","S_M_Y_XMech_01","S_M_Y_XMech_02","U_M_M_Aldinapoli","U_M_M_BankMan","U_M_M_BikeHire_01","U_M_M_FIBArchitect","U_M_M_FilmDirector","U_M_M_GlenStank_01",
		"U_M_M_Griff_01","U_M_M_Jesus_01","U_M_M_JewelSec_01","U_M_M_JewelThief","U_M_M_MarkFost","U_M_M_PartyTarget","U_M_M_ProlSec_01","U_M_M_ProMourn_01","U_M_M_RivalPap","U_M_M_SpyActor","U_M_M_WillyFist","U_M_O_FinGuru_01",
		"U_M_O_TapHillBilly","U_M_O_Tramp_01","U_M_Y_Abner","U_M_Y_AntonB",	"U_M_Y_BabyD","U_M_Y_Baygor","U_M_Y_BurgerDrug_01","U_M_Y_Chip","U_M_Y_Cyclist_01","U_M_Y_Guido_01","U_M_Y_GunVend_01","U_M_Y_Hippie_01",
		"U_M_Y_ImpoRage","U_M_Y_Justin","U_M_Y_Mani","U_M_Y_MilitaryBum","U_M_Y_Paparazzi","U_M_Y_Party_01","U_M_Y_Pogo_01","U_M_Y_Prisoner_01","U_M_Y_ProlDriver_01","U_M_Y_RSRanger_01","U_M_Y_SBike","U_M_Y_StagGrm_01","U_M_Y_Tattoo_01", "S_M_Y_Swat_01", "U_M_Y_FIBMugger_01", "U_M_M_FIBArchitect", "U_M_Y_MilitaryBum", "MP_M_FIBSec_01", "S_M_M_FIBOffice_01","S_M_M_FIBOffice_02", "U_M_Y_MilitaryBum", "S_M_M_Marine_01","S_M_M_Marine_02", "S_M_Y_Prisoner_01",
		"A_M_Y_BreakDance_01","A_M_Y_BusiCas_01","A_M_Y_Business_01","A_M_Y_Business_02","A_M_Y_Business_03", "A_M_Y_Golfer_01","A_M_Y_HasJew_01","A_M_Y_Hiker_01", "A_M_Y_Hippy_01","A_M_Y_Hipster_01","A_M_Y_Hipster_02",
		"A_M_Y_Hipster_03", "CSB_PrologueDriver","CSB_ProlSec","CSB_Ramp_gang","CSB_Ramp_hic","CSB_Ramp_hipster", "CSB_Ramp_mex","CSB_Reporter","CSB_RoccoPelosi","CSB_Screen_Writer","CSB_TrafficWarden","G_M_M_ArmBoss_01",
		"G_M_M_ArmGoon_01","G_M_M_ArmLieut_01","G_M_M_ChemWork_01","G_M_M_ChiBoss_01","G_M_M_ChiCold_01","G_M_M_ChiGoon_01","G_M_M_ChiGoon_02","G_M_M_KorBoss_01","G_M_M_MexBoss_01","G_M_M_MexBoss_02","G_M_Y_ArmGoon_02","G_M_Y_Azteca_01",
		"G_M_Y_BallaEast_01","G_M_Y_BallaOrig_01","G_M_Y_BallaSout_01","G_M_Y_FamCA_01","G_M_Y_FamDNF_01","G_M_Y_FamFor_01","G_M_Y_Korean_01","G_M_Y_Korean_02","G_M_Y_KorLieut_01","G_M_Y_Lost_01","G_M_Y_Lost_02","G_M_Y_Lost_03",
		"G_M_Y_MexGang_01","G_M_Y_MexGoon_01","G_M_Y_MexGoon_02","G_M_Y_MexGoon_03","G_M_Y_PoloGoon_01","G_M_Y_PoloGoon_02","G_M_Y_SalvaBoss_01","G_M_Y_SalvaGoon_01","G_M_Y_SalvaGoon_02","G_M_Y_SalvaGoon_03","G_M_Y_StrPunk_01","G_M_Y_StrPunk_02",
		"HC_Driver","HC_Gunman","HC_Hacker", "A_M_M_ACult_01","A_M_M_AfriAmer_01", "A_M_M_BevHills_01","A_M_M_BevHills_02","A_M_M_Business_01",
		"A_M_M_EastSA_01","A_M_M_EastSA_02","A_M_M_Farmer_01","A_M_M_FatLatin_01","A_M_M_GenFat_01","A_M_M_GenFat_02","A_M_M_Golfer_01","A_M_M_HasJew_01","A_M_M_Hillbilly_01","A_M_M_Hillbilly_02","A_M_M_Indian_01",
		"A_M_M_KTown_01","A_M_M_Malibu_01","A_M_M_MexCntry_01","A_M_M_MexLabor_01","A_M_M_OG_Boss_01","A_M_M_Paparazzi_01","A_M_M_Polynesian_01","A_M_M_ProlHost_01","A_M_M_RurMeth_01","A_M_M_Salton_01","A_M_M_Salton_02",
		"A_M_M_Salton_03","A_M_M_Salton_04","A_M_M_Skater_01","A_M_M_Skidrow_01","A_M_M_SoCenLat_01","A_M_M_SouCent_01","A_M_M_SouCent_02","A_M_M_SouCent_03","A_M_M_SouCent_04","A_M_M_StLat_02","A_M_M_Tennis_01",
		"A_M_M_Tourist_01","A_M_M_TrampBeac_01","A_M_M_Tramp_01","A_M_M_TranVest_01","A_M_M_TranVest_02","A_M_O_ACult_01","A_M_O_ACult_02","A_M_O_Beach_01","A_M_O_GenStreet_01","A_M_O_KTown_01","A_M_O_Salton_01",
		"A_M_O_SouCent_01","A_M_O_SouCent_02","A_M_O_SouCent_03","A_M_O_Tramp_01","A_M_Y_ACult_01","A_M_Y_ACult_02","A_M_Y_BeachVesp_01","A_M_Y_BeachVesp_02", "A_M_Y_BevHills_01",
		"A_M_Y_BevHills_02", "A_M_Y_Cyclist_01","A_M_Y_DHill_01","A_M_Y_Downtown_01","A_M_Y_EastSA_01","A_M_Y_EastSA_02","A_M_Y_Epsilon_01","A_M_Y_Epsilon_02","A_M_Y_Gay_01","A_M_Y_Gay_02", "A_M_Y_GenStreet_01","A_M_Y_GenStreet_02","A_M_Y_Indian_01","A_M_Y_Jetski_01","A_M_Y_Juggalo_01","A_M_Y_KTown_01","A_M_Y_KTown_02","A_M_Y_Latino_01","A_M_Y_MethHead_01","A_M_Y_MexThug_01","A_M_Y_MotoX_01","A_M_Y_MotoX_02", "CSB_Customer","CSB_Denise_friend",
		"CSB_FOS_rep","CSB_G","CSB_Groom","CSB_Grove_str_dlr","CSB_Hao","CSB_Hugh","CSB_Imran","CSB_Janitor","CSB_Maude","CSB_MWeather","CSB_Ortega","CSB_Oscar","MP_G_M_Pros_01","MP_HeadTargets","MP_M_Claude_01","MP_M_ExArmy_01","MP_M_FamDD_01","MP_M_FIBSec_01","MP_M_Freemode_01",
		"MP_M_Marston_01","MP_M_Niko_01","MP_M_ShopKeep_01","MP_S_M_Armoured_01","Player_One","Player_Two","Player_Zero",
		"S_M_M_AmmuCountry","S_M_M_Armoured_01","S_M_M_Armoured_02","S_M_M_AutoShop_01",
		"S_M_M_AutoShop_02","S_M_M_Bouncer_01","S_M_M_ChemSec_01","S_M_M_CIASec_01","S_M_M_CntryBar_01","S_M_M_DockWork_01","S_M_M_Doctor_01","S_M_M_Gaffer_01","S_M_M_Gardener_01","S_M_M_GenTransport"
	};
	inline int updatemodels_pos{};
	inline char const* updatemodels[] = {
		"S_M_M_RaceOrg_01", "A_M_Y_CarClub_01", "A_F_Y_CarClub_01", "S_F_M_RetailStaff_01", "G_M_M_Prisoners_01", "S_M_M_Tattoo_01", "G_M_M_Slasher_01", "IG_LilDee",
		"IG_DrugDealer", "CSB_DrugDealer", "A_M_Y_TattooCust_01", "IG_Moodyman_02", "CSB_Moodyman_02", "S_M_M_Autoshop_03", "S_F_M_Autoshop_01", "IG_Benny_02", "IG_AviSchwartzman_02",
		"CSB_AviSchwartzman_02", "IG_Sessanta", "CSB_Sessanta", "IG_Mimi", "CSB_Mimi", "IG_Hao_02", "CSB_Hao_02",
	};
}


namespace PedOptions {

	
	inline char const* scenarios_vars_frontend[] = {
	"Paparazzi",
	"Drug Dealer",
	"Drinking Coffee",
	"Playing Instruments",
	"Flexing",
	"Jogging",
	"Binoculars",
	"Clipboard",
	"Bench Press",
	"Chin Ups",
	"BBQ",
	"Superhero",
	"Fishing",
	"Security",
	"Leaf Blower",
	"Film Shocking Event",
	"Idle Cop",
	"Drinking",
	"Play Golf",
	"Welding",
	"Smoking Weed",
	"Hammering",
	"Play Tennis",
	"Jack Hammer",
	"Seat Armchair",
	"Seat Bar Chair",
	"Seat Bench",
	"Seat Sun Lounger",
	"Seat Chair",
	"Fixing Vehicle",
	"Yoga",
	"Medic Time Of Death",
	"Tourist Map",
	"Police Investigate",
	"Partying",
	"Night Woman High Class",
	"Night Woman Low Class",
	"Sleeping",
	"Stupor"
	};

	inline char const* scenarios_vars_backend[] = {
		"WORLD_HUMAN_PAPARAZZI",
		"WORLD_HUMAN_DRUG_DEALER_HARD",
		"WORLD_HUMAN_AA_COFFEE",
		"WORLD_HUMAN_MUSICIAN",
		"WORLD_HUMAN_MUSCLE_FLEX",
		"WORLD_HUMAN_JOG_STANDING",
		"WORLD_HUMAN_BINOCULARS",
		"WORLD_HUMAN_CLIPBOARD",
		"PROP_HUMAN_SEAT_MUSCLE_BENCH_PRESS",
		"PROP_HUMAN_MUSCLE_CHIN_UPS",
		"PROP_HUMAN_BBQ",
		"WORLD_HUMAN_SUPERHERO",
		"WORLD_HUMAN_STAND_FISHING",
		"WORLD_HUMAN_SECURITY_SHINE_TORCH",
		"WORLD_HUMAN_GARDENER_LEAF_BLOWER",
		"WORLD_HUMAN_MOBILE_FILM_SHOCKING",
		"WORLD_HUMAN_COP_IDLES",
		"WORLD_HUMAN_DRINKING",
		"WORLD_HUMAN_GOLF_PLAYER",
		"WORLD_HUMAN_WELDING",
		"WORLD_HUMAN_SMOKING_POT",
		"WORLD_HUMAN_HAMMERING",
		"WORLD_HUMAN_TENNIS_PLAYER",
		"WORLD_HUMAN_CONST_DRILL",
		"PROP_HUMAN_SEAT_ARMCHAIR",
		"PROP_HUMAN_SEAT_BAR",
		"PROP_HUMAN_SEAT_BENCH",
		"PROP_HUMAN_SEAT_SUNLOUNGER",
		"PROP_HUMAN_SEAT_CHAIR",
		"WORLD_HUMAN_VEHICLE_MECHANIC",
		"WORLD_HUMAN_YOGA",
		"CODE_HUMAN_MEDIC_TIME_OF_DEATH",
		"WORLD_HUMAN_TOURIST_MAP",
		"CODE_HUMAN_POLICE_INVESTIGATE",
		"WORLD_HUMAN_PARTYING",
		"WORLD_HUMAN_PROSTITUTE_HIGH_CLASS",
		"WORLD_HUMAN_PROSTITUTE_LOW_CLASS",
		"WORLD_HUMAN_BUM_SLUMPED",
		"WORLD_HUMAN_STUPOR",

	};


	inline char const* visionstyle_vars_frontend[] = {
		"Chop",
		"Silverlake Water",
		"Night Lighting",
		"White Out",
		"Camera BW",
		"Camera Security",
		"Camera Security Buzz",
		"Canyon Mission",
		"Police",
		"Damaged",
		"Bank HLWD",
		"Death",
		"Drugs 1",
		"Drugs 2",
		"Drugs 3",
		"Prologue",
		"Prologue Reflection",
		"Prolouge Shootout 1",
		"Prolouge Shootout 2",
		"Cinema",
		"cinema 001",
		"Pulse",
		"Race Turbo Dark",
		"Race Turbo Flash",
		"Race Turbo Light",
		"Ranch",
		"Red Mist",
		"Red Mist Blend",
		"UFO",
		"UFO Deathray",
		"Under Water",
		"Tunnel",
		"Stoned",
		"Faint",
		"Blooming Light",
		"Bag",
		"Building Top",
		"Shooting Black Out",
		"Shooting Shining Light",
	};
	inline char const* visiontyle_vars_backend[] = {
		"CHOP",
		"WATER_silverlake",
		"whitenightlighting",
		"WhiteOut",
		"CAMERA_BW",
		"CAMERA_secuirity",
		"CAMERA_secuirity_FUZZ",
		"canyon_mission",
		"Cops",
		"Damage",
		"Bank_HLWD",
		"MP_death_grade",
		"drug_drive_blend01",
		"drug_drive_blend02",
		"drug_flying_01",
		"Prologue",
		"prologue_reflection_opt",
		"prologue_shootout",
		"Prologue_shootout_opt",
		"Cinema",
		"cinema_001",
		"Pulse",
		"RaceTurboDark",
		"RaceTurboFlash",
		"RaceTurboLight",
		"Ranch",
		"REDMIST",
		"REDMIST_blend",
		"UFO",
		"ufo_deathray",
		"underwater",
		"Tunnel",
		"Barry1_Stoned",
		"BarryFadeOut",
		"BloomLight",
		"buildingTOP",
		"BulletTimeDark",
		"BulletTimeLight",

	};

	inline int walkstyle_pos{};
	inline int vision_pos{};
	inline int scenario_pos{};
	inline char const* walkstyle_vars_frontend[] = {
		"Generic Male",
		"Generic Female",
		"Police",
		"Gangster",
		"Hipster",
		"Hobo",
		"Fat (Imp User Moment)",
		"Lester the Molester",
		"Drunk",
		"Moderately Drunk",
		"Very Drunk",
		"Lady's of the night",
		"Alien",
		"Business",
		"Causal",
		"Clipboard",
		"Coward",
		"On fire",
		"Afraid",
		"Hiking",
		"Hurrying",
		"Injured",
		"Bully",
		"Muscle",
		"Quick",
		"Sad (Extra Depressed)",
		"Shady",
		"Shocked",
		"Arrogant",
		"Handbag",
		"Heels",
		"? (Unknown Action)",
		"Leaf Blower",
		"Tazered",
		"Tough Guy",
		"Bag",
	};
	inline char const* walkstyle_vars_backend[] = {
		"move_m@generic",
		"move_f@generic",
		"move_cop@action",
		"move_m@gangster@generic",
		"move_m@hipster@a",
		"move_m@hobo@a",
		"move_m@fat@a",
		"move_lester_CaneUp",
		"move_m@drunk@a",
		"move_m@drunk@moderatedrunk",
		"move_m@drunk@verydrunk",
		"move_f@sexy@a",
		"move_m@alien",
		"move_m@busines@a",
		"move_m@casual@a",
		"move_m@clipboard",
		"move_m@fire",
		"move_m@flee@a",
		"move_m@hiking",
		"move_m@hurry",
		"move_injured_generic",
		"move_m@intimidation@1h",
		"move_m@muscle@a",
		"move_m@quick",
		"move_m@sad",
		"move_m@shadyped@a",
		"move_m@shocked@a",
		"move_f@arrogant@a",
		"move_f@handbag",
		"move_f@heels",
		"move_p_m_one",
		"move_m@leaf_blower",
		"move_m@bail_bond_tazered",
		"move_m@tough_guy@",
		"move_m@bag",
	};
}


namespace Ptfx {

	inline int PtfxName_pos {};
	inline const char* PtfxName[] = 
	{
		"alien disintegrate","alien teleport","clown appears","clown death","fbi dd breach smoke","agency3b elec box","heist ornate banknotes","rcpap1 camera","firework xmas burst rgw",
		"sol1 sniper impact","ojdg4 boat exp","ojdg4 water exp","drug grd train exp","drug grd plane exp","fbi exp building","agency3b heli expl","josh3 light explosion","exp clown",
		"meth pipe smoke","ex2 car impact","mp plane landing tyre smoke","wheel burnout","indep firework starburst","indep firework fountain","indep firework shotburst","indep firework trailburst",
		"powerplay beast appear","ie jug mask flame","adversary judgement ash","adversary judgement lens dirt","alien charging","alien impact","alien beam","ch finale laser","stunts bomb fuse",
		"xs money rain","ba club champagne spray","ba club drink pour",
	};

	inline const char* PtfxCaller[] =
	{
		"scr_rcbarry1","scr_rcbarry1","scr_rcbarry2","scr_rcbarry2","scr_agencyheist","scr_agencyheistb","scr_ornate_heist","scr_rcpaparazzo1","proj_xmas_firework","scr_martin1","scr_oddjobtraffickingair",
		"scr_oddjobtraffickingair","scr_oddjobtraffickingair","scr_oddjobtraffickingair","scr_agencyheist","scr_agencyheist","scr_josh3","scr_rcbarry2","scr_familyscenem","scr_exile2","scr_mp_creator","scr_recartheft",
		"scr_indep_fireworks","scr_indep_fireworks","scr_indep_fireworks","scr_indep_fireworks","scr_powerplay","scr_impexp_jug","scr_bike_adversary","scr_bike_adversary","scr_rcbarry1","scr_rcbarry1","scr_rcbarry1","scr_ch_finale",
		"scr_stunts","scr_xs_celebration","scr_ba_club","scr_ba_club",
	};
	inline const char* PtfxCaller2[] =
	{
		"scr_alien_disintegrate","scr_alien_teleport","scr_clown_appears","scr_clown_death","scr_fbi_dd_breach_smoke","scr_agency3b_elec_box","scr_heist_ornate_banknotes","scr_rcpap1_camera","scr_firework_xmas_burst_rgw",
		 "scr_sol1_sniper_impact","scr_ojdg4_boat_exp","scr_ojdg4_water_exp","scr_drug_grd_train_exp","scr_drug_grd_plane_exp","scr_fbi_exp_building","scr_agency3b_heli_expl","scr_josh3_light_explosion","scr_exp_clown",
		"scr_meth_pipe_smoke","scr_ex2_car_impact","scr_mp_plane_landing_tyre_smoke","scr_wheel_burnout","scr_indep_firework_starburst","scr_indep_firework_fountain","scr_indep_firework_shotburst","scr_indep_firework_trailburst",
		 "scr_powerplay_beast_appear","scr_ie_jug_mask_flame","scr_adversary_judgement_ash","scr_adversary_judgement_lens_dirt","scr_alien_charging","scr_alien_impact","scr_alien_beam","scr_ch_finale_laser","scr_stunts_bomb_fuse",
		 "scr_xs_money_rain","scr_ba_club_champagne_spray","scr_ba_club_drink_pour",
	};
}



