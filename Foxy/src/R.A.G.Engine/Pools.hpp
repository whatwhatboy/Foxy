#pragma once
#include "Pch/Common.hpp"
#include "Memory/Patterns/Pointers.hpp"

namespace rage {
	struct entTask {
		bool typeHasFlag(ePoolTypes flag) {
			assert(!(_type & ~31));
			assert(!(static_cast<uint16_t>(flag) & ~31));
			return (_type & static_cast<uint16_t>(flag)) != 0;
		}
		entTask(ePoolTypes type) : _type(static_cast<uint16_t>(type)) {}
		void run(std::vector<uintptr_t>& ptrs) {
			auto replayInf = *base::pointers::g_replayInterface;
			auto pedInf = replayInf->m_ped_interface;
			auto vehInf = replayInf->m_vehicle_interface;
			auto objInf = replayInf->m_object_interface;
			auto pckInf = replayInf->m_pickup_interface;
			if (typeHasFlag(ePoolTypes::ped)) {
				auto list = pedInf->m_ped_list;
				for (auto i = 0; i < pedInf->m_max_peds; i++) {
					if (auto address = list->getAddress(i)) {
						ptrs.push_back(address);
					}
				}
			}
			if (typeHasFlag(ePoolTypes::vehicle)) {
				auto list = vehInf->m_vehicle_list;
				for (auto i = 0; i < vehInf->m_max_vehicles; i++) {
					if (auto address = list->getAddress(i)) {
						ptrs.push_back(address);
					}
				}
			}
			if (typeHasFlag(ePoolTypes::object)) {
				auto list = objInf->m_object_list;
				for (auto i = 0; i < objInf->m_max_objects; i++) {
					if (auto address = list->getAddress(i)) {
						ptrs.push_back(address);
					}
				}
			}
			if (typeHasFlag(ePoolTypes::pickup)) {
				auto list = pckInf->m_pickup_list;
				for (auto i = 0; i < pckInf->m_max_pickups; i++) {
					if (auto address = list->getAddress(i)) {
						ptrs.push_back(address);
					}
				}
			}
			/*if (typeHasFlag(ePoolTypes::camera)) {
				for (auto i = 0; i < base::pointers::g_cameraPool->size; i++) {
					if (auto address = base::pointers::g_cameraPool->getAddress(i)) {
						ptrs.push_back(address);
					}
				}
			}*/
		}
	private:
		uint16_t _type;
	};
	inline void getEntPtr(entTask type, std::vector<uintptr_t>& result) {
		entTask(type).run(result);
	}
	inline std::vector<int32_t> getAllWorld(entTask type, int max) {
		using gsh = int64_t(*)(int64_t ent);
		int count = 0;
		std::vector<int32_t> entities;
		std::vector<uintptr_t> pointers;
		getEntPtr(type, pointers);
		for (auto& cEntity : pointers) {
			if (count == max)
				break;
			if (auto entity = base::pointers::g_getScriptHandle(cEntity); entity) {
				entities.push_back((const int32_t&)entity);
				count++;
			}
		}
		return entities;
	}
	inline int getAllWorld(entTask type, int max, int* arr) {
		auto entities = getAllWorld(type, max);
		for (int i = 0; i < entities.size(); ++i)
			arr[i] = entities[i];
		return (int)entities.size();
	}
}

class genericPool {
public:
	bool isValid(int i) {
		assert(i >= 0);
		return mask(i) != 0;
	}
	uint64_t getAddress(int i) {
		assert(i >= 0);
		return mask(i) & (poolStartAddress + i * itemSize);
	}
	long long mask(int i) {
		assert(i >= 0);
		long long num1 = byteArray[i] & 0x80; // check for high bit.
		return ~((num1 | -num1) >> 63);
	}
public:
	uint64_t poolStartAddress;
	uint8_t* byteArray;
	int size;
	int itemSize;
};