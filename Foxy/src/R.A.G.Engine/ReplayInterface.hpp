#pragma once
#include "Pch/Common.hpp"
#include "CPedFactory.hpp"
#include "NetObject.hpp"

class CPickup {
public:
	char pad_0000[48]; //0x0000
	class CNavigation* m_navigation; //0x0030
	char pad_0038[88]; //0x0083
	rage::vector3 m_position; //0x0090
	char pad_009C[1012]; //0x009C
	int32_t m_money; //0x0490
	char pad_0484[260]; //0x0484
};//Size: 0x0588
class CPickupHandle {
public:
	CPickup* m_pointer; //0x0000 
	int32_t m_handle; //0x0008 
	char pad_0x000C[4]; //0x000C
}; //Size: 0x0010
class CPickupList {
public:
	CPickupHandle m_array[73]; //0x0000 
	uintptr_t addr(int index) {
		uintptr_t addr = (uintptr_t)static_cast<void*>(this) + (index * 0x10);
		return *(uintptr_t*)addr;
	}
}; //Size=0x0490
class CPickupInterface {
public:
	char pad_0x0000[0x100]; //0x0000
	CPickupList* m_list; //0x0100 
	int32_t m_size; //0x0108 
	int32_t m_count; //0x0110 
	CPickup* get(const int& index) {
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x0114
class CObjectHandle {
public:
	CObject* m_pointer; //0x0000
	int32_t m_handle; //0x0008
	char pad_000C[4]; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CObjectHandle) == 0x10);
class CObjectList {
public:
	CObjectHandle m_array[2300]; //0x0000
	uintptr_t addr(int index) {
		uintptr_t addr = (uintptr_t)static_cast<void*>(this) + (index * 0x10);
		return *(uintptr_t*)addr;
	}
}; //Size: 0x8FC0
class CObjectInterface {
public:
	char pad_0000[344]; //0x0000
	CObjectList* m_list; //0x0158
	int32_t m_size; //0x0160
	char pad_0164[4]; //0x0164
	int32_t m_count; //0x0168
	CObject* get(const int& index) {
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x016C
class CPedHandle {
public:
	CPed* m_pointer; //0x0000
	int32_t m_handle; //0x0008
	char pad_000C[4]; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CPedHandle) == 0x10);
class CPedList {
public:
	CPedHandle m_array[256]; //0x0000
	uintptr_t addr(int index) {
		uintptr_t addr = (uintptr_t)static_cast<void*>(this) + (index * 0x10);
		return *(uintptr_t*)addr;
	}
}; //Size: 0x1000
class CPedInterface {
public:
	char pad_0000[256]; //0x0000
	CPedList* m_list; //0x0100
	int32_t m_size; //0x0108
	char pad_010C[4]; //0x010C
	int32_t m_count; //0x0110
	CPed* get(const int& index) {
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x0114
class CVehicleHandle {
public:
	class CVehicle* m_pointer; //0x0000
	int32_t m_handle; //0x0008
	char pad_000C[4]; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CVehicleHandle) == 0x10);
class CVehicleList {
public:
	CVehicleHandle m_array[300]; //0x0000
	uintptr_t addr(int index) {
		uintptr_t addr = (uintptr_t)static_cast<void*>(this) + (index * 0x10);
		return *(uintptr_t*)addr;
	}
}; //Size: 0x12C0
class CVehicleInterface {
public:
	char pad_0000[384]; //0x0000
	CVehicleList* m_list; //0x0180
	int32_t m_size; //0x0188
	char pad_018C[4]; //0x018C
	int32_t m_count; //0x0190
	CVehicle* get(const int& index) {
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x0194
class CCameraInterface {
public:
	char pad_0x0000[1032]; //0x0000
}; //Size: 0x0408
class CReplayInterface {
public:
	char pad_0000[8]; //0x0000
	CCameraInterface* m_camera_interface; //0x0008
	CVehicleInterface* m_vehicle_interface; //0x0010
	CPedInterface* m_ped_interface; //0x0018
	CPickupInterface* m_pickup_interface; //0x0020 
	CObjectInterface* m_object_interface; //0x0028
	char pad_0030[16]; //0x0030
}; //Size: 0x0040