#pragma once
#include <cstdint>

namespace rage {
	enum class eKickReason : uint8_t {
		VotedOut,
		PeerComplaints,
		ConnectionError,
		NatType,
		SocialClub_Admin,
		SocialClub_AdminBlacklist,
		NumberOfReasons
	};
	enum class eNetMessage : uint32_t {
		MsgInvalid = 0xFFFFF,
		//Known Messages
		CMsgSessionAcceptChat = 0x62,
		snMMsgStartMatchCmd = 0x2D,
		CMsgSetInvitableCmd = 0x1F,
		CMsgSessionMemberIds = 0x23,
		CMsgRequestGamerInfo = 0x54,
		snMsgRemoveGamersFromSessionCmd = 0x53,
		CMsgNotMigrating = 0x35,
		CMsgMigrateHostResponse = 0x12,
		CMsgMigrateHostRequest = 0x66,
		CMsgJoinResponse = 0x2A,
		CMsgJoinRequest = 0x41,
		CMsgHostLeftWhilstJoiningCmd = 0x58,
		CMsgConfigResponse = 0x5F,
		CMsgConfigRequest = 0x48,
		CMsgChangeSessionAttributesCmd = 0x5A,
		snMsgAddGamerToSessionCmd = 0x64, //This is where send net info to lobby is called, among other things
		CMsgReassignResponse = 0x10,
		CMsgReassignNegotiate = 0x01,
		CMsgReassignConfirm = 0x26,
		CMsgPlayerData = 0x18,
		CMsgPackedReliables = 0x30,
		CMsgPackedCloneSyncACKs = 0x3B,
		CMsgNonPhysicalData = 0x16,
		CMsgNetArrayMgrUpdateAck = 0x5D,
		CMsgNetArrayMgrUpdate = 0x60,
		CMsgNetArrayMgrSplitUpdateAck = 0x25,
		CMsgScriptVerifyHostAck = 0x0B,
		CMsgScriptVerifyHost = 0x3E,
		CMsgScriptNewHost = 0x0E,
		CMsgScriptMigrateHostFailAck = 0x1A,
		CMsgScriptMigrateHost = 0x33,
		CMsgScriptLeaveAck = 0x40,
		CMsgScriptLeave = 0x17,
		CMsgScriptJoinHostAck = 0x4D,
		CMsgScriptJoinAck = 0x43,
		CMsgScriptJoin = 0x5C,
		CMsgScriptHostRequest = 0x67,
		CMsgScriptHandshakeAck = 0x5B,
		CMsgScriptHandshake = 0x57,
		CMsgScriptBotLeave = 0x2B, //Unused?
		CMsgScriptBotJoinAck = 0x63, //Unused?
		CMsgScriptBotJoin = 0x1C, //Unused?
		CMsgScriptBotHandshakeAck = 0x31, //Unused?
		CMsgScriptBotHandshake = 0x4B, //Unused?
		CMsgPartyLeaveGame = 0x3D,
		CMsgPartyEnterGame = 0x1E,
		CMsgCloneSync = 0x4E, //AKA, CloneSync, CloneRemove, etc.
		CMsgActivateNetworkBot = 0x65, //Unused?
		CMsgRequestObjectIds = 0x29,
		CMsgInformObjectIds = 0x09,
		CMsgTextMessage = 0x24, //fwuiInputMessageBase
		CMsgPlayerIsTyping = 0x61,
		CMsgPackedEvents = 0x4F, //ProcessPackedEvents (ReceivedEvent) constructor
		CMsgPackedEventReliablesCMsgs = 0x20,
		CMsgRequestKickFromHost = 0x0D,
		CMsgTransitionToGameStart = 0x50,
		CMsgTransitionToGameNotify = 0x02,
		CMsgTransitionToActivityStart = 0x06,
		CMsgTransitionToActivityFinish = 0x36,
		CMsgTransitionParameters = 0x3C,
		CMsgTransitionParameterString = 0x37,
		CMsgTransitionLaunchNotify = 0x1B,
		CMsgTransitionLaunch = 0x19,
		CMsgTransitionGamerInstruction = 0x14,
		CMsgTextMessage2 = 0x0A, //CMsgTestMessage (Should've been the name of this, and CMsgTextMessage should've been "CMsgChatMessage")
		CMsgSessionEstablishedRequest = 0x52,
		CMsgSessionEstablished = 0x07,
		CMsgRequestTransitionParameters = 0x42,
		CMsgRadioStationSyncRequest = 0x47,
		CMsgRadioStationSync = 0x46,
		CMsgPlayerCardSync = 0x3A,
		CMsgPlayerCardRequest = 0x6A,
		CMsgLostConnectionToHost = 0x81,
		CMsgKickPlayer = 0x34, //Host kick
		CMsgDebugStall = 0x7E, //Unused?
		CMsgCheckQueuedJoinRequestReply = 0x59,
		CMsgCheckQueuedJoinRequest = 0x51,
		CMsgBlacklist = 0x0C,
		CMsgRoamingRequestBubbleRequiredResponse = 0x83,
		CMsgRoamingRequestBubbleRequiredCheck = 0x82,
		CMsgRoamingRequestBubble = 0x2E,
		CMsgRoamingJoinBubble = 0x4C,
		CMsgRoamingJoinBubbleAck = 0x3F,
		CMsgRoamingInitialBubble = 0x32,
		CMsgVoiceStatus = 0x03,
		CMsgTextChatStatus = 0x00,
		CMsgJoinResponse2 = 0x08,
		CMsgJoinRequest2 = 0x68,
		//Interesting Messages
		CMsgNetTimeSync = 0x38, //Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 37
		CMsgNetComplaint = 0x55, //Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 54
		CMsgNetLagPing = 0x27, //Unused? Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 26
		CMsgSearchResponse = 0x6B, // unused? Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 6A
		CMsgSearchRequest = 0x05, //Unused?
		CMsgQosProbeResponse = 0x2C, //Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 2B
		CMsgQosProbeRequest = 0x1D, //Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 1C
		CMsgCxnRelayAddressChanged = 0x49, //Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 48
		CMsgCxnRequestRemoteTimeout = 0x2F, //Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 2E
		CMsgSessionDetailRequest = 0x22, //Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 21
		CMsgSessionDetailResponse = 0x13, //Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 12
		CMsgKeyExchangeOffer = 0x0F, //Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 0E (last result)
		CMsgKeyExchangeAnswer = 0x44, //Ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 43
		//Unknown Messages
		CMsg_0x87 = 0x87,
		CMsg_0x88 = 0x88,
		CMsg_0x80 = 0x80,
		CMsg_0x28 = 0x28,
		CMsg_0x11 = 0x11,
		CMsg_0x45 = 0x45,
		CMsg_0x89 = 0x89,
		CMsg_0x86 = 0x86,
	};
}