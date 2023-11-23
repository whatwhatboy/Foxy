#pragma once
#include "Vectors.hpp"
#include "AtArray.hpp"
#include "grcRenderTarget.hpp"

class UISceneObjectData {
public:
	void set_element_pos(rage::vector2 xy, rage::vector2 wh) {
		m_bg_rect_xywh.z = wh.x; //Make fullscreen
		m_bg_rect_xywh.w = wh.y; //Make fullscreen
		m_bg_rect_xywh.x = xy.x; //Convert and set pos
		m_bg_rect_xywh.y = xy.y; //Convert and set pos
		m_bg_rect_xywh_43.z = wh.x; //Make fullscreen
		m_bg_rect_xywh_43.w = wh.y; //Make fullscreen
		m_bg_rect_xywh_43.x = xy.x; //Convert and set pos
		m_bg_rect_xywh_43.y = xy.y; //Convert and set pos
		m_bg_rect_color = NULL;
	}
public:
	char pad_0000[16]; //0x0000
	rage::vector4 m_position; //0x0010
	rage::vector4 m_position_43; //0x0020
	rage::vector4 m_rotation; //0x0030
	rage::vector4 m_bg_rect_xywh; //0x0040
	rage::vector4 m_bg_rect_xywh_43; //0x0050
	uint32_t m_bg_rect_color; //0x0060 - Divisor 0x3F800000
	uint32_t m_blend_color; //0x0064 - Divisor 0x3F800000
	bool m_enabled; //0x0065
	char pad_0066[23]; //0x006
}; //Size: 0x0080
static_assert(sizeof(UISceneObjectData) == 0x80);
class UIScenePreset {
public:
	rage::joaat_t m_scene_hash; //0x0000
	char pad_0004[12]; //0x0004
	class UISceneObjectData m_object_list[5]; //0x0010
	char pad_0290[16]; //0x0290
}; //Size: 0x2A0
static_assert(sizeof(UIScenePreset) == 0x2A0);
class UISceneObjectPatchedData {
public:
	class CPed* m_scene_ped; //0x0000
	char pad_0008[8]; //0x0008
	rage::vector4 m_ped_position; //0x0010
	char pad_0020[52]; //0x0020
	float m_ped_alpha; //0x0054
	char pad_0058[8]; //0x0058
}; //Size: 0x0060
static_assert(sizeof(UISceneObjectPatchedData) == 0x60);
class ui3DDrawMgr {
public:
	UIScenePreset* get_present_from_hash(rage::joaat_t hash) {
		int presentCount{}, index{};
		if (!m_preset_count)
			return 0;
		uint64_t address{ decltype(address)(m_presets[0]) };
		for (rage::joaat_t* lookup{ decltype(lookup)(address) }; *lookup != hash; lookup += sizeof(rage::grcRenderTarget)) {
			++presentCount, ++index;
			if (presentCount >= m_preset_count)
				return nullptr;
		}
		return (UIScenePreset*)(address + sizeof(UIScenePreset) * index);
	}
	bool push_present(uint32_t hash) {
		if (!m_is_preset_set && !m_current_preset) {
			m_current_preset = get_present_from_hash(hash);
			return true;
		}
		return false;
	}
	void push_scene_lighting(uint32_t hash, int element, float value) {
		UIScenePreset* preset{ get_present_from_hash(hash) };
		preset->m_object_list[element].m_bg_rect_color = value / 0x3F800000;
	}
	void add_element_to_scene(uint32_t hash, int element, CPed* ped, rage::vector4 pos, float alpha) {
		if (m_current_preset) {
			UIScenePreset* preset{ get_present_from_hash(hash) };
			if (element >= 5)
				return;
			auto& data{ m_patched_data[0] };
			auto& objects{ preset->m_object_list[element] };
			data.m_scene_ped = ped;
			data.m_ped_position = pos;
			data.m_ped_alpha = alpha;
		}
	}
public:
	class rage::grcRenderTarget* m_render_targets[12]; //0x0000
	char pad_0060[1958]; //0x0060
	class UIScenePreset** m_presets; //0x0808
	uint32_t m_preset_count; //0x0810
	uint64_t unk_0818; //0x0818
	class UISceneObjectPatchedData m_patched_data[5]; //0x0820
	class UIScenePreset* m_current_preset; //0x0A00
	char pad_0A08[18]; //0x0A08
	uint8_t m_is_preset_set; //0x0A1A
	char pad_0A1B[21]; //0x0A1B
}; //Size: 0x0A30
static_assert(sizeof(ui3DDrawMgr) == 0xA30);