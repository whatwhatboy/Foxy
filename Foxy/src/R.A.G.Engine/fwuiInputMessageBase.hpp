#pragma once
#include <cstdint>

namespace rage {
    class fwuiInputMessageBase {
    public:
        uint32_t m_timer_one; //0x0000
        uint32_t m_timer_two; //0x0004
        uint32_t m_char_count; //0x0008
        uint32_t m_key_held_time; //0x000C
        uint32_t m_team_label; //0x0010
        uint8_t m_chat_open; //0x0014
        uint8_t m_is_job; //0x0015
        uint8_t m_disabled; //0x0016
        uint8_t m_not_typing; //0x0017
        uint32_t m_focus_mode; //0x0018
        uint32_t m_chat_mode; //0x001C
        uint32_t m_scaleform; //0x0020
        char pad_0024[8]; //0x0024
        char16_t m_current_text[142]; //0x002C
        uint32_t m_hud_color; //0x0148
        uint8_t m_hud_color_override; //0x014C
        char pad_014D[43]; //0x014D

        void Send(const char* value, int characterCount, fnptr<bool(const char*, int, char16_t*, uint8_t)> onDataSet, bool restoreOrginialMessageData = false, bool restore = true) {
            auto originalValue_CharCount = m_char_count;
            auto originalValue_ChatOpen = m_chat_open;
            auto originalValue_IsJob = m_is_job;
            auto originalValue_Disabled = m_disabled;
            auto originalValue_NotTyping = m_not_typing;
            auto originalValue_CurrentText = m_current_text;
            m_char_count = characterCount;
            m_chat_open = TRUE;
            m_is_job = FALSE;
            m_disabled = FALSE;
            m_not_typing = TRUE;
            RtlZeroMemory(m_current_text, sizeof(m_current_text));
            u16strcpy(m_current_text, value, sizeof(m_current_text));
            if (onDataSet(value, characterCount, m_current_text, originalValue_CharCount) && restore) {
                m_char_count = restoreOrginialMessageData ? originalValue_CharCount : m_char_count;
                m_chat_open = restoreOrginialMessageData ? originalValue_ChatOpen : m_chat_open;
                m_is_job = originalValue_IsJob;
                m_disabled = originalValue_Disabled;
                m_not_typing = originalValue_NotTyping;
                if (restoreOrginialMessageData) {
                    RtlZeroMemory(m_current_text, sizeof(m_current_text));
                    memcpy(m_current_text, originalValue_CurrentText, m_char_count);
                }
            }
        }
    }; //Size: 0x0178
    static_assert(sizeof(fwuiInputMessageBase) == 0x178);
}