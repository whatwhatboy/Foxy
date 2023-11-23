#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/Vectors.hpp"

#pragma pack(push, 4)
class CPhysicalAttachDataNode {
public:
	char pad_0000[192]; //0x0000
	bool m_attached; //0x00C0
	bool unk_00C1; //0x00C1
	uint16_t m_attached_to; //0x00C2
	char pad_00C4[12]; //0x00C4
	rage::vector3 m_offset; //0x00D0
	char pad_00DC[4]; //0x00DC
	rage::vector4 m_orientation; //0x00E0
	rage::vector3 m_parent_offset; //0x00F0
	char pad_00FC[4]; //0x00FC
	uint16_t m_other_attach_bone; //0x0100
	uint16_t m_attach_bone; //0x0102
	uint32_t m_attach_flags; //0x0104
	bool m_allow_initial_separation; //0x0108
	char pad_00109[3]; //0x0109
	float unk_010C; //0x010C
	float unk_0110; //0x0110
	bool unk_0114; //0x0114
	bool unk_0115; //0x0115
	bool m_is_cargo_vehicle; //0x0116
}; //Size: 0x0118
static_assert(sizeof(CPhysicalAttachDataNode) == 0x118);
#pragma pack(pop)
#pragma pack(push, 4)
class CPedAttachDataNode {
public:
	char pad_0000[192]; //0x0000
	rage::vector3 m_offset; //0x00C0
	char pad_00CC[4]; //0x00CC
	rage::vector4 m_orientation; //0x00D0
	uint16_t m_attached_to; //0x00E0
	uint16_t m_attachment_bone; //0x00E2
	uint32_t m_attachment_flags; //0x00E4
	float m_heading_1; //0x00E8
	float m_heading_2; //0x00EC
	bool m_attached; //0x00F0
	bool unk_00F1; //0x00F1
};
static_assert(sizeof(CPedAttachDataNode) == 0xF4);
#pragma pack(pop)
#pragma pack(push, 4)
class CTrainGameStateDataNode {
public:
	char pad_0000[192]; //0x0000
	bool m_is_engine; //0x00C0
	bool m_is_caboose; //0x00C1
	bool m_is_mission_train; //0x00C2
	bool m_direction; //0x00C3
	bool m_has_passenger_carriages; //0x00C4
	bool m_render_derailed; //0x00C5
	bool unk_00C6; //0x00C6
	bool unk_00C7; //0x00C7
	uint16_t m_engine_id; //0x00C8
	int8_t m_train_config_index; //0x00CA
	int8_t m_carriage_config_index; //0x00CB
	int8_t m_track_id; //0x00CC
	char pad_00CD[3]; //0x00CD
	float m_distance_from_engine; //0x00D0
	float m_cruise_speed; //0x00D4
	uint16_t m_linked_to_backward_id; //0x00D8
	uint16_t m_linked_to_forward_id; //0x00DA
	uint32_t m_train_state; //0x0DC
	bool unk_00E0; //0x00E0
	bool m_force_doors_open; //0x0E1
}; //Size: 0x00E4
static_assert(sizeof(CTrainGameStateDataNode) == 0xE4);
#pragma pack(pop)
class CVehicleCreationDataNode {
public:
	char pad_0000[192]; //0x0000
	uint32_t m_pop_type; //0x00C0
	uint32_t m_random_seed; //0x00C4
	uint32_t m_model; //0x00C8
	uint32_t m_vehicle_status; //0x00CC
	uint32_t m_max_health; //0x00D0
	uint32_t m_creation_token; //0x00D4
	bool m_car_budget; //0x00D8
	bool m_needs_to_be_hotwired; //0x00D9
	bool m_tires_dont_burst; //0x00DA
	char pad_00DB[165]; //0x00DB
}; //Size: 0x0180
static_assert(sizeof(CVehicleCreationDataNode) == 0x180);
class CPlayerAppearanceDataNode {
public:
	char pad_0x0[192]; //0x0
	uint32_t unk_0xC0[52]; //0xC0
	uint32_t m_component_bitset; //0x190
	char pad_0x194[4]; //0x194
	uint32_t unk_0x198[12]; //0x198
	uint32_t m_drawables[12]; //0x1C8
	uint32_t m_textures[12]; //0x1F8
	uint32_t m_palettes[12]; //0x228
	char pad_0x258[8]; //0x258
	uint32_t unk_0x260[6]; //0x260
	uint32_t unk_0x278[6]; //0x278
	char pad_0x290[8]; //0x290
	float unk_0x298; //0x298
	uint8_t unk_0x29C; //0x29C
	uint8_t unk_0x29D; //0x29D
	char pad_0x29E[26]; //0x29E
	float m_shape_mix; //0x2B8
	float m_skin_mix; //0x2BC
	float m_third_mix; //0x2C0
	float unk_0x2C4; //0x2C4
	float unk_0x2C8[13]; //0x2C8
	float unk_0x2FC[13]; //0x2FC
	float unk_0x330[20]; //0x330
	uint8_t unk_0x380[13]; //0x380
	uint8_t unk_0x38D[13]; //0x38D
	uint8_t unk_0x39A[13]; //0x39A
	uint8_t m_shape_first; //0x3A7
	uint8_t m_shape_second; //0x3A8
	uint8_t m_shape_third; //0x3A9
	uint8_t m_skin_first; //0x3AA
	uint8_t m_skin_second; //0x3AB
	uint8_t m_skin_third; //0x3AC
	uint8_t unk_0x3AD[13]; //0x3AD
	uint8_t unk_0x3BA[11]; //0x3BA
	int16_t unk_0x3C6; //0x3C6
	uint8_t unk_0x3C8; //0x3C8
	uint8_t unk_0x3C9; //0x3C9
	char pad_0x3CA[1]; //0x3CA
	bool unk_0x3CB; //0x3CB
	bool unk_0x3CC; //0x3CC
	char pad_0x3CD[3]; //0x3CD
	uint32_t unk_0x3D0; //0x3D0
	uint32_t unk_0x3D4; //0x3D4
	uint32_t unk_0x3D8; //0x3D8
	uint32_t unk_0x3DC; //0x3DC
	uint32_t unk_0x3E0; //0x3E0
	float unk_0x3E4; //0x3E4
	float unk_0x3E8; //0x3E8
	float unk_0x3EC; //0x3EC
	uint32_t unk_0x3F0; //0x3F0
	uint32_t unk_0x3F4; //0x3F4
	uint32_t unk_0x3F8; //0x3F8
	uint32_t unk_0x3FC; //0x3FC
	uint32_t unk_0x400; //0x400
	bool unk_0x404; //0x404
	bool unk_0x405; //0x405
	bool unk_0x406; //0x406
	bool unk_0x407; //0x407
	bool unk_0x408; //0x408
	bool unk_0x409; //0x409
	uint32_t unk_0x40C; //0x40C
	uint32_t m_model_hash; //0x410
	uint32_t m_voice_hash; //0x414
	uint32_t m_phone_mode; //0x418
	uint32_t unk_0x41C; //0x41C
	uint8_t m_parachute_tint_index; //0x420
	uint8_t m_parachute_pack_tint_index; //0x421
	uint16_t m_respawn_object; //0x422
	bool m_has_head_blend_data; //0x424
	bool unk_0x425; //0x425
	bool m_has_respawn_object; //0x426
	char pad_0x427; //0x427
	uint32_t unk_0x428_clip_maybe; //0x428
	uint32_t unk_0x42C; //0x42C
	uint32_t unk_0x430; //0x430
	bool unk_0x434; //0x434
	bool unk_0x435; //0x435
	bool unk_0x436; //0x436
	uint8_t unk_0x437; //0x437
	uint16_t unk_0x438; //0x438
	uint16_t unk_0x43A; //0x43A
	uint16_t unk_0x43C; //0x43C
	bool unk_0x43E; //0x43E
	bool unk_0x43F; //0x43F
	bool unk_0x440; //0x440
	uint8_t unk_0x441; //0x441
	uint32_t unk_0x442; //0x442
	uint32_t unk_0x446; //0x446
	inline int getDrawable(int index) {
		if (m_component_bitset & (1 << index))
			return m_drawables[index];
		return 0;
	}
};
class CPedComponents {
public:
	uint32_t m_component_bitset; //0x0
	char pad_0x4[4]; //0x4
	uint32_t unk_0x8[12]; //0x8
	uint32_t m_drawables[12]; //0x38
	uint32_t m_textures[12]; //0x68
	uint32_t m_palettes[12]; //0x98
};
static_assert(sizeof(CPedComponents) == 0xC8);
class CPedAppearanceDataNode {
public:
	char pad_0x0[192]; //0x0
	uint32_t unk_0xC0[6]; //0xC0
	uint32_t unk_0xD8[6]; //0xD8
	class CPedComponents components; //0xF0
	char pad_0x1B8[8]; //0x1B8
	uint32_t unk_0x1C0; //0x1C0
	uint8_t unk_0x1C4; //0x1C4
	uint8_t unk_0x1C5; //0x1C5
	char pad_0x1C6[2]; //0x1C6
	uint32_t unk_0x1C8; //0x1C8
	uint32_t unk_0x1CC; //0x1CC
	uint32_t unk_0x1D0; //0x1D0
	bool unk_0x1D4; //0x1D4
	bool unk_0x1D5; //0x1D5
	bool unk_0x1D6; //0x1D6
	uint8_t unk_0x1D7; //0x1D7
	uint16_t unk_0x1D8; //0x1D8
	uint16_t unk_0x1DA; //0x1DA
	uint16_t unk_0x1DC; //0x1DC
	bool unk_0x1DE; //0x1DE
	bool unk_0x1DF; //0x1DF
	bool unk_0x1E0; //0x1E0
	uint8_t unk_0x1E1; //0x1E1
};
static_assert(sizeof(CPedAppearanceDataNode) == 0x1E4);

class CSectorDataNode {
public:
	char pad_0000[192]; //0x0000
	uint16_t m_pos_x; //0x00C0
	uint16_t m_pos_y; //0x00C2
	uint16_t m_pos_z; //0x00C4
}; //Size: 0x00C6
static_assert(sizeof(CSectorDataNode) == 0xC6);

#pragma pack(push, 4)
class CPlayerGameStateDataNode {
public:
	char pad_0000[192];
	uint32_t m_player_state;
	bool m_controls_disabled_by_script;
	bool m_is_max_armor_and_health_default;
	bool unk_0xC6;
	bool m_is_spectating;
	bool m_is_antagonistic_to_another_player;
	bool m_never_target;
	bool m_use_kinematic_physics;
	bool unk_0xCB;
	bool unk_0xCC;
	bool unk_0xCD;
	bool unk_0xCE;
	bool unk_0xCF;
	bool unk_0xD0;
	bool unk_0xD1;
	bool m_random_peds_flee;
	bool m_every_ped_back_away;
	bool unk_0xD4;
	bool m_is_invincible;
	bool unk_0xD6;
	bool unk_0xD7;
	bool unk_0xD8;
	bool unk_0xD9;
	bool m_bullet_proof;
	bool m_fire_proof;
	bool m_explosion_proof;
	bool m_collision_proof;
	bool m_melee_proof;
	bool m_water_proof;
	bool m_steam_proof;
	bool unk_0xE1;
	bool unk_0xE2;
	bool unk_0xE3;
	bool unk_0xE4;
	bool unk_0xE5;
	bool unk_0xE6;
	bool unk_0xE7;
	bool unk_0xE8;
	bool unk_0xE9;
	bool unk_0xEA;
	bool unk_0xEB;
	bool unk_0xEC;
	bool unk_0xED;
	bool unk_0xEE;
	bool unk_0xEF;
	bool unk_0xF0;
	bool unk_0xF1;
	bool unk_0xF2;
	bool unk_0xF3;
	bool unk_0xF4;
	bool unk_0xF5;
	bool unk_0xF6;
	bool unk_0xF7;
	bool unk_0xF8;
	bool unk_0xF9;
	bool unk_0xFA;
	bool unk_0xFB;
	uint32_t unk_0xFC;
	uint32_t m_mobile_ring_state;
	int32_t m_player_team;
	float m_air_drag_multiplier;
	uint32_t m_max_health;
	uint32_t m_max_armor;
	uint32_t unk_0x114;
	uint16_t word118;
	uint32_t m_override_receive_chat;
	uint32_t m_override_send_chat;
	bool unk_0x124;
	bool unk_0x125;
	bool unk_0x126;
	bool unk_0x127;
	uint16_t m_spectating_net_id;
	uint8_t m_antagonistic_to_player_id;
	uint8_t m_tutorial_index;
	uint8_t m_tutorial_instance_id;
	char pad_0x12D[2];
	uint32_t unk_0x130;
	uint32_t unk_0x134;
	bool m_is_overriding_voice_proximity;
	char pad_0x139[7];
	float m_voice_proximity_x;
	float m_voice_proximity_y;
	float m_voice_proximity_z;
	float m_voice_proximity_radius_maybe;
	uint32_t unk_0x150;
	uint32_t unk_0x154;
	bool unk_0x158;
	uint32_t m_decor_count;
	uint32_t m_decor_1_type;
	uint32_t m_decor_1_value;
	uint32_t m_decor_1_name;
	uint32_t m_decor_2_type;
	uint32_t m_decor_2_value;
	uint32_t m_decor_2_name;
	uint32_t m_decor_3_type;
	uint32_t m_decor_3_value;
	uint32_t m_decor_3_name;
	bool m_friendly_fire_allowed;
	bool unk_0x185;
	uint8_t m_current_garage_instance_index;
	uint8_t m_current_property_id;
	uint8_t unk_0x188;
	uint8_t unk_0x189;
	bool unk_0x18A;
	bool unk_0x18B;
	float m_weapon_defence_modifier;
	float m_weapon_defence_modifier_2;
	bool m_is_overriding_population_control_sphere;
	char pad_0x195[11];
	float m_population_control_sphere_x;
	float m_population_control_sphere_y;
	float m_population_control_sphere_z;
	uint16_t unk_0x1AC;
	uint16_t unk_0x1AE;
	uint16_t unk_0x1B0;
	bool pad_0x1B2;
	bool unk_0x1B3;
	bool m_no_collision;
	bool unk_0x1B5;
	bool unk_0x1B6;
	bool m_super_jump;
	bool unk_0x1B8;
	bool unk_0x1B9;
	uint16_t unk_0x1BA;
	uint32_t unk_0x1BC;
	float unk_0x1C0;
	float m_weapon_damage_modifier; //Divisor: 0x3F800000
	float m_melee_weapon_damage_modifier; //Divisor: 0x3F800000
	float unk_0x1CC;
	bool unk_0x1D0;
	char pad_0x1D1[15];
	float unk_0x1E0;
	float unk_0x1E4;
	float unk_0x1E8;
	uint32_t unk_0x1EC;
	uint8_t unk_0x1F0;
	uint8_t unk_0x1F1;
	bool unk_0x1F2;
	uint8_t unk_0x1F3;
};
#pragma pack(pop)

#pragma pack(push, 4)
class CDecorator {
public:
	uint32_t m_type;
	uint32_t m_value;
	uint32_t m_name_hash;

	bool isSet() {
		if (!m_value || !m_name_hash) {
			return false;
		}
		return true;
	}
}; //Size: 0x000C
static_assert(sizeof(CDecorator) == 0xC);

class CDynamicEntityGameStateDataNode {
public:
	char pad_0000[192]; // 0x0000
	uint32_t m_interior_index; // 0x00C0
	bool unk_00C4; // 0x00C4
	bool unk_00C5; // 0x00C5
	uint32_t m_decor_count; // 0x00C8
	CDecorator m_decors[12]; // 0x00CC

	bool ensureArraySecurity() {
		//If 13 or above, overflow can occur
		if (m_decor_count >= 12 + 1) {
			m_decor_count = 12;
			return false;
		}
		return true;
	}
	uint64_t* getArr() {
		return (uint64_t*)&m_decors[0];
	}
	uint32_t getArrSize() {
		return m_decor_count;
	}
}; //Size: 0x15C
static_assert(sizeof(CDynamicEntityGameStateDataNode) == 0x15C);
#pragma pack(pop)

#pragma pack(push, 4)
class CDoorCreationDataNode {
public:
	char pad_0000[192]; //0x0000
	uint32_t m_model; //0x00C0
	char pad_00C4[12]; //0x00C4
	rage::vector3 m_pos; //0x00D0
	char pad_00DC[12]; //0x00DC
	bool m_is_script_door; //0x00E8
	bool m_player_wants_control; //0x00E9
}; //Size: 0x00EC
static_assert(sizeof(CDoorCreationDataNode) == 0xEC);
#pragma pack(pop)

#pragma pack(push, 8)
class CPickupCreationDataNode {
public:
	char pad_0000[192]; //0x0000
	bool m_has_placement; //0x00C0
	char pad_00C1[7]; //0x00C1
	uint64_t* unk_struct_00C8; //0x00C8
	uint32_t unk_00D0; //0x00D0
	uint16_t unk_00D4; //0x00D4
	char pad_00D6[66]; //0x00D6
	uint32_t m_pickup_hash; //0x0118
	uint32_t m_amount; //0x011C
	uint32_t m_custom_model; //0x0120
	uint32_t m_life_time; //0x0124
	uint32_t m_weapon_component[11]; //0x0128
	uint32_t m_num_weapon_components; //0x0154
	uint32_t m_tint_index; //0x0158
	bool m_player_gift; //0x015C
	bool unk_015D; //0x015D
	char pad_015E[6]; //0x015E
	uint32_t unk_0164; //0x0164
	bool unk_0168; //0x0168
	bool ensureArraySecurity() {
		//If 12 or above, overflow can occur
		if (m_num_weapon_components >= sizeof(m_weapon_component) + 1) {
			m_num_weapon_components = 11;
			return false;
		}
		return true;
	}
}; //Size: 0x0170
static_assert(sizeof(CPickupCreationDataNode) == 0x170);
#pragma pack(pop)

#pragma pack(push, 4)
class CPedCreationDataNode {
public:
	char pad_0000[192]; //0x0000
	uint32_t m_pop_type;  //0x00C0
	uint32_t m_model; //0x00C4
	uint32_t m_random_seed; //0x00C8
	uint32_t m_max_health; //0x00CC
	bool m_in_vehicle; //0x00D0
	char pad_0xD1[1]; //0x00D1
	uint16_t m_vehicle_id; //0x00D2
	uint32_t m_vehicle_seat; //0x00D4
	bool m_has_prop; //0x00D8
	char pad_0xD9[3];  //0x00D9
	uint32_t m_prop_model; //0x00DC
	bool m_is_standing; //0x00E0
	bool m_is_respawn_object_id; //0x00E1
	bool m_is_respawn_flagged_for_removal; //0x00E2
	bool m_has_attr_damage_to_player; //0x00E3
	uint8_t m_attribute_damage_to_player; //0x00E4
	uint32_t m_voice_hash; //0x00E8
}; //Size: 0x00EC
static_assert(sizeof(CPedCreationDataNode) == 0xEC);
#pragma pack(pop)

#pragma pack(push, 2)
class CPedGameStateDataNode {
public:
	char pad_0000[192]; //0x0000
	bool m_weapon_component_something[11]; //0x0C0
	uint32_t m_weapon_component_hash[11]; //0x0CC
	uint32_t m_gadget_hash[3]; //0x0F8
	uint32_t unk_0104; // 0x0104
	uint32_t unk_0108; // 0x0108
	uint32_t unk_010C; // 0x010C
	float unk_0110; // 0x0110
	float unk_0114; // 0x0114
	float unk_0118; // 0x0118
	bool unk_011C; // 0x011C
	char pad_011D[3]; //0x011D
	uint32_t m_arrest_state; //0x0120
	uint32_t m_death_state; //0x0124
	uint32_t m_weapon_hash; //0x0128
	uint32_t m_num_weapon_components; //0x012C
	uint32_t m_num_equiped_gadgets; //0x0130
	uint32_t m_seat; //0x0134
	uint32_t m_action_mode_override; //0x0138
	uint32_t unk_013C; // 0x013C
	uint16_t m_vehicle; //0x0140
	uint16_t m_mount_id; //0x0142
	uint16_t m_custodian_id; //0x0144
	uint16_t unk_0146; // 0x0146
	bool m_tint_index; //0x0148
	char pad_0149; //0x0149
	uint8_t unk_014A; // 0x014A
	bool m_is_handcuffed; //0x014B
	bool m_can_preform_arrest; //0x014C
	bool m_can_preform_uncuff; //0x014D
	bool m_can_be_arrested; //0x014E
	bool m_is_in_custody; //0x014F
	char pad_0150; //0x0150
	bool m_weapon_exists; //0x0151
	bool m_weapon_visible; //0x0152
	bool m_weapon_has_ammo; //0x0153
	bool m_weapon_attach_left; //0x0154
	char pad_0155; //0x0155
	bool m_in_seat; //0x0156
	bool m_in_vehicle; //0x0157
	bool m_on_mount; //0x0158
	bool m_has_custodian_or_arrest_flags; //0x0159
	char pad_015A; //0x015A
	bool m_action_mode_enabled; //0x015B
	bool m_stealth_mode_enabled; //0x015C
	bool unk_015D; // 0x015D
	bool unk_015E; // 0x015E
	bool unk_015F; // 0x015F
	bool unk_0160; // 0x0160
	bool unk_0161; // 0x0161
	bool unk_0162; // 0x0162
	bool unk_0163; // 0x0163
	bool unk_0164; // 0x0164
	bool unk_0165; // 0x0165
	bool unk_0166; // 0x0166
	bool unk_0167; // 0x0167
	bool unk_0168; // 0x0168
	bool unk_0169; // 0x0169
	bool unk_016A; // 0x016A
	bool unk_016B; // 0x016B
	bool unk_016C; // 0x016C
	bool unk_016D; // 0x016D
	bool unk_016E; // 0x016E
	bool unk_016F; // 0x016F
	bool unk_0170; // 0x0170
	bool unk_0171; // 0x0171
	bool unk_0172; // 0x0172
	bool ensureArraySecurity_WeaponComponets() {
		//If 12 or above, overflow can occur
		if (m_num_weapon_components >= sizeof(m_weapon_component_something) + 1) {
			m_num_weapon_components = 11;
			return false;
		}
		return true;
	}
	bool ensureArraySecurity_Gadets() {
		//If 3 or above, overflow can occur
		if (m_num_equiped_gadgets >= sizeof(m_gadget_hash) + 1) {
			m_num_equiped_gadgets = 2;
			return false;
		}
		return true;
	}
	bool ensureArraySecurity() {
		return ensureArraySecurity_WeaponComponets() && ensureArraySecurity_Gadets();
	}
}; //Size: 0x0174
static_assert(sizeof(CPedGameStateDataNode) == 0x174);
#pragma pack(pop)

#pragma pack(push, 4)
class CPedInventoryDataNode {
public:
	char pad_0000[5232];
	uint32_t m_items[105];
	uint32_t m_num_items;
	uint32_t m_ammos[65];
	uint32_t m_ammo_quantities[65];
	uint32_t m_num_ammos;
	uint8_t unk_1680[105];
	uint8_t unk_16E9[105];
	char pad_1680[1260];
	bool m_infinite_ammos[65];
	bool m_ammo_all_infinite;
	bool ensureArraySecurity_Ammos() {
		//If 66 or above, overflow can occur
		if (m_num_ammos >= sizeof(m_ammos) + 1) {
			m_num_ammos = 66;
			return false;
		}
		return true;
	}
	bool ensureArraySecurity_Items() {
		//If 106 or above, overflow can occur
		if (m_num_items >= sizeof(m_items) + 1) {
			m_num_items = 105;
			return false;
		}
		return true;
	}
	bool ensureArraySecurity() {
		return ensureArraySecurity_Ammos() && ensureArraySecurity_Items();
	}
}; //Size: 0x1E24
static_assert(sizeof(CPedInventoryDataNode) == 0x1E24);
#pragma pack(pop)

#pragma pack(push, 4)
class CPedTaskSpecificDataNode {
public:
	char pad_0000[192]; //0x0000
	uint32_t m_task_index; //0x00C0
	uint32_t m_task_type; //0x00C4
	uint32_t m_buffer_size; //0x00C8
	uint8_t m_task_data_buffer[603]; //0x00CC
	bool ensureArraySecurity() {
		//If 604 or above, overflow can occur
		if (m_buffer_size >= sizeof(m_task_data_buffer) + 1) {
			m_buffer_size = 603;
			return false;
		}
		return true;
	}
}; //Size: 0x0328
static_assert(sizeof(CPedTaskSpecificDataNode) == 0x328);
#pragma pack(pop)

enum eVehicleGadgetType : uint32_t
{
	FORKS,
	SEARCH_LIGHT,
	PICK_UP_ROPE_WITH_HOOK,
	PICK_UP_ROPE_WITH_MAGNET,
	DIGGER_ARM,
	HANDLER_FRAME,
	BOMB_BAY,
};

#pragma pack(push, 4)
class CVehicleGadgetData {
public:
	eVehicleGadgetType m_gadget_type; //0x0000
	uint8_t m_data[94]; //0x0004
}; //Size: 0x64
static_assert(sizeof(CVehicleGadgetData) == 0x64);

class CVehicleGadgetDataNode {
public:
	char pad_0000[192]; //0x0000
	bool m_has_parent_offset; //0x00C0
	char pad_00C1[15]; //0x00C1
	uint32_t m_parent_offset_x; //0x00D0
	uint32_t m_parent_offset_y; //0x00D4
	uint32_t m_parent_offset_z; //0x00D8
	uint32_t m_parent_offset_w; //0x00DC
	uint32_t m_gadget_count; //0x00E0
	CVehicleGadgetData m_gadget_data[2]; //0x00E4
	bool ensureArraySecurity() {
		//If 3 or above, overflow can occur
		if (m_gadget_count >= sizeof(m_gadget_data) + 1) {
			m_gadget_count = 2;
			return false;
		}
		return true;
	}
}; //Size: 0x01AC
static_assert(sizeof(CVehicleGadgetDataNode) == 0x1AC);
#pragma pack(pop)