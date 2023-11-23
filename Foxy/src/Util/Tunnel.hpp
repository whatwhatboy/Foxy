#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/Rage.hpp"
#include "R.A.G.Engine/Network.hpp"
#include "R.A.G.Engine/BitBuffer.hpp"
#include "R.A.G.Engine/GamerHandle.hpp"
#include "R.A.G.Engine/Network.hpp"
#include "R.A.G.Engine/NetTunnel.hpp"
#include "Features/Features.hpp"
#include "Util/Util.hpp"
#include "Util/Packet.hpp"

namespace base {
	class tunnel {
	public:
		tunnel(uint64_t receiver) : m_receiver(receiver), m_handle(m_receiver) {}
		bool establish(uint32_t type, uint32_t reason) {
			if (!getSession()) {
				g_dxUiManager.sendNotify(xorstr_("Network Tunnel"), xorstr_("Failed to get {}'s session! The player is likely offline", m_handle.m_rockstar_id));
				return false;
			}
			rage::netTunnelDesc description{ type, reason };
			bool wasEtablished{};
			g_fiberPool.queue([&] {
				wasEtablished = pointers::g_openNetTunnel(util::network::session::getPointer()->m_net_connection_mgr, &m_sessionInfo.m_peer_info, &description, &m_request, &m_status);
				while (m_status.m_status_code == 1)
					fiber::cur()->wait(1ms);
			});
			return wasEtablished && m_status.m_status_code == 3;
		}
		bool getSession() {
			features::getSessionFromGamerHandle(m_handle, [&](rage::rlSessionByGamerTaskResult result, bool success) {
				m_establishedInitialConnection = success;
				m_sessionInfo = result.m_session_info;
			});
			return m_establishedInitialConnection;
		}
	public:
		void handleTunnel(uint32_t netMessageType) {
			switch (netMessageType) {
			case 0: {
				packet msg{ rage::eNetMessage::snMsgRemoveGamersFromSessionCmd };
				msg.Write<uint64_t>(util::network::session::getPointer()->m_rline_session.m_session_id, 0x40);
				auto netConnectionMgr = util::network::session::getPointer()->m_net_connection_mgr;
				auto netConnectionPeer = pointers::g_getConnectionPeer(netConnectionMgr, m_request.m_connection_identifier);
				rage::snMsgRemoveGamersFromSessionCmd cmd{ util::player::g_handle->m_gamer_info.m_gamer_handle.m_rockstar_id, { netConnectionPeer->m_peer_id }, 1 };
				msg.send(uint32_t(m_request.m_identifier));
			} break;
			case 1: {
				packet msg{ rage::eNetMessage::CMsgLostConnectionToHost };
				msg.Write<uint64_t>(util::network::session::getPointer()->m_rline_session.m_session_id, 0x40);
				msg.send(uint32_t(m_request.m_identifier));
			} break;
			case 2: {
				//CMsgNetComplaint
			} break;
			case 3: {
				packet msg{ rage::eNetMessage::CMsgTransitionLaunch };
				rage::rlGamerHandle invalidGamerHandle{ 10 };
				invalidGamerHandle.serialize(msg);
				msg.send(uint32_t(m_request.m_identifier));
			} break;
			}
		}
	public:
		uint64_t m_receiver{};
		rage::rlGamerHandle m_handle{};
		rage::rlSessionInfo m_sessionInfo{};
		rage::netTunnelRequest m_request{};
		rage::netStatus m_status{};
		bool m_establishedInitialConnection{};
	};
	inline void remoteKick(uint64_t rid, int32_t type, int32_t reason) {
		if (type == -1 && reason == -1) {
			for (int32_t i{}; i <= 5; i++) {
				remoteKick(rid, 0, i);
				remoteKick(rid, 1, i);
				fiber::cur()->wait();
			}
			return;
		}
		auto tun = std::make_unique<tunnel>(rid);
		if (!tun->establish(type, reason)) {
			g_dxUiManager.sendNotify(xorstr_("Network Tunnel"), xorstr_("Failed to establish connection to the tunnel, closing now..."));
			tun.reset();
			return;
		}
		rage::snMsgRemoveGamersFromSessionCmd cmd{};
		auto netConnectionMgr = util::network::session::getPointer()->m_net_connection_mgr;
		auto netConnectionPeer = pointers::g_getConnectionPeer(netConnectionMgr, tun->m_request.m_connection_identifier);
		cmd.m_session_id = util::player::g_handle->m_gamer_info.m_gamer_handle.m_rockstar_id;
		cmd.m_handles[0].m_rockstar_id = netConnectionPeer->m_peer_id;
		cmd.m_num_handles = 1;
	}
}