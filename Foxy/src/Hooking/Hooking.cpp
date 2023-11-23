#include "Hooking.hpp"
#include "Hooking/Hooks/Natives/NativeHooking.hpp"

namespace base {
	hooking::hooking() :
		m_mhKeepalive(),
		m_scriptVirtualMachineHk(xorstr_("SVM"), pointers::g_scriptVirtualMachine, &hooks::scriptVirtualMachine, true),
		m_netTextMsgConstructorHk(xorstr_("NTMC"), pointers::g_netTextMsgConstructor, &hooks::netTextMsgConstructor),
		m_receiveCloneSyncHk(xorstr_("RCS"), pointers::g_receiveCloneSync, &hooks::receiveCloneSync),
		m_netConMgrChannelDispatchEventHk(xorstr_("NCMCDE"), pointers::g_netConMgrChannelDispatchEvent, &hooks::netConMgrChannelDispatchEvent),
		m_constructAddGamerToSessionCmdHk(xorstr_("CAGTSC"), pointers::g_constructAddGamerToSessionCmd, &hooks::constructAddGamerToSessionCmd, true),
		m_writePlayerGameStateDataNodeHk(xorstr_("WPGSDN"), pointers::g_writePlayerGameStateDataNode, &hooks::writePlayerGameStateDataNode),
		m_writePlayerGamerDataNodeHk(xorstr_("WPGDN"), pointers::g_writePlayerGamerDataNode, &hooks::writePlayerGamerDataNode),
		m_constructJoinRequestHk(xorstr_("CJR"), pointers::g_constructJoinRequest, &hooks::constructJoinRequest),
		m_multiplayerChatFilterHk(xorstr_("MCF"), pointers::g_multiplayerChatFilter, &hooks::multiplayerChatFilter),
		m_getPeerAddressHk(xorstr_("GPA"), pointers::g_getPeerAddress, &hooks::getPeerAddress),
		m_getNetworkEventDataHk(xorstr_("GNED"), pointers::g_getNetworkEventData, &hooks::getNetworkEventData),
		m_hasRosPrivilegeHk(xorstr_("HRP"), pointers::g_hasRosPrivilege, &hooks::hasRosPrivilege),
		m_startMatchmakingFindSessionsHk(xorstr_("SMFS"), pointers::g_startMatchmakingFindSessions, &hooks::startMatchmakingFindSessions, true),
		m_handleJoinRequestHk(xorstr_("HJR"), pointers::g_handleJoinRequest, &hooks::handleJoinRequest),
		m_constructPlayerDataMsgHk(xorstr_("CPDM"), pointers::g_constructPlayerDataMsg, &hooks::constructPlayerDataMsg),
		m_networkPlayerMgrShutdownHk(xorstr_("NPMS"), pointers::g_networkPlayerMgrShutdown, &hooks::networkPlayerMgrShutdown, true),
		m_networkPlayerMgrInitializeHk(xorstr_("NPMI"), pointers::g_networkPlayerMgrInitialize, &hooks::networkPlayerMgrInitialize, true),
		m_queueDependencyHk(xorstr_("QD"), pointers::g_queueDependency, &hooks::queueDependency),
		m_hasIntervalElapsedHk(xorstr_("HIE"), pointers::g_hasIntervalElapsed, &hooks::hasIntervalElapsed),
		m_convertThreadToFiberHk(xorstr_("CTTF"), *dynamicFunction{ xorstr_("kernel32.dll"), xorstr_("ConvertThreadToFiber") }, &hooks::convertThreadToFiber),
		m_getProcAddressHk(xorstr_("GPA"), &GetProcAddress, &hooks::getProcAddress),
		m_swapchainVftHk(*pointers::g_swapchain, 19)
	{
		IDXGISwapChain* sc{ *pointers::g_swapchain };
		ID3D11Device* dev{};
		ID3D11DeviceContext* ctx{};
		sc->GetDevice(__uuidof(ID3D11Device), (void**)&dev);
		dev->GetImmediateContext(&ctx);
		m_d3dContextVftHk = std::make_unique<hookVFT>(ctx, 115);
		m_d3dContextVftHk->hook(&hooks::copyResource, 115 - 1 - 68);
		m_swapchainVftHk.hook(&hooks::present, 8);
		m_swapchainVftHk.hook(&hooks::getBuffer, 9);
		m_swapchainVftHk.hook(&hooks::resizeBuffers, 13);
		detour::enableAll();
		g_nativeHooks = std::make_unique<nativeHooks>();
	}
	hooking::~hooking() {
		m_swapchainVftHk.unhook(8);
		m_swapchainVftHk.unhook(9);
		m_swapchainVftHk.unhook(13);
		m_d3dContextVftHk->unhook(115 - 1 - 68);
		g_nativeHooks.reset();
		m_d3dContextVftHk.reset();
	}
	void hooking::hook() {
		m_swapchainVftHk.enable();
		MH_ApplyQueued();
	}
	void hooking::unhook() {
		if (!this)
			return;
		m_swapchainVftHk.disable();
		detour::disableAll();
		MH_ApplyQueued();
	}
}