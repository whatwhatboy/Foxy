#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/Rage.hpp"
#include "R.A.G.Engine/Friends.hpp"
#include "R.A.G.Engine/CEntity.hpp"
#include "R.A.G.Engine/ReplayInterface.hpp"
#include "R.A.G.Engine/HashTable.hpp"
#include "R.A.G.Engine/DataNode.hpp"
#include "R.A.G.Engine/WebConstuction.hpp"
#include "R.A.G.Engine/UI3DScenes.hpp"
#include "R.A.G.Engine/pgDictionary.hpp"
#include <R.A.G.Engine/Natives/NativeDefines.hpp>
#include "Memory/BytePatch.hpp"

namespace rage {
	using Cmd = fnptr<void(class scrThreadInfo*)>;
	class scrNativeRegistrationTable;
	class scrProgramTable;
	class rlSessionInfo;
	class rlSessionByGamerTaskResult;
	class snSession;
	class snPlayer;
	class snMsgRemoveGamersFromSessionCmd;
	class snMsgAddGamerToSessionCmd;
	class netConnectionManager;
	class netConnectionPeer;
	class scrProgram;
	namespace netConnection {
		class InFrame;
	}
	class rlMetric;
	class rlPresenceEventInviteAccepted;
	class fwuiInputMessageBase;
	class CTransactionMgr;
	class CEventNetwork;
	class netTunnelDesc;
	class netTunnelRequest;
	class netStatus;
	class grcTextureStore;
	class grcTextureFactory;
	class grcTexture;
	class netObjMgrMessageHandler;
	union scrValue;
}
class CNetworkPlayerMgr;
class Network;
class CNetComplaintMgr;
class ScInfo;
class CEventNetworkTextMessageReceived;
class RemoteGamerInfoMsg;
class CPlayerGamerDataNode;
class CMsgTextMessage;
class CQueuedSessionInfoArray;
class NetworkGameFilterMatchmakingComponent;
class CJoinRequestContext;
class CMsgJoinResponse;
class CGameScriptHandlerMgr;

namespace funcTypes {
	using scriptVirtualMachineT = rage::eThreadState(*)(rage::scrValue* stack, rage::scrValue** globals, rage::scrProgram* program, rage::scrThreadContext* threadContext);
	using pointerToHandleT = int32_t(*)(rage::CEntity* ptr);
	using handleToPointerT = rage::CEntity*(*)(int32_t handle);
	using getSessionByGamerHandleT = bool(*)(int metricMgr, rage::rlGamerHandle* handles, int count, rage::rlSessionByGamerTaskResult* result, int unk, bool* success, int* state);
	using getSessionInfoFromGsInfoT = bool(*)(rage::rlSessionInfo* output, char* gsinfoBuffer, int32_t* bytesRead);
	using getGsInfoFromSessionInfoT = bool(*)(rage::rlSessionInfo* input, char* gsinfoBuffer, int32_t bufferSize, int32_t* bytesWritten);
	using joinBySessionInfoT = bool(*)(Network* network, rage::rlSessionInfo* info, int unk, int flags, rage::rlGamerHandle* handles, int handleCount);
	using startMatchmakingFindSessionsT = bool(*)(int profileIndex, int availableSlots, NetworkGameFilterMatchmakingComponent* filter, uint32_t maxSessions, rage::rlSessionInfo* results, uint32_t* resultCount, int* state);
	using readBitbufArrayT = bool(*)(rage::datBitBuffer* buffer, PVOID read, int bits, int unk);
	using writeBitbufArrayT = bool(*)(rage::datBitBuffer* buffer, PVOID read, int bits, int unk);
	using readBitsSingleT = bool(*)(uint8_t* data, uint32_t* output, int bits, int start);
	using writeBitsSingleT = int64_t(*)(uint8_t* data, uint32_t val, int bits, int start);
	using sendEventAckT = void(*)(rage::netEventMgr* mgr, CNetGamePlayer* sender, CNetGamePlayer* receiver, int idx, int handledBitset);
	using sendSyncAckT = void(*)(rage::netObjMgrMessageHandler* mgr, CNetGamePlayer* sender, CNetGamePlayer* receiver, uint16_t sequence, BOOL unk, BOOL unk2, uint16_t* objId, int64_t* ackCode);
	using receiveCloneSyncT = int64_t(*)(CNetworkObjectMgr* objMgr, CNetGamePlayer* sender, CNetGamePlayer* receiver, uint16_t syncType, uint16_t objId, rage::datBitBuffer* buffer, uint16_t unk, int timestamp);
	using netConMgrChannelDispatchEventT = bool(*)(void* _this, void* pConMgr, rage::netConnection::InFrame* pEvent);
	using getSyncTreeT = rage::netSyncTree*(*)(CNetworkObjectMgr* objMgr, uint16_t syncType);
	using getNetObjectT = rage::netObject*(*)(CNetworkObjectMgr* objMgr, uint16_t objId, bool canBeDeletedWhilePending);
	using constructAddGamerToSessionCmdT = bool(*)(void* _This, rage::snMsgAddGamerToSessionCmd* pGamerCmd);
	using writePlayerGameStateDataNodeT = bool(*)(rage::netObject* player, CPlayerGameStateDataNode* pNode);
	using writePlayerGamerDataNodeT = void(*)(rage::netObject* player, CPlayerGamerDataNode* pNode);
	using netTextMsgConstructorT = bool(*)(rage::netConnectionManager* mgr, int msgId, CMsgTextMessage* msg, int flags, void* unk);
	using readBitbufferIntoSyncTreeT = void(*)(rage::netSyncTree* tree, uint64_t flag, uint32_t flag2, rage::datBitBuffer* buffer, uint64_t netLogStub);
	using resetNetworkComplaintsT = void(*)(CNetComplaintMgr* mgr);
	using queuePacketWithPlayerT = bool(*)(CNetGamePlayer* player, void* data, int bufferSize, int flag, uint16_t* outStatus, CNetGamePlayer* localPlayer);
	using queuePacketViaMsgIdT = bool(*)(rage::netConnectionManager* mgr, int msgId, void* data, int size, int flags, void* unk);
	using sendPacketViaConIdT = void(*)(rage::netConnectionManager* manager, rage::netConnectionPeer* player, uint32_t connectionId, void* data, int32_t size, int32_t flags);
	using sendInviteAcceptedPresenceEventT = void(*)(void* presenceStruct, rage::rlPresenceEventInviteAccepted* invite, int flags);
	using constructJoinRequestT = bool(*)(RemoteGamerInfoMsg* info, void* data, int size, int* bits);
	using multiplayerChatFilterT = int(*)(int64_t chatType, const char* input, const char** output);
	using getPeerAddressT = bool(*)(uint64_t* peerAddress);
	using getNetworkEventDataT = void(*)(uint64_t _This, rage::CEventNetwork* netEvent);
	using hasRosPrivilegeT = bool(*)(void* _This, int privilege);
	using handleJoinRequestT = bool(*)(Network* _This, rage::snSession* session, rage::rlGamerInfo* gamerInfo, CJoinRequestContext* ctx, BOOL isTransition);
	using writeJoinResponseDataT = bool(*)(CMsgJoinResponse* response, void* data, int size, uint32_t* size_used);
	using constructPlayerDataMsgT = bool(*)(CNetGamePlayerDataMsg* msg, rage::datBitBuffer* buffer);
	using getGxtLabelFromTableT = const char*(*)(void* _This, const char* label);
	using getJoaatedGxtLabelFromTableT = const char*(*)(void* _This, rage::joaat_t hash);
	using networkPlayerMgrShutdownT = void(*)(CNetworkPlayerMgr* _This);
	using networkPlayerMgrInitializeT = void(*)(CNetworkPlayerMgr* _This, uint64_t ui64Unk, uint32_t ui32Unk, uint32_t ui32Arr4_Unk[4]);
	using getConnectionPeerT = rage::netConnectionPeer*(*)(rage::netConnectionManager* manager, uint64_t peerId);
	using openNetTunnelT = bool(*)(rage::netConnectionManager* mgr, rage::rlPeerInfo* peerInfo, rage::netTunnelDesc* description, rage::netTunnelRequest* request, rage::netStatus* status);
	using triggerScriptEventT = void(*)(int eventGroup, Any* args, int argCount, int playerBits);
	using setupStoreItemT = uint32_t*(*)(rage::grcTextureStore* store, uint32_t* out, uint32_t* dictHash);
	using addDictionaryToPoolT = void(*)(rage::grcTextureStore* pool, uint32_t slot, rage::pgDictionary* dict);
	using addGrcTextureToDictionaryT = void(*)(rage::pgDictionary* dict, uint32_t hash, rage::grcTexture* texture);
	using setupDictionaryT = rage::pgDictionary*(*)(rage::pgDictionary* dict, uint32_t count);
	using queueDependencyT = void(*)(void* dependency);
	using hasIntervalElapsedT = bool(*)(uint32_t timestamp, uint32_t interval);
	using addSyncLatencySampleT = void(*)(CNetworkObjectMgr* mgr, uint8_t playerId, int latency);
	using pushScenePresentMgrT = bool(*)(ui3DDrawMgr* mgr, uint32_t* hash);
	using addElementToSceneT = bool(*)(ui3DDrawMgr* mgr, uint32_t* hash, int element, CPed* cped, Vector3 pos, float alpha);
	using setSceneElementLightingT = bool(*)(ui3DDrawMgr* mgr, uint32_t* hash, int element, float lightingValue);
	using getScrCommandT = rage::Cmd(*)(rage::scrNativeRegistrationTable* _This, uint64_t TranslatedHash);
	using scrThreadInitT = void(*)(rage::scrThread* thread);
	using scrThreadTickT = rage::eThreadState(*)(rage::scrThread* thread, uint32_t opsToExecute);
	using scrThreadKillT = void(*)(rage::scrThread* thread);
	using receivedEventT = void(*)(rage::netEventMgr* _this, CNetGamePlayer* sender, CNetGamePlayer* receiver, uint16_t id, int index, int bitset, int timestamp, rage::datBitBuffer* buffer);
	using registerStreamingTextureT = uint32_t(*)(int32_t* idOut, const char* texturePath, bool unkTrue, const char* textureName, bool errorIfFailed);
}
namespace base {
	inline void vpMemcpy(void* destination, size_t size, void* source) {
		DWORD protect;
		VirtualProtect(destination, size, PAGE_EXECUTE_READWRITE, &protect);
		memcpy(destination, source, size);
		VirtualProtect(destination, size, protect, &protect);
	}
	template <typename aT, typename vT>
	inline void vpMemcpy(aT address, vT value) {
		DWORD protect;
		VirtualProtect((void*)address, sizeof(value), PAGE_EXECUTE_READWRITE, &protect);
		memcpy((void*)address, &value, sizeof(value));
		VirtualProtect((void*)address, sizeof(value), protect, &protect);
	}
	class CDispatch {
	public:
		char pad_0000[32]; //0x0000
		bool m_is_active; //0x0020
	}; //Size: 0x0020
	class CDispatchmentTable {
	public:
		void* unk_0000; //0x0000
		CDispatch m_dispatch[15]; //0x0008

		CDispatch& get(enum eDispatchType service) {
			return *reinterpret_cast<CDispatch*>(uint64_t(this) + (static_cast<uint64_t>(service) * 8));
		}
	}; //Size: 0x0078
	namespace pointers {
		extern void scanPointers();
		extern void applyPatches();
		extern void restorePatches();
		inline funcTypes::scriptVirtualMachineT g_scriptVirtualMachine{};
		inline funcTypes::handleToPointerT g_handleToPointer{};
		inline funcTypes::pointerToHandleT g_pointerToHandle{};
		inline funcTypes::getSessionByGamerHandleT g_getSessionByGamerHandle{};
		inline funcTypes::getSessionInfoFromGsInfoT g_getSessionInfoFromGsInfo{};
		inline funcTypes::getGsInfoFromSessionInfoT g_getGsInfoFromSessionInfo{};
		inline funcTypes::joinBySessionInfoT g_joinBySessionInfo{};
		inline funcTypes::startMatchmakingFindSessionsT g_startMatchmakingFindSessions{};
		inline funcTypes::readBitbufArrayT g_readBitbufArray{};
		inline funcTypes::writeBitbufArrayT g_writeBitbufArray{};
		inline funcTypes::readBitsSingleT g_readBitsSingle{};
		inline funcTypes::writeBitsSingleT g_writeBitsSingle{};
		inline funcTypes::sendEventAckT g_sendEventAck{};
		inline funcTypes::sendSyncAckT g_sendSyncAck{};
		inline funcTypes::receiveCloneSyncT g_receiveCloneSync{};
		inline funcTypes::netConMgrChannelDispatchEventT g_netConMgrChannelDispatchEvent{};
		inline funcTypes::getSyncTreeT g_getSyncTree{};
		inline funcTypes::getNetObjectT g_getNetObject{};
		inline funcTypes::constructAddGamerToSessionCmdT g_constructAddGamerToSessionCmd{};
		inline funcTypes::writePlayerGameStateDataNodeT g_writePlayerGameStateDataNode{};
		inline funcTypes::writePlayerGamerDataNodeT g_writePlayerGamerDataNode{};
		inline funcTypes::netTextMsgConstructorT g_netTextMsgConstructor{};
		inline funcTypes::readBitbufferIntoSyncTreeT g_readBitbufferIntoSyncTree{};
		inline funcTypes::resetNetworkComplaintsT g_resetNetworkComplaints{};
		inline funcTypes::queuePacketWithPlayerT g_queuePacketWithPlayer{};
		inline funcTypes::queuePacketViaMsgIdT g_queuePacketViaMsgId{};
		inline funcTypes::sendPacketViaConIdT g_sendPacketViaConId{};
		inline funcTypes::sendInviteAcceptedPresenceEventT g_sendInviteAcceptedPresenceEvent{};
		inline funcTypes::constructJoinRequestT g_constructJoinRequest{};
		inline funcTypes::multiplayerChatFilterT g_multiplayerChatFilter{};
		inline funcTypes::getPeerAddressT g_getPeerAddress{};
		inline funcTypes::getNetworkEventDataT g_getNetworkEventData{};
		inline funcTypes::hasRosPrivilegeT g_hasRosPrivilege{};
		inline funcTypes::handleJoinRequestT g_handleJoinRequest{};
		inline funcTypes::writeJoinResponseDataT g_writeJoinResponseData{};
		inline funcTypes::constructPlayerDataMsgT g_constructPlayerDataMsg{};
		inline funcTypes::getGxtLabelFromTableT g_getGxtLabelFromTable{};
		inline funcTypes::getJoaatedGxtLabelFromTableT g_getJoaatedGxtLabelFromTable{};
		inline funcTypes::networkPlayerMgrShutdownT g_networkPlayerMgrShutdown{};
		inline funcTypes::networkPlayerMgrInitializeT g_networkPlayerMgrInitialize{};
		inline funcTypes::getConnectionPeerT g_getConnectionPeer{};
		inline funcTypes::openNetTunnelT g_openNetTunnel{};
		inline funcTypes::triggerScriptEventT g_triggerScriptEvent{};
		inline funcTypes::setupStoreItemT g_setupStoreItem{};
		inline funcTypes::addDictionaryToPoolT g_addDictionaryToPool{};
		inline funcTypes::addGrcTextureToDictionaryT g_addGrcTextureToDictionary{};
		inline funcTypes::setupDictionaryT g_setupDictionary{};
		inline funcTypes::queueDependencyT g_queueDependency{};
		inline funcTypes::hasIntervalElapsedT g_hasIntervalElapsed{};
		inline funcTypes::addSyncLatencySampleT g_addSyncLatencySample{};
		inline funcTypes::pushScenePresentMgrT g_pushScenePresentMgr{};
		inline funcTypes::addElementToSceneT g_addElementToScene{};
		inline funcTypes::setSceneElementLightingT g_setSceneElementLighting{};
		inline funcTypes::getScrCommandT g_getScrCommand{};
		inline funcTypes::scrThreadInitT g_scrThreadInit{};
		inline funcTypes::scrThreadTickT g_scrThreadTick{};
		inline funcTypes::scrThreadKillT g_scrThreadKill{};
		inline funcTypes::receivedEventT g_receivedEvent{};
		inline funcTypes::registerStreamingTextureT g_registerStreamingTexture{};

		inline ui3DDrawMgr** g_ui3DSceneMgr{};
		inline memory::bytePatch* g_blameExplode{};
		inline rage::atArray<GtaThread*>* g_gtaThreads{};
		inline Network** g_network{};
		inline void* g_reportDependency{};
		inline uint8_t* g_rcsFlag{};
		inline uint32_t* g_networkTime{};
		inline rage::netEventMgr** g_netEventMgr{};
		inline rage::grcTextureFactory** g_textureFactory{};
		inline rage::grcTextureStore* g_textureStore{};
		inline void* g_gxtLabels{};
		inline rage::fwuiInputMessageBase** g_fwuiInputMessageBase{};
		inline rage::rlGamerInfo* g_chatGamerInfo{};
		inline CDispatchmentTable* g_dispatchSerivceTable{};
		inline uint32_t* g_region{};
		inline void* g_presenceStruct{};
		inline rage::scrProgramTable* g_scrProgramTbl{};
		inline HashTable<rage::CBaseModelInfo*>* g_modelTable{};
		inline CPedFactory** g_pedFactory{};
		inline CNetworkPlayerMgr** g_networkPlayerMgr{};
		inline CNetworkObjectMgr** g_netObjMgr{};
		inline CGameScriptHandlerMgr* g_gameScriptHandlerMgr{};
		inline CReplayInterface** g_replayInterface{};
		inline CFriendRegistry* g_friendRegistry{};
		inline ScInfo* g_socialClubInfo{};
		inline IDXGISwapChain** g_swapchain{};
		inline rage::scrNativeRegistrationTable* g_nativeRegTbl{};
		inline uint32_t* g_gameState{};
		inline rage::scrValue** g_globals{};
		inline uint32_t* g_threadId{};
		inline uint32_t* g_threadCount{};
		inline void* g_jmpRbxRegister{};
		inline HWND g_hwnd{};
	}
}