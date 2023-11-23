#pragma once
#include "Pch/Common.hpp"
#include "SessionInfo.hpp"
#include "Util/Util.hpp"

class CGameInvite {
public:
    virtual ~CGameInvite() = default; //0x0008 (0)
    virtual void unk_0008() = 0; //0x0008 (1)
    virtual char* Identifier() = 0; //0x0010 (2)
    virtual bool FormatPresenceEvent(void* table, uint64_t unk2) = 0; //0x0018 (3)
    virtual bool unk_0020(void* unk, uint64_t unk2) = 0; //0x0020 (4)
    virtual void SendInvite(char* inviteKey) = 0; //0x0028 (5)
public:
    void Construct(rage::rlGamerHandle localHandle, rage::rlSessionInfo info) {
        h = localHandle.m_rockstar_id;
        h2 = localHandle.m_platform;
        if_ = -1;
        it = -1;
        f = 0xA000;
        gm = 0;
        cc = localHandle.m_rockstar_id;
        cc2 = localHandle.m_platform;
        l = 0;
        p = 0;
        cr = 0;
        u = 0;
        d = 889455970;
        jq = 0;
        strcpy(s, info.encode());
        strcpy(c, "");
        strcpy(mid, "");
        strcpy(n, base::util::localization::createRandomString(10).c_str());
        memcpy(&session, &info, sizeof(info));
    }
public:
    rage::rlSessionInfo session; //0x0008
    uint64_t h; //0x0078
    uint64_t h2; //0x0080
    char s[125]; //0x0088
    char n[17]; //0x00D5
    char unk_00C6[2]; //0x00E6
    uint32_t gm; //0x00E8
    char c[23]; //0x00EC
    char unk_00E3[5]; //0x0103
    uint64_t cc; //0x0108
    uint64_t cc2; //0x0110
    char mid[30]; //0x0118
    char unk_0116[2]; //0x0136
    uint32_t if_; //0x0138
    uint32_t it; //0x013C
    uint32_t l; //0x0140
    uint32_t p; //0x0144
    uint32_t f; //0x0148
    uint32_t cr; //0x014C
    uint32_t u; //0x0150
    uint32_t d; //0x0154
    uint32_t jq; //0x0158
}; //Size: 0x015C