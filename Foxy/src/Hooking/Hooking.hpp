#pragma once
#include "Pch/Common.hpp"
#include "HookingMethods/VMTHook.hpp"
#include "Memory/Patterns/Pointers.hpp"
#include "DirectX/Renderer/Renderer.hpp"
#include "R.A.G.Engine/Natives/Natives.hpp"
#include "Util/Protections.hpp"
#include "Core/Logger.hpp"
#include "HookingMethods/DetourHelper.hpp"
#include "R.A.G.Engine/WebConstuction.hpp"
#include "R.A.G.Engine/GamerHandle.hpp"
#include "R.A.G.Engine/CNetworkPlayerMgr.hpp"
#include "R.A.G.Engine/CPlayerGamerDataNode.hpp"
#include "R.A.G.Engine/BitBuffer.hpp"
#include "R.A.G.Engine/Network.hpp"
#include "UI/Manager/DXUiManager.hpp"
#include "Memory/BytePatch.hpp"
#include <sol/sol.hpp>
#define CALL(hk, ...) g_hooking->m_##hk##Hk.getOg<funcTypes::##hk##T>()(__VA_ARGS__);
#define CALL_DECL(hk, ...) g_hooking->m_##hk##Hk.getOg<decltype(&##hk)>()(__VA_ARGS__);
#define RET_CALL(hk, ...) return CALL(hk, __VA_ARGS__);
#define RET_CALL_DECL(hk, ...) return CALL_DECL(hk, __VA_ARGS__);

inline bool rage::rlGamerHandle::deserialize(class rage::datBitBuffer& buf) {
	if ((m_platform = buf.Read<uint8_t>(8)) != 3)
		return false;
	buf.ReadInt64((int64_t*)&m_rockstar_id, 64);
	m_flag = buf.Read<uint8_t>(8);
	return true;
}
inline bool rage::rlGamerHandle::serialize(class rage::datBitBuffer& buf) {
	if (!buf.Write<uint8_t>(uint8_t(m_platform), 8))
		return false;
	if (m_platform == 3) {
		buf.WriteQword(m_rockstar_id, 0x40);
		buf.Write<uint8_t>(m_flag, 8);
		return true;
	}
	return false;
}
inline bool rage::rlSessionInfo::serialize(rage::datBitBuffer& buf) {

	return true;
}
inline bool rage::rlSessionInfo::deserialize(rage::datBitBuffer& buf) {

	return true;
}
inline char* rage::rlSessionInfo::encode() {
	char gsinfo[0x100]{};
	base::pointers::g_getGsInfoFromSessionInfo(this, gsinfo, 0x7D, nullptr);
	return gsinfo;
}
inline bool rage::rlSessionInfo::decode(char* gsinfo) {
	return base::pointers::g_getSessionInfoFromGsInfo(this, gsinfo, nullptr);
}
inline Vector2 rage::vector2::serialize() {
	return Vector2(x, y);
}
inline Vector3 rage::vector3::serialize() {
	return Vector3(x, y, z);
}
inline Vector4 rage::vector4::serialize() {
	return Vector4(x, y, z, w);
}
namespace rage {
	class scrProgram;
	class scrThreadContext;
	class netConnectionManager;
	class netEventMgr;
	class datBitBuffer;
	class rlGamerInfo;
	namespace netConnection {
		class InFrame;
	}
	class netObject;
	class snMsgAddGamerToSessionCmd;
	union scrValue;
}
namespace base {
	struct dynamicFunction {
		LPCSTR m_moduleName{};
		LPCSTR m_exportName{};
		HMODULE m_module{ GetModuleHandleA(m_moduleName) };
		FARPROC operator*() {
			if (!m_module)
				m_module = GetModuleHandleA(m_moduleName);
			return GetProcAddress(m_module, m_exportName);
		}
	};
	inline memory::bytePatch* g_namePatch{};
	inline void* g_nonCleanSurface{};
	inline void* g_cleanSurface{};
	struct hooks {
		static rage::eThreadState scriptVirtualMachine(rage::scrValue* stack, rage::scrValue* globals, rage::scrProgram* program, rage::scrThreadContext* threadContext);
		static bool netTextMsgConstructor(rage::netConnectionManager* mgr, int msgId, CMsgTextMessage* msg, int flags, void* unk);
		static int64_t receiveCloneSync(CNetworkObjectMgr* objMgr, CNetGamePlayer* sender, CNetGamePlayer* receiver, uint16_t syncType, uint16_t objId, rage::datBitBuffer* buffer, uint16_t unk, int timestamp);
		static bool netConMgrChannelDispatchEvent(void* _this, void* pConMgr, rage::netConnection::InFrame* pEvent);
		static bool constructAddGamerToSessionCmd(void* _This, rage::snMsgAddGamerToSessionCmd* pGamerCmd);
		static bool writePlayerGameStateDataNode(rage::netObject* player, CPlayerGameStateDataNode* pNode);
		static void writePlayerGamerDataNode(rage::netObject* player, CPlayerGamerDataNode* pNode);
		static bool constructJoinRequest(RemoteGamerInfoMsg* info, void* data, int size, int* bits);
		static int multiplayerChatFilter(int64_t chatType, const char* input, const char** output);
		static bool getPeerAddress(uint64_t* peerAddress);
		static void getNetworkEventData(uint64_t _This, rage::CEventNetwork* netEvent);
		static bool hasRosPrivilege(void* _This, int privilege);
		static bool startMatchmakingFindSessions(int profileIndex, int availableSlots, NetworkGameFilterMatchmakingComponent* filter, uint32_t maxSessions, rage::rlSessionInfo* results, uint32_t* resultCount, int* state);
		static bool handleJoinRequest(Network* _This, rage::snSession* session, rage::rlGamerInfo* gamerInfo, CJoinRequestContext* ctx, BOOL isTransition);
		static bool constructPlayerDataMsg(CNetGamePlayerDataMsg* msg, rage::datBitBuffer* buffer);
		static void networkPlayerMgrInitialize(CNetworkPlayerMgr* _This, std::uint64_t ui64Unk, std::uint32_t ui32Unk, std::uint32_t ui32Arr4_Unk[4]);
		static void networkPlayerMgrShutdown(CNetworkPlayerMgr* _This);
		static void queueDependency(void* dependency);
		static bool hasIntervalElapsed(uint32_t timestamp, uint32_t interval);
		static LPVOID convertThreadToFiber(LPVOID param);
		static FARPROC getProcAddress(HMODULE hModule, LPCSTR lpProcName);
		static void copyResource(ID3D11DeviceContext* deviceContext, ID3D11Resource* pDstResource, ID3D11Resource* pSrcResource);
		static HRESULT present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags);
		static HRESULT getBuffer(IDXGISwapChain* swapChain, UINT Buffer, REFIID riid, void** ppSurface);
		static HRESULT resizeBuffers(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
	};
	class hookQueue {
	public:
		bool tickSge(int playerId, int64_t* argArr, int argCount) {
			std::vector<int64_t> args;
			for (auto i = 0; i < argCount; i++)
				args.push_back(argArr[i]);
			for (auto& fn : m_sgeFuncs) {
				auto thread = sol::thread::create(fn.second.lua_state());
				auto thrFn = sol::function(thread.lua_state(), sol::ref_index(fn.second.registry_index()));
				if (thrFn(playerId, args, argCount))
					return false;
			}
			args.clear();
			return true;
		}
		bool tickGcd(int playerId, std::string message, bool isTeam) {
			for (auto& fn : m_gcdFuncs) {
				auto thread = sol::thread::create(fn.second.lua_state());
				auto thrFn = sol::function(thread.lua_state(), sol::ref_index(fn.second.registry_index()));
				if (thrFn(playerId, message, isTeam))
					return false;
			}
			return true;
		}
	public:
		void add(std::string id, int type, sol::function func) {
			switch (type) {
			case 1: m_sgeFuncs.insert({ id, func }); break;
			case 2: m_gcdFuncs.insert({ id, func }); break;
			}
		}
		void remove(std::string id) {
			m_sgeFuncs.erase(id);
		}
	public:
		std::map<std::string, sol::function> m_sgeFuncs;
		std::map<std::string, sol::function> m_gcdFuncs;

	};
	inline hookQueue g_hookQueue;
	class hooking {
		friend struct hooks;
	public:
		hooking();
		~hooking();
	public:
		void hook();
		void unhook();
	public:
		MinHook m_mhKeepalive;
		detour m_scriptVirtualMachineHk;
		detour m_netTextMsgConstructorHk;
		detour m_receiveCloneSyncHk;
		detour m_netConMgrChannelDispatchEventHk;
		detour m_constructAddGamerToSessionCmdHk;
		detour m_writePlayerGameStateDataNodeHk;
		detour m_writePlayerGamerDataNodeHk;
		detour m_constructJoinRequestHk;
		detour m_multiplayerChatFilterHk;
		detour m_getPeerAddressHk;
		detour m_getNetworkEventDataHk;
		detour m_hasRosPrivilegeHk;
		detour m_startMatchmakingFindSessionsHk;
		detour m_handleJoinRequestHk;
		detour m_constructPlayerDataMsgHk;
		detour m_networkPlayerMgrShutdownHk;
		detour m_networkPlayerMgrInitializeHk;
		detour m_queueDependencyHk;
		detour m_hasIntervalElapsedHk;
		detour m_convertThreadToFiberHk;
		detour m_getProcAddressHk;
		hookVFT m_swapchainVftHk;
		std::unique_ptr<hookVFT> m_d3dContextVftHk;
	};
	inline std::unique_ptr<hooking> g_hooking;
}