#pragma once
#include "Pch/Common.hpp"
#include "Memory/Patterns/Pointers.hpp"
#define LODWORD(x)  (*((DWORD*)&(x)))
#define HIDWORD(x)  (*((DWORD*)&(x)+1))
#define SLODWORD(x) (*((int32_t*)&(x)))
#define SHIDWORD(x) (*((int32_t*)&(x)+1))

namespace rage {
	class datBitBuffer;
	class CSyncDataBase {
	public:
		virtual ~CSyncDataBase() = default;
		CSyncDataBase() {
			unk_0008 = NULL;
			m_net_object = nullptr;
			m_buffer = nullptr;
		}

		int64_t unk_0008; //0x0008
		class netObject* m_net_object; //0x0010
		class datBitBuffer* m_buffer; //0x0018
	}; //Size: 0x0020
	static_assert(sizeof(CSyncDataBase) == 0x20);
	class CSyncDataReader : public CSyncDataBase {
	public:
		CSyncDataReader() : CSyncDataBase() {}
		virtual ~CSyncDataReader() = default; //0x0000 (0)
		virtual bool ReadDword(uint32_t* read, uint32_t bits) = 0; //0x0008 (1)
		virtual bool ReadWord(uint16_t* read, uint32_t bits) = 0; //0x0010 (2)
		virtual bool ReadByte(uint8_t* read, uint32_t bits) = 0; //0x0018 (3)
		virtual bool ReadInt32(int32_t* read, uint32_t bits) = 0; //0x0020 (4)
		virtual bool ReadInt16(int16_t* read, uint32_t bits) = 0; //0x0028 (5)
		virtual bool ReadInt8(int8_t* read, uint32_t bits) = 0; //0x0030 (6)
		virtual bool ReadBool(bool* read) = 0; //0x0038 (7)
		virtual bool ReadInt64(int64_t* read, uint32_t bits) = 0; //0x0040 (8)
		virtual bool ReadInt32_2(int32_t* read, uint32_t bits) = 0; //0x0048 (9)
		virtual bool ReadInt16_2(int16_t* read, uint32_t bits) = 0; //0x0050 (10)
		virtual bool ReadInt8_2(int8_t* read, uint32_t bits) = 0; //0x0058 (11)
		virtual bool ReadQword(uint64_t* read, uint32_t bits) = 0; //0x0060 (12)
		virtual bool ReadDword_2(uint32_t* read, uint32_t bits) = 0; //0x0068 (13)
		virtual bool ReadWord_2(uint16_t* read, uint32_t bits) = 0; //0x0070 (14)
		virtual bool ReadByte_2(uint8_t* read, uint32_t bits) = 0; //0x0078 (15)
		virtual bool ReadSignedFloat(float* read, float divisior, uint32_t bits) = 0; //0x0080 (16)
		virtual bool ReadFloat(float* read, float divisior, uint32_t bits) = 0; //0x0088 (17)
		virtual bool ReadNetObjectId(uint16_t* read) = 0; //0x0090 (18)
		virtual bool ReadFloat3(float* read, float* divisior, uint32_t bits) = 0; //0x0098 (19)
		virtual bool ReadFloat11(float* read) = 0; //0x00A0 (20)
		virtual bool ReadFloat3_2(float* read, float* divisior, uint32_t bits) = 0; //0x00A8 (21)
		virtual bool ReadFloat4(float* read, uint32_t bits) = 0; //0x00B0 (22)
		virtual bool ReadArray(void* read, uint32_t bits) = 0; //0x00B8 (23)
		virtual bool ReadArray_2(void* read, uint32_t bits) = 0; //0x00C0 (24)
		virtual bool IsSizeCalculator() = 0; //0x00C8 (25)
		virtual uint32_t GetBitsRead() = 0; //0x00D0 (26)
		auto SetBitBuffer(datBitBuffer* buffer) {
			m_buffer = buffer;
			return this;
		}
	}; //Size: 0x0020
	template <typename t>
	using functionalUniquePtr = std::unique_ptr<t, std::function<void(t*)>>;
	class CSyncDataWriter : public CSyncDataBase {
	public:
		CSyncDataWriter() : CSyncDataBase() {}
		virtual ~CSyncDataWriter() = default; //0x0000 (0)
		virtual bool WriteDword(uint32_t* write, uint32_t bits) = 0; //0x0008 (1)
		virtual bool WriteWord(uint16_t* write, uint32_t bits) = 0; //0x0010 (2)
		virtual bool WriteByte(uint8_t* write, uint32_t bits) = 0; //0x0018 (3)
		virtual bool WriteInt32(int32_t* write, uint32_t bits) = 0; //0x0020 (4)
		virtual bool WriteInt16(int16_t* write, uint32_t bits) = 0; //0x0028 (5)
		virtual bool WriteInt8(int8_t* write, uint32_t bits) = 0; //0x0030 (6)
		virtual bool WriteBool(bool* write) = 0; //0x0038 (7)
		virtual bool WriteInt64(int64_t* write, uint32_t bits) = 0; //0x0040 (8)
		virtual bool WriteInt32_2(int32_t* write, uint32_t bits) = 0; //0x0048 (9)
		virtual bool WriteInt16_2(int16_t* write, uint32_t bits) = 0; //0x0050 (10)
		virtual bool WriteInt8_2(int8_t* write, uint32_t bits) = 0; //0x0058 (11)
		virtual bool WriteQword(uint64_t* write, uint32_t bits) = 0; //0x0060 (12)
		virtual bool WriteDword_2(uint32_t* write, uint32_t bits) = 0; //0x0068 (13)
		virtual bool WriteWord_2(uint16_t* write, uint32_t bits) = 0; //0x0070 (14)
		virtual bool WriteByte_2(uint8_t* write, uint32_t bits) = 0; //0x0078 (15)
		virtual bool WriteSignedFloat(float* write, float divisior, uint32_t bits) = 0; //0x0080 (16)
		virtual bool WriteFloat(float* write, float divisior, uint32_t bits) = 0; //0x0088 (17)
		virtual bool WriteNetObjectId(uint16_t* write) = 0; //0x0090 (18)
		virtual bool WriteFloat3(float* write, float* divisior, uint32_t bits) = 0; //0x0098 (19)
		virtual bool WriteFloat11(float* write) = 0; //0x00A0 (20)
		virtual bool WriteFloat3_2(float* write, float* divisior, uint32_t bits) = 0; //0x00A8 (21)
		virtual bool WriteFloat4(float* write, uint32_t bits) = 0; //0x00B0 (22)
		virtual bool WriteArray(void* write, uint32_t bits) = 0; //0x00B8 (23)
		virtual bool WriteArray_2(void* write, uint32_t bits) = 0; //0x00C0 (24)
		virtual bool IsSizeCalculator() = 0; //0x00C8 (25)
		virtual uint32_t GetBitsWritten() = 0; //0x00D0 (26)
		auto SetBitBuffer(datBitBuffer* buffer) {
			m_buffer = buffer;
			return this;
		}
	}; //Size: 0x0020

	class datBitBuffer {
	public:
		datBitBuffer(u8* data, u32 size, bool flagBitsToWrite = false) {
			m_data = data;
			m_bitOffset = 0;
			m_maxBit = size * 8;
			m_bitsRead = 0;
			m_curBit = 0;
			m_highestBitsRead = 0;
			m_flagBits = 0;
			if (flagBitsToWrite) {
				m_flagBits &= 0xFCu;
				m_flagBits &= 0xFDu;
				m_flagBits |= 0xFCu;
			}
			else {
				m_flagBits &= ~2u;
				m_flagBits |= 1u;
			}
		}
		datBitBuffer(void* data, u32 size, bool flagBitsToWrite = false) : datBitBuffer((u8*)data, size, flagBitsToWrite) {}
		datBitBuffer(bool flagBitsToWrite = false) : datBitBuffer((u8*)nullptr, NULL, flagBitsToWrite) {}

		bool IsFlagSet(u8 flag) {
			return (m_flagBits & flag) != NULL;
		}
		u32 GetPosition() {
			return m_bitsRead;
		}
		u32 GetMaxPossibleBit() {
			return (m_highestBitsRead + 7) >> 3;
		}
		bool Seek(u32 bits) {
			if (bits >= 0) {
				u32 length{ IsFlagSet(1) ? m_maxBit : m_curBit };
				if (bits <= length) {
					m_bitsRead = bits;
					return true;
				}
			}
			return false;
		}
		s64 SeekCur(u32 bits) {
			m_bitsRead += static_cast<u32>(bits);
			if (m_bitsRead > m_curBit)
				m_curBit = m_bitsRead;
			return m_bitsRead;
		}
		s64 AddNumberOfBits(u32 bits) {
			m_bitsRead += static_cast<u32>(bits);
			if (m_bitsRead > m_highestBitsRead)
				m_highestBitsRead = m_bitsRead;
			return m_bitsRead;
		}
		u32 GetDataLength() {
			u32 leftoverBit{ (m_curBit % 8u) ? 1u : 0u };
			return (m_curBit / 8u) + leftoverBit;
		}
		u32 GetMaxDataLength() {
			return (m_flagBits & 1) != NULL ? m_maxBit : m_curBit;
		}
		bool EnsureBitData(u32 bits) {
			if (IsFlagSet(2) || m_bitsRead + bits > GetMaxDataLength())
				return false;
			return true;
		}
		u64 WriteBitsSingle(u32 value, s32 bits) {
			return base::pointers::g_writeBitsSingle(m_data, value, bits, m_bitsRead + m_bitOffset);
		}
		u32 ReadBitsSingle(u32 numBits) {
			u32 result{};
			base::pointers::g_readBitsSingle(m_data, &result, numBits, m_bitsRead + m_bitOffset);
			return result;
		}
		template <typename t>
		bool Write(t value, u32 bits = 0) {
			uint32_t bitsToWrite{ bits ? bits : sizeof(t) };
			bool res{ WriteBitsSingle((u32)value, bitsToWrite) ? true : false };
			SeekCur(bitsToWrite);
			return res;
		}
		template <typename t>
		void WriteSigned(t value, s32 bits) {
			s32 sign{ value < 0 };
			u32 signEx{ sign ? 0xFFFFFFFF : 0 };
			u32 d{ value ^ signEx };
			Write<s32>(1, sign);
			Write<s32>(bits - 1, d);
		}
		void WriteFloat(s32 length, float divisor, float value) {
			s32 max{ (1 << length) - 1 };
			s32 integer{ (s32)((value / divisor) * max) };
			Write<s32>(length, integer);
		}
		void WriteSignedFloat(s32 length, float divisor, float value) {
			s32 max{ (1 << (length - 1)) - 1 };
			s32 integer{ (s32)((value / divisor) * max) };
			WriteSigned<s32>(length, integer);
		}
		template <typename t>
		t Read(u32 bits) {
			if (bits <= 32) {
				return t(ReadBitsSingle(bits));
			}
			else {
				u32 Low{ ReadBitsSingle(bits - 32u) };
				u32 High{ ReadBitsSingle(32u) };
				return t(High | ((u64)Low << 32));
			}
		}
		template <typename t>
		t ReadSigned(u32 bits) {
			s32 sign{ Read<s32>(1) };
			s32 data{ Read<s32>(bits - 1) };
			return t((s64)sign + (data ^ -sign));
		}
		float ReadFloat(s32 length, float divisor) {
			s32 integer{ Read<s32>(length) };
			float max = (1 << length) - 1;
			return ((float)integer / max) * divisor;
		}
		float ReadSignedFloat(s32 length, float divisor) {
			s32 integer{ ReadSigned<s32>(length) };
			auto max = (1 << (length - 1)) - 1;
			return ((float)integer / max) * divisor;
		}
		bool ReadPeerId(u64* value) {
			if (!EnsureBitData(0x20))
				return false;
			*value = Read<u64>(0x20);
			return true;
		}
		bool WriteString(char* string, u32 length) {
			bool extended{ Write<bool>(length > m_maxBit ? true : false, 1) };
			u32 len{ Write<u32>(extended ? 15 : 7) };
			if (len > length ? true : false)
				return false;
			WriteArray(string, len * 8);
			return true;
		}
		template <u32 length>
		bool WriteString(char(&string)[length]) {
			return WriteString(string, length);
		}
		bool ReadString(char* string, u32 length) {
			bool extended{ Read<bool>(1) };
			u32 len{ Write<u32>(extended ? 15 : 7) };
			if (len > length ? true : false)
				return false;
			ReadArray(string, len * 8);
			if (string[len - 1] != '\0')
				return false;
			return true;
		}
		template <u32 length>
		bool ReadString(char(&string)[length]) {
			return ReadString(string, length);
		}
		bool ReadBool(bool* boolean) {
			if (!EnsureBitData(1))
				return false;
			*boolean = Read<uint8_t>(1);
			return true;
		}
		bool WriteBool(bool value) {
			return Write<bool>(value);
		}
		bool ReadByte(u8* integer, int bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<u8>(bits);
			return true;
		}
		bool WriteByte(u8 value, int bits) {
			return Write<u8>(value, bits);
		}
		bool ReadWord(u16* integer, int bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<u16>(bits);
			return true;
		}
		bool WriteWord(u16 value, int bits) {
			return Write<u16>(value, bits);
		}
		bool ReadDword(u32* integer, int bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<u32>(bits);
			return true;
		}
		bool WriteDword(uint32_t value, int bits) {
			return Write<uint32_t>(value, bits);
		}
		bool ReadInt32(s32* integer, int bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = ReadSigned<s32>(bits);
			return true;
		}
		bool ReadQword(u64* integer, int bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<u64>(bits);
			return true;
		}
		bool WriteQword(u64 value, int bits) {
			if (bits <= 32) {
				if (IsFlagSet(1))
					return false;
				if ((m_bitsRead + bits) <= m_maxBit) {
					if (!IsFlagSet(2)) {
						Write<u64>(value, bits);
					}
					return true;
				}
				return false;
			}
			else {
				if (IsFlagSet(1))
					return false;
				if ((m_bitsRead + 32) > m_maxBit)
					return false;
				if (!IsFlagSet(2)) {
					Write<u64>(value, 32);
				}
				if (IsFlagSet(1) || (bits - 32) + m_bitsRead > m_maxBit) {
					return false;
				}
				else {
					if (!IsFlagSet(2)) {
						Write<u32>(SHIDWORD(value), bits - 32);
					}
				}
			}
			return true;
		}
		bool ReadInt64(s64* integer, s32 bits) {
			if (!EnsureBitData(bits))
				return false;
			*integer = ReadSigned<s64>(bits);
			return true;
		}
		bool WriteArray(void* array, s32 size) {
			return base::pointers::g_writeBitbufArray(this, array, size, 0);
		}
		bool ReadArray(void* array, s32 size) {
			return base::pointers::g_readBitbufArray(this, array, size, 0);
		}
	public:
		u8* m_data; //0x0000
		u32 m_bitOffset; //0x0008
		u32 m_maxBit; //0x000C
		u32 m_bitsRead; //0x0010
		u32 m_curBit; //0x0014
		u32 m_highestBitsRead; //0x0018
		u8 m_flagBits; //0x001C
	}; //Size: 0x0020
	static_assert(sizeof(datBitBuffer) == 0x20);
}
#pragma pack(push, 1)
class CPackedMessageData {
public:
	CPackedMessageData(void* buffer, bool nullifyData = true) {
		if (nullifyData) {
			m_count = NULL;
			m_type = NULL;
			memset(m_data_buffer, NULL, sizeof(m_data_buffer));
		}
		if (m_data_buffer != buffer)
			memcpy(m_data_buffer, buffer, sizeof(m_data_buffer));
		m_buffer = rage::datBitBuffer(m_data_buffer, sizeof(m_data_buffer));
		m_buffer.Seek(0);
	}
	CPackedMessageData(bool nullifyData = true) : CPackedMessageData(m_data_buffer, nullifyData) {}
	uint32_t m_count; //0x0000
	uint32_t m_type; //0x0004
	rage::datBitBuffer m_buffer; //0x0008
	uint8_t m_data_buffer[912]; //0x0028

	bool Serialize(rage::datBitBuffer* buffer) {
		m_count = buffer->Read<uint32_t>(5);
		if (m_count > 0) {
			auto bufferSize{ buffer->Read<uint32_t>(13) };
			char data[4096]{};
			if (!buffer->ReadArray(data, bufferSize)) {
				return false;
			}
			m_buffer = rage::datBitBuffer(data, bufferSize);
			m_buffer.m_maxBit = bufferSize;
		}
		return true;
	}
}; //Size: 0x03B8
static_assert(sizeof(CPackedMessageData) == 0x3B8);
#pragma pack(pop)
#pragma pack(push, 1)
class CMsgCloneSync {
public:
	CMsgCloneSync(bool nullifyData = true) : m_data(CPackedMessageData(nullifyData)) {}
	CNetGamePlayer* m_owner; //0x0000
	uint32_t m_timestamp; //0x0008
	uint16_t m_net_sequence; //0x000C
	uint8_t m_num_objects; //0x000E
	uint8_t unk_000F; //0x000F
	CPackedMessageData m_data; //0x0010

	bool Serialize(rage::datBitBuffer* buffer) {
		m_net_sequence = buffer->Read<uint16_t>(16);
		m_timestamp = buffer->Read<uint32_t>(13);
		m_data.Serialize(buffer);
		return true;
	}
}; //Size: 0x03C8
static_assert(sizeof(CMsgCloneSync) == 0x3C8);
class CMsgPackedReliables {
public:
	CMsgPackedReliables() :
		m_creates(CPackedMessageData()), m_create_acks(CPackedMessageData()), m_removes(CPackedMessageData()), m_remove_acks(CPackedMessageData())
	{}
	CNetGamePlayer* m_owner; //0x0000
	uint32_t m_timestamp; //0x0008
	uint32_t unk_000C; //0x000C
	CPackedMessageData m_creates; //0x0010
	CPackedMessageData m_create_acks; //0x03C8
	CPackedMessageData m_removes; //0x0780
	CPackedMessageData m_remove_acks; //0x0B38

	uint32_t FlagBits() {
		uint32_t flags{ m_creates.m_count };
		if (m_create_acks.m_count)
			flags |= 2;
		if (m_removes.m_count)
			flags |= 4;
		if (m_remove_acks.m_count)
			flags |= 8;
		return flags;
	}
	bool Serialize(rage::datBitBuffer* buffer) {
		uint32_t flags{ FlagBits() };
		flags = buffer->Read<uint32_t>(4);
		if (flags & 1) {
			m_timestamp = buffer->Read<uint32_t>(0x20);
			m_creates.Serialize(buffer);
		}
		if (flags & 2)
			m_create_acks.Serialize(buffer);
		if (flags & 4)
			m_removes.Serialize(buffer);
		if (flags & 8)
			m_remove_acks.Serialize(buffer);
		return true;
	}
}; //Size: 0x0EF0
static_assert(sizeof(CMsgPackedReliables) == 0x0EF0);
#pragma pack(pop)
#pragma pack(push, 1)
class CMsgPackedEvents {
public:
	CMsgPackedEvents(bool nullifyBuffer = true) : m_data(CPackedMessageData()) {}
	CNetGamePlayer* m_owner; //0x0000
	CPackedMessageData m_data; //0x0018
	bool Serialize(rage::datBitBuffer* buffer) {
		return m_data.Serialize(buffer);
	}
}; //Size: 0x03C0
static_assert(sizeof(CMsgPackedEvents) == 0x3C0);
#pragma pack(pop)