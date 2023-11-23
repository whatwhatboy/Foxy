#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/Rage.hpp"
#include "R.A.G.Engine/Network.hpp"
#include "R.A.G.Engine/BitBuffer.hpp"

namespace base {
	class packet : public rage::datBitBuffer {
	private:
		char m_bufferData[916]{};
	public:
		rage::eNetMessage m_msg{};
	public:
		packet(rage::eNetMessage msg, bool serialiseMsg = true, bool zeroMem = true) : rage::datBitBuffer(m_bufferData, sizeof(m_bufferData), true), m_msg(msg) {
			if (zeroMem)
				ZeroMemory(m_bufferData, sizeof(m_bufferData));
			if (serialiseMsg)
				writeMsg();
		}
		packet() : m_msg(rage::eNetMessage::MsgInvalid) {}
	public:
		void writeMsg() {
			//Write the cursor
			if (m_msg != rage::eNetMessage::MsgInvalid) {
				Write<int>(0x3246, 14);
				if (int(m_msg) > 0xFF) {
					Write<bool>(true, 1);
					Write<rage::eNetMessage>(m_msg, 16);
				}
				else {
					Write<bool>(false, 1);
					Write<rage::eNetMessage>(m_msg, 8);
				}
			}
		}
	public:
		bool send(uint32_t msgId) {
			return pointers::g_queuePacketViaMsgId(util::network::session::getPointer()->m_net_connection_mgr, msgId, m_bufferData, GetMaxPossibleBit(), 1, nullptr);
		}
		bool send(CNetGamePlayer* player) {
			return pointers::g_queuePacketWithPlayer(player, m_bufferData, GetMaxPossibleBit(), 1, nullptr, nullptr);
		}
		void send(uint64_t peerId, int connectionId) {
			if (auto netConnectionMgr = util::network::session::getPointer()->m_net_connection_mgr; netConnectionMgr) {
				auto netConnectionPeer = pointers::g_getConnectionPeer(netConnectionMgr, peerId);
				pointers::g_sendPacketViaConId(netConnectionMgr, netConnectionPeer, connectionId, m_bufferData, GetMaxPossibleBit(), 0x1000000);
			}
		}
	};
}