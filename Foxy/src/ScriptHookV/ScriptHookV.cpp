#include "ScriptHookV.hpp"
#include "R.A.G.Engine/Natives/Invoker/Invoker.hpp"
#include "Util/Globals.hpp"
#include "Util/Util.hpp"
#include "Fibers/Fiber.hpp"
#include "Fibers/FiberMgr.hpp"

namespace shv {
	void scriptRegister(HMODULE module, void(*LP_SCRIPT_MAIN)()) {
		static char name[256];
		if (GetModuleFileNameA(module, name, 256)) {
			if (strstr(name, "Menyoo")) {
				uint64_t base{ uint64_t(module) };
				uint64_t address{ base + 0x2CDC06 };
				std::vector<uint8_t> patch{ 0x90, 9 };
				DWORD oldProtect{};
				VirtualProtect((void*)address, patch.size(), PAGE_EXECUTE_READWRITE, &oldProtect);
				memcpy((void*)address, patch.data(), patch.size());
				VirtualProtect((void*)address, patch.size(), oldProtect, &oldProtect);
			}
		}
		LOG_DEBUG("Registered script {}", name);
		g_fiberMgr.add(std::format("0x{:X}:0x{:X}", (uint64_t)module, (uint64_t)LP_SCRIPT_MAIN).c_str(), LP_SCRIPT_MAIN);
	}
	void scriptRegisterAdditionalThread(HMODULE module, void(*LP_SCRIPT_MAIN)()) {
		g_fiberMgr.add(std::format("0x{:X}:0x{:X}", (uint64_t)module, (uint64_t)LP_SCRIPT_MAIN).c_str(), LP_SCRIPT_MAIN);
	}
	void scriptUnregister(HMODULE module) {
		g_fiberMgr.removeBase(std::format("0x{:X}", (uint64_t)module).c_str());
	}
	void scriptUnregisterDepricated(void(*LP_SCRIPT_MAIN)()) {
		g_fiberMgr.removeBase(std::format("0x{:X}", (uint64_t)LP_SCRIPT_MAIN).c_str());
	}
	void nativeInit(uint64_t hash) {
		g_hash = hash;
		g_invoker.begin();
	}
	void nativePush64(uint64_t val) {
		g_invoker.push(static_cast<uint64_t&&>(val));
	}
	PUINT64 nativeCall() {
		g_invoker.end(g_hash);
		return g_invoker.m_context.getResultPtr();
	}
	PUINT64 getGlobalPtr(int index) {
		return PUINT64(global(index).Value());
	}
	int registerRawStreamingFile(const std::string& fileName, const std::string& registerAs) {
		int32_t id{ -1 };
		pointers::g_registerStreamingTexture(&id, fileName.c_str(), true, registerAs.c_str(), false);
		return id;
	}
	int createTexture(const char* fileName) {
		return g_renderer->createTexture(fileName);
	}
	void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a) {
		g_renderer->drawTexture(id, index, level, time, sizeX, sizeY, centerX, centerY, posX, posY, rotation, screenHeightScaleFactor, r, g, b, a);
	}
	void scriptWait(DWORD waitTime) {
		fiber::cur()->wait(std::chrono::milliseconds(waitTime));
	}
	void keyboardHandlerRegister(TKeyboardFn function) {
		g_keyboardFunctions.insert(function);
	}
	void keyboardHandlerUnregister(TKeyboardFn function) {
		g_keyboardFunctions.erase(function);
	}
	eGameVersion getGameVersion() {
		return GetModuleHandleA("steam_api64.dll") ? VER_1_0_1493_1_STEAM : VER_1_0_1493_1_NOSTEAM;
	}
	BYTE* getScriptHandleBaseAddress(int handle) {
		return (BYTE*)pointers::g_handleToPointer(handle);
	}
	void presentCallbackUnregister(PresentCallback cb) {
		g_presentCallbacks.insert(cb);
	}
	void presentCallbackRegister(PresentCallback cb) {
		g_presentCallbacks.erase(cb);
	}
	template <typename t>
	int getPoolObjects(int type, int* arr, int arrSize) {
		std::vector<uint64_t> objects{};
		if (auto replayInterface{ *pointers::g_replayInterface }) {
			t* inf;
			switch (type) {
			case 0: {
				inf = (t*)replayInterface->m_vehicle_interface;
			} break;
			case 1: {
				inf = (t*)replayInterface->m_ped_interface;
			} break;
			case 2: {
				inf = (t*)replayInterface->m_object_interface;
			} break;
			case 3: {
				inf = (t*)replayInterface->m_pickup_interface;
			} break;
			}
			for (int32_t i{}; i != inf->m_count; ++i) {
				if (arrSize == i)
					break;
				objects.push_back(inf->m_list->addr(i));
			}
		}
		for (int32_t i{}; i != objects.size(); ++i) {
			arr[i] = util::entity::getEntityFromHandle((rage::CEntity*)objects[i]);
		}
		return objects.size();
	}
	int worldGetAllVehicles(int* arr, int arrSize) {
		return getPoolObjects<CVehicleInterface>(0, arr, arrSize);
	}
	int worldGetAllPeds(int* arr, int arrSize) {
		return getPoolObjects<CPedInterface>(1, arr, arrSize);
	}
	int worldGetAllObjects(int* arr, int arrSize) {
		return getPoolObjects<CObjectInterface>(2, arr, arrSize);
	}
	int worldGetAllPickups(int* arr, int arrSize) {
		return getPoolObjects<CPickupInterface>(3, arr, arrSize);
	}
}