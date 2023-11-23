#include "Hooking/Hooking.hpp"
#include "Hooking/Hooks/ProcessPackedEvents.hpp"
#include "Hooking/Hooks/ReceiveCloneData.hpp"
#include "R.A.G.Engine/Network.hpp"
#include "R.A.G.Engine/BitBuffer.hpp"
#include "R.A.G.Engine/CMsgTextMessage.hpp"
#include "Util/Util.hpp"
#include "Util/Chat.hpp"
#include "Features/Features.hpp"

namespace base {
	//bool rage::netConMgr::Channel::DispatchEvent(rage::netConMgr* pConMgr, rage::netConnection::InFrame* pEvent);
	bool hooks::netConMgrChannelDispatchEvent(void* _this, void* pConMgr, rage::netConnection::InFrame* pEvent) {
		if (pEvent->GetId() == 0 || pEvent->GetId() == 4) {
			try {
				rage::datBitBuffer buffer(pEvent->m_data, pEvent->m_length);
				buffer.m_flagBits = 1;
				CNetGamePlayer* player{};
				if (rage::snPlayer* snPlayer{ util::network::session::getPointer()->getPlayerByPlatformData(pEvent->m_platform_data) }) {
					player = util::network::getCNetGamePlayerViaPeerAddress(snPlayer->m_gamer_info.m_peer_address);
				}
				rage::eNetMessage msgType{};
				if (player && player->IsConnected() && util::network::misc::deserialiseNetMessage(msgType, buffer)) {
					if (player->m_player_id != util::player::g_entityHandle) {
						switch (msgType) {
						case rage::eNetMessage::snMsgAddGamerToSessionCmd: {
							g_dxUiManager.sendNotify(xorstr_("Playerlist"), xorstr_("{} has joined"), player->GetName());
						} break;
						case rage::eNetMessage::snMsgRemoveGamersFromSessionCmd: {
							rage::snMsgRemoveGamersFromSessionCmd cmd;
							buffer.ReadQword(&cmd.m_session_id, 64);
							buffer.ReadDword(&cmd.m_num_handles, 6);
							for (uint32_t i{}; i != cmd.m_num_handles; ++i) {
								buffer.ReadQword(&cmd.m_handles[i].m_rockstar_id, 64);
							}
							if (player && cmd.m_num_handles == 1 && pEvent->m_msg_id == -1) {
								CNetGamePlayer* receiver{};
								if (auto snPeer = util::network::session::peer::getViaConnectionId(0); snPeer) {
									auto rlGamerInfo = snPeer->m_gamer_data;
									if (cmd.m_handles[0].m_rockstar_id == rlGamerInfo.m_unk_gamer_handle.m_rockstar_id) {
										receiver = util::network::getCNetGamePlayerViaPeerAddress(rlGamerInfo.m_peer_address);
									}
								}
								if (util::player::g_networkHandle->IsNetworkHost()) {
									if (receiver)
										g_dxUiManager.sendNotify(xorstr_("Playerlist"), xorstr_("{} tried to breakup kick {}"), player->GetName(), receiver->GetName());
									return true;
								}
								else {
									if (receiver)
										g_dxUiManager.sendNotify(xorstr_("Playerlist"), xorstr_("{} breakup kicked {}"), player->GetName(), receiver->GetName());
								}
							}
						} break;
						case rage::eNetMessage::CMsgPackedEvents: {
							CMsgPackedEvents cMsgPackedEvents{};
							if (cMsgPackedEvents.Serialize(&buffer)) {
								uint32_t bufPos{};
								uint32_t rEvent_BufferPos{};
								uint32_t pos{};
								auto buffer = &cMsgPackedEvents.m_data.m_buffer;
								bufPos = buffer->GetMaxDataLength(), pos = buffer->GetPosition();
								if ((int)(bufPos - pos) >= 39) {
									do {
										uint16_t id{ 0x58 };
										int32_t idx{};
										int32_t handledBitset{};
										uint32_t bufferSize{};
										uint32_t rawBufferSize{};
										bool extended{};
										buffer->ReadWord(&id, 0x7);
										buffer->ReadInt32(&idx, 0x9);
										buffer->ReadInt32(&handledBitset, 0x8);
										buffer->ReadDword(&bufferSize, 0xF);
										buffer->ReadBool(&extended);
										if (extended)
											buffer->ReadDword(&rawBufferSize, 16);
										uint8_t data[4096]{};
										if (bufferSize)
											buffer->ReadArray(data, bufferSize);
										rage::datBitBuffer eventBuffer{ data, bufferSize };
										eventBuffer.m_maxBit = bufferSize + 1;
										g_processPackedEvents(*pointers::g_netEventMgr, player, util::player::g_networkHandle, id, idx, handledBitset, rawBufferSize, &eventBuffer);
										rEvent_BufferPos = buffer->GetMaxDataLength(), pos = buffer->GetPosition();
									} while ((int)(rEvent_BufferPos - pos) >= 39);
								}
							}
						} break;
							/*case rage::eNetMessage::CMsgCloneSync: {
								CMsgCloneSync cMsgCloneSync{};
								if (cMsgCloneSync.Serialize(&buffer)) {
									CNetGamePlayer* sender{ util::network::getCNetGamePlayerViaConnectionId(pEvent->m_connection_identifier) }, *receiver{ util::player::g_networkHandle };
									rage::netObjMgrMessageHandler* msgHandler{ util::network::object::mgr::get()->m_handler };
									if (sender && receiver && util::network::object::mgr::get()) {
										uint16_t last{ msgHandler->GetLastSequence(sender) };
										if (!msgHandler->IsSequenceGreater(cMsgCloneSync.m_net_sequence, last, 16)) {
											uint16_t objId{};
											int64_t ackCode{ 3 };
											pointers::g_sendSyncAck(msgHandler, sender, receiver, cMsgCloneSync.m_net_sequence, 1, 1, &objId, &ackCode);
											return false;
										}
										msgHandler->SetLastSequence(sender, cMsgCloneSync.m_net_sequence);
										rage::datBitBuffer* msgBuffer{ &cMsgCloneSync.m_data.m_buffer };
										uint16_t objectIds[128]{};
										uint32_t ackCodes[128]{};
										uint32_t numObjectsInMessage{};
										uint32_t numFailedObjects{};
										uint32_t payloadSize{ msgBuffer->m_maxBit };
										uint32_t bitsRemaining{ payloadSize - msgBuffer->GetPosition() };
										bool tChecked{};
										while (bitsRemaining > 35) {
											int32_t oldBitsRemaining = bitsRemaining;
											uint32_t timestamp{ msgBuffer->Read<uint32_t>(32) };
											if (sender->m_player_id != -1) {
												uint32_t netTime{ *pointers::g_networkTime };
												uint32_t latency{ netTime > timestamp ? netTime - timestamp : 0 };
												pointers::g_addSyncLatencySample(util::network::object::mgr::get(), sender->m_player_id, latency);
												if (!tChecked) {
													if (msgHandler->m_frame_times[sender->m_player_id] != 0) {
														uint32_t diff = timestamp - msgHandler->m_frame_times[sender->m_player_id];
														if (diff > 100) {
															return false;
														}
													}
													else {
														msgHandler->m_frame_times[sender->m_player_id] = timestamp;
													}
													tChecked = true;
												}
											}
											uint8_t numObjects{ msgBuffer->Read<uint8_t>(3) };
											for (uint8_t i{}; i != numObjects; ++i) {
												uint32_t type{ msgBuffer->Read<uint32_t>(4) }, id{ msgBuffer->Read<uint32_t>(13) };
												uint32_t bitPos{ msgBuffer->GetPosition() };
												uint32_t bytePos{ bitPos >> 3 };
												uint32_t offset{ bitPos - bytePos * 8 };
												uint32_t size{ ((payloadSize - bitPos) >> 3) + 1 };
												rage::datBitBuffer syncBuffer{ rage::datBitBuffer(&((uint8_t*)msgBuffer->m_data)[bytePos], size << 3) };
												syncBuffer.m_maxBit = size << 3;
												syncBuffer.m_bitOffset = offset;
												*pointers::g_rcsFlag = 0x0;
												uint32_t ackCode = g_receiveCloneData(util::network::object::mgr::get(), sender, static_cast<uint16_t>(type), static_cast<uint16_t>(id), &syncBuffer);
												*pointers::g_rcsFlag = 0x1;
												if (numObjectsInMessage < 128) {
													objectIds[numObjectsInMessage] = (uint16_t)id;
													ackCodes[numObjectsInMessage] = ackCode;
													if (ackCode != 8) numFailedObjects++;
													numObjectsInMessage++;
												}
												bitPos += syncBuffer.GetPosition();
												msgBuffer->Seek(bitPos);
											}
										}
									}
								}
							} break;*/
							/*case rage::eNetMessage::CMsgPackedReliables: {
								CMsgPackedReliables cMsgPackedReliables{};
								if (cMsgPackedReliables.Serialize(&buffer)) {
									auto messageBuffer = &cMsgPackedReliables.m_creates.m_buffer;
									while (messageBuffer->GetDataLength() - messageBuffer->GetPosition() >= 38) {
										uint16_t objectType{}, objectId{}, flags{};
										uint32_t bits{};
										uint8_t createData[4096]{};
										messageBuffer->ReadWord(&objectType, 4);
										messageBuffer->ReadWord(&objectId, 13);
										messageBuffer->ReadWord(&flags, 8);
										messageBuffer->ReadDword(&bits, 13);
										if (bits > 0 && messageBuffer->ReadArray(createData, bits)) {
											rage::datBitBuffer createBuffer(createData, bits, false);
											createBuffer.m_maxBit = bits + 1;
											createBuffer.Seek(0);
											g_receiveCloneData(util::network::object::mgr::get(), player, objectType, objectId, &createBuffer);
										}
									}
									messageBuffer = &cMsgPackedReliables.m_create_acks.m_buffer;
									while (messageBuffer->GetDataLength() - messageBuffer->GetPosition() >= 38) {
										uint16_t objectId{};
										uint32_t ackCode{};
										messageBuffer->ReadWord(&objectId, 13u);
										messageBuffer->ReadDword(&ackCode, 4u);
										util::network::object::mgr::get()->HandleCloneCreateAck(player, util::player::g_networkHandle, objectId, ackCode);
									}
									messageBuffer = &cMsgPackedReliables.m_removes.m_buffer;
									while (messageBuffer->GetMaxDataLength() - messageBuffer->GetPosition() >= 18) {
										uint16_t objectId{};
										uint32_t token{};
										messageBuffer->ReadWord(&objectId, 13u);
										messageBuffer->ReadDword(&token, 5u);
										util::network::object::mgr::get()->HandleCloneRemove(player, util::player::g_networkHandle, objectId, token);
									}
									messageBuffer = &cMsgPackedReliables.m_remove_acks.m_buffer;
									while (messageBuffer->GetDataLength() - messageBuffer->GetPosition() >= 38) {
										uint16_t objectId{};
										uint32_t ackCode{};
										messageBuffer->ReadWord(&objectId, 13u);
										messageBuffer->ReadDword(&ackCode, 4u);
										util::network::object::mgr::get()->HandleCloneRemoveAck(player, util::player::g_networkHandle, objectId, ackCode);
									}
									return false;
								}
							} break;*/
						case rage::eNetMessage::CMsgSessionEstablished: {
							rage::rlGamerHandle handle{ 0 };
							if (auto rlGamerInfo = player->GetGamerInfo(); rlGamerInfo) {
								uint64_t sessionId{};
								buffer.ReadQword(&sessionId, 64);
								handle.deserialize(buffer);
								if (sessionId == (*pointers::g_network)->m_game_session_ptr->m_rline_session.m_session_id) {
									if (rlGamerInfo->m_unk_gamer_handle.m_rockstar_id != handle.m_rockstar_id) {
										g_dxUiManager.sendNotify(xorstr_("Modder Detection"), xorstr_("{} is a modder (Reason: Spoofed RID (Real RID: {}) | Reason code: M[R]_SPFRID)"), rlGamerInfo->m_unk_gamer_handle.m_rockstar_id, player->GetName());
										handle.m_rockstar_id = rlGamerInfo->m_unk_gamer_handle.m_rockstar_id;
										handle.serialize(buffer);
										return true;
									}
								}
							}
						} break;
						case rage::eNetMessage::CMsgLostConnectionToHost: {
							uint64_t sessionId{};
							buffer.ReadQword(&sessionId, 64);
							rage::rlGamerHandle handle{ 0 };
							handle.deserialize(buffer);
							auto localPlayer = util::player::g_networkHandle;
							if (auto rlGamerInfo = util::player::g_networkHandle->GetGamerInfo(); rlGamerInfo && rlGamerInfo->m_unk_gamer_handle.m_rockstar_id == handle.m_rockstar_id) {
								g_dxUiManager.sendNotify(xorstr_("Modder Detection"), xorstr_("{} is a modder (Reason: Attempted Kick | Reason Code: M[R]_LCTHK)"), player->GetName());
								if (auto togType = protections::g_blockRemovePlayerKick.toggleType; protections::g_blockRemovePlayerKick.toggle) {
									if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
										g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Kick (H0) from {}"), player->GetName());
									if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
										return true;
								}
							}
						} break;
						case rage::eNetMessage::CMsgNetComplaint: {
							//No need to check if we're host, it'll backfire regardless. It'll only false complain.
							if (!util::player::g_networkHandle->IsNetworkHost()) {
								if (auto net = util::network::get(); net) {
									auto complaintMgr = net->m_game_complaint_mgr;
									uint64_t peerAddress;
									buffer.ReadQword(&peerAddress, 64);
									uint32_t numberOfpeerAddresses{};
									buffer.ReadDword(&numberOfpeerAddresses, 32);
									if (numberOfpeerAddresses <= 64) {
										//We only need the first element, so we only need to grab one
										uint64_t arrayElement{};
										buffer.ReadQword(&arrayElement, 64);
										if (arrayElement != NULL) {
											if (auto togType = protections::g_blockRemovePlayerKick.toggleType; protections::g_blockRemovePlayerKick.toggle) {
												if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
													g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Kick (D0) from {}"), util::network::getCNetGamePlayerViaPeerAddress(peerAddress)->GetName());
												if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock) {
													complaintMgr.remove_complaint(arrayElement);
													buffer.Seek(0);
													return false;
												}
											}
										}
									}
									if (auto togType = protections::g_blockRaiseComplaintKick.toggleType; protections::g_blockRaiseComplaintKick.toggle) {
										int kickType{};
										if (complaintMgr.num_of_addresses_complainted() < 1)
											kickType = 1;
										if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
											g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Kick (D{}) from {}"), kickType, util::network::getCNetGamePlayerViaPeerAddress(peerAddress)->GetName());
										if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock) {
											complaintMgr.remove_all_complaints();
											buffer.Seek(0);
											return false;
										}
									}
								}
							}
						} break;
						case rage::eNetMessage::CMsgKickPlayer: {
							uint64_t sessionId;
							uint64_t peerId;
							buffer.ReadQword(&sessionId, 64);
							buffer.ReadQword(&peerId, 64);
							rage::eKickReason reason = buffer.Read<rage::eKickReason>(0x3);
							buffer.Read<int>(32);
							switch (reason) {
							case rage::eKickReason::VotedOut: {
								g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("You got kicked by the host. Host kicks cannot be rejoined due to the player blacklist."));
							} break;
							case rage::eKickReason::PeerComplaints: {
								g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("You got kicked by {}, rejoining the session."), player->GetName());
								pointers::g_joinBySessionInfo(util::network::get(), &util::network::get()->m_last_joined_session.m_session_info, 1, 1 | 2 | 4, nullptr, 0);
							} break;
							}
							return false;
						} break;
						case rage::eNetMessage::CMsgRequestObjectIds: {
							//TODO: Hook sendInvite and store last invite time, compare join time with last invite time to check if player is rid joining with sendInviteAcceptedPresenceEvent
							if (NETWORK::NETWORK_SESSION_IS_PRIVATE() && protections::g_blockPlayersFromRIDJoiningInPrivateSessions) {
								protections::g_arrayOfBlacklistedPlayers.push_back({ player, xorstr_("Invite-Only Session") });
							}
							if (NETWORK::NETWORK_SESSION_IS_SOLO() && protections::g_blockPlayersFromRIDJoiningInPrivateSessions) {
								bool isFriend{};
								static auto friendReg = pointers::g_friendRegistry;
								for (int i{}; i != friendReg->m_friend_count; i++) {
									if (auto fr = friendReg->get(i); fr && fr->m_name == player->GetName()) {
										isFriend = true;
									}
								}
								if (!isFriend)
									protections::g_arrayOfBlacklistedPlayers.push_back({ player, xorstr_("Solo Session") });
							}
							if (NETWORK::NETWORK_SESSION_IS_CLOSED_FRIENDS() && protections::g_blockPlayersFromRIDJoiningInPrivateSessions) {
								static auto friendReg = pointers::g_friendRegistry;
								for (int i{}; i != friendReg->m_friend_count; i++) {
									if (auto fr = friendReg->get(i); fr) {
										if (fr->m_rockstar_id == player->GetGamerInfo()->m_gamer_handle.m_rockstar_id) {
											if (fr->m_friend_state != 3 && fr->m_is_joinable || fr->m_friend_state != 1) {
												protections::g_arrayOfBlacklistedPlayers.push_back({ player, xorstr_("Spoofing As Friend") });
											}
										}
									}
								}
							}
							if (NETWORK::NETWORK_SESSION_IS_CLOSED_CREW() && protections::g_blockPlayersFromRIDJoiningInPrivateSessions) {
								if (player->m_clan_data.m_clan_id_2 != util::player::g_networkHandle->m_clan_data.m_clan_id_2) {
									protections::g_arrayOfBlacklistedPlayers.push_back({ player, "Spoofing As Crew Member" });
								}
							}
							for (auto& val : protections::g_arrayOfBlacklistedPlayers) {
								if (val.m_platformData == player->GetGamerInfo()->m_platform_data) {
									if (auto togType = protections::g_playerBlacklist.toggleType; protections::g_playerBlacklist.toggle) {
										if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
											g_dxUiManager.sendNotify(xorstr_("Blacklist"), xorstr_("Join from {} ({})"), player->GetName(), val.m_blacklistReason);
										if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
											return true;
									}
								}
							}
						} break;
						case rage::eNetMessage::CMsgTextMessage: {
							CMsgTextMessage msg;
							buffer.ReadString(msg.m_message, sizeof(msg.m_message));
							buffer.ReadPeerId(&msg.m_peer_id);
							buffer.ReadBool(&msg.m_is_team);
							auto words = getWords(msg.m_message);
							handleCmds(player->m_player_id, words);
							g_dxUiManager.sendNotify(xorstr_("Chat"), xorstr_("{} [{}]: {}"), util::localization::getGxtLabel(std::format("MP_CHAT_{}", msg.m_is_team ? xorstr_("TEAM") : xorstr_("ALL")).c_str()), player->GetName(), msg.m_message);
						} break;
						case rage::eNetMessage::CMsgTextMessage2: {
							CMsgTextMessage2 msg;
							buffer.ReadString(msg.m_message, sizeof(msg.m_message));
							buffer.ReadPeerId(&msg.m_peer_id);
							g_dxUiManager.sendNotify(xorstr_("Text"), xorstr_("{}: {}"), player->GetName(), msg.m_message);
						} break;
						}
					}
				}
			}
			catch (std::exception& ex) {

			}
		}
		RET_CALL(netConMgrChannelDispatchEvent, _this, pConMgr, pEvent)
	}
}