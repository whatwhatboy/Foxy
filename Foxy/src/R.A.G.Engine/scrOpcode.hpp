#pragma once
//		code, string, debug
#define OP(c, s, d) OP_##c

enum scrOpcode {
	// b=8 bit byte, s=signed 16 bit short, h=unsigned 16 bit, a=24 bit addr, d=unsigned 32 bit, f=float, $=counted string
	// No operation (zero)
	OP(NOP, 1, ""),		// 0

	// Integer Arithmetic / Comparison
	OP(IADD, 1, ""), 		// 1
	OP(ISUB, 1, ""),		// 2
	OP(IMUL, 1, ""),		// 3
	OP(IDIV, 1, ""),		// 4
	OP(IMOD, 1, ""),		// 5
	OP(INOT, 1, ""),		// 6
	OP(INEG, 1, ""),		// 7

	OP(IEQ, 1, ""),		// 8
	OP(INE, 1, ""),		// 9
	OP(IGT, 1, ""),		// 10
	OP(IGE, 1, ""),		// 11
	OP(ILT, 1, ""),		// 12
	OP(ILE, 1, ""),		// 13

	// Float Arithmetic / Comparison
	OP(FADD, 1, ""),		// 14
	OP(FSUB, 1, ""),		// 15
	OP(FMUL, 1, ""),		// 16
	OP(FDIV, 1, ""),		// 17
	OP(FMOD, 1, ""),		// 18
	OP(FNEG, 1, ""),		// 19

	OP(FEQ, 1, ""),		// 20
	OP(FNE, 1, ""),		// 21
	OP(FGT, 1, ""),		// 22
	OP(FGE, 1, ""),		// 23
	OP(FLT, 1, ""),		// 24
	OP(FLE, 1, ""),		// 25

	// Vector arithmetic
	OP(VADD, 1, ""),		// 26
	OP(VSUB, 1, ""),		// 27
	OP(VMUL, 1, ""),		// 28
	OP(VDIV, 1, ""),		// 29
	OP(VNEG, 1, ""),		// 30

	// Bitwise arithmetic
	OP(IAND, 1, ""),		// 31
	OP(IOR, 1, ""),		// 32
	OP(IXOR, 1, ""),		// 33

	// Conversion
	OP(I2F, 1, ""),		// 34
	OP(F2I, 1, ""),		// 35
	OP(F2V, 1, ""),		// 36

	// Stack
	OP(PUSH_CONST_U8, 2, "b"),	// 37 - Push unsigned byte constant
	OP(PUSH_CONST_U8_U8, 3, "bb"),// 38
	OP(PUSH_CONST_U8_U8_U8, 4, "bbb"),// 39
	OP(PUSH_CONST_U32, 5, "d"),	// 40 - Push unsigned integer constant
	OP(PUSH_CONST_F, 5, "f"),		// 41 - Push floating-point constant
	OP(DUP, 1, ""),				// 42 - Duplicate top-of-stack
	OP(DROP, 1, ""),				// 43 - Throw out top-of-stack
	OP(NATIVE, 4, "bbb"),			// 44 - Call a native (C++) function (6:2,8,8 split)
	OP(ENTER, 0, "bs$"),			// 45 - Allocate stack frame
	OP(LEAVE, 3, "bb"),			// 46 - Deallocate stack frame and return

	OP(LOAD, 1, ""),			// 47 - Pop address from stack, push value at that address to stack
	OP(STORE, 1, ""),			// 48 - Pop address and value from stack, store value at specified address
	OP(STORE_REV, 1, ""),		// 49 - Pop value and address from stack, store value at specified address, leave address on stack (for array construction)

	OP(LOAD_N, 1, ""),			// 50 - Pop address and count from stack, push 'count' values to stack
	OP(STORE_N, 1, ""),			// 51 - Pop address, count, and values from stack, store them at specified address

	// The _LOAD version must be after the base version, and the _STORE version right after the _LOAD version:
	// (even below, where they're also grouped by immediate operand size)
	// Runtime range check and index lookup (for arrays)
	OP(ARRAY_U8, 2, "b"),			// 52
	OP(ARRAY_U8_LOAD, 2, "b"),	// 53
	OP(ARRAY_U8_STORE, 2, "b"),	// 54

	OP(LOCAL_U8, 2, "b"),			// 55
	OP(LOCAL_U8_LOAD, 2, "b"),	// 56
	OP(LOCAL_U8_STORE, 2, "b"),	// 57

	OP(STATIC_U8, 2, "b"),		// 58
	OP(STATIC_U8_LOAD, 2, "b"),	// 59
	OP(STATIC_U8_STORE, 2, "b"),	// 60

	OP(IADD_U8, 2, "b"),			// 61
	OP(IMUL_U8, 2, "b"),			// 62

	OP(IOFFSET, 1, ""),					// 63
	OP(IOFFSET_U8, 2, "b"),				// 64
	OP(IOFFSET_U8_LOAD, 2, "b"),			// 65
	OP(IOFFSET_U8_STORE, 2, "b"),			// 66

	// BEGIN OPCODES WITH 16BIT IMMEDIATES; DO NOT MOVE (expected by scrThread::SwapForPlatform)
	OP(PUSH_CONST_S16, 3, "s"),	// 67 - Push signed integer constant
	OP(IADD_S16, 3, "s"),			// 68
	OP(IMUL_S16, 3, "s"),			// 69

	OP(IOFFSET_S16, 3, "s"),				// 70
	OP(IOFFSET_S16_LOAD, 3, "s"),			// 71
	OP(IOFFSET_S16_STORE, 3, "s"),		// 72

	OP(ARRAY_U16, 3, "h"),		// 73
	OP(ARRAY_U16_LOAD, 3, "h"),	// 74
	OP(ARRAY_U16_STORE, 3, "h"),	// 75

	OP(LOCAL_U16, 3, "h"),		// 76
	OP(LOCAL_U16_LOAD, 3, "h"),	// 77
	OP(LOCAL_U16_STORE, 3, "h"),	// 78

	OP(STATIC_U16, 3, "h"),		// 79
	OP(STATIC_U16_LOAD, 3, "h"),	// 80
	OP(STATIC_U16_STORE, 3, "h"),	// 81

	OP(GLOBAL_U16, 3, "h"),		// 82
	OP(GLOBAL_U16_LOAD, 3, "h"),	// 83
	OP(GLOBAL_U16_STORE, 3, "h"),	// 84

	// Branch; Target is relative signed address (from the start of the NEXT insn)
	OP(J, 3, "R"),			// 85
	OP(JZ, 3, "R"),			// 86
	OP(IEQ_JZ, 3, "R"),		// 87
	OP(INE_JZ, 3, "R"),		// 88
	OP(IGT_JZ, 3, "R"),		// 89
	OP(IGE_JZ, 3, "R"),		// 90
	OP(ILT_JZ, 3, "R"),		// 91
	OP(ILE_JZ, 3, "R"),		// 92
	// END OPCODES WITH 16BIT IMMEDIATES; DO NOT MOVE (expected by scrThread::SwapForPlatform)

	// BEGIN OPCODES WITH 24BIT IMMEDIATES; DO NOT MOVE (expected by scrThread::SwapForPlatform)
	OP(CALL, 4, "a"),				// 93
	OP(STATIC_U24, 4, "a"),		// 94
	OP(STATIC_U24_LOAD, 4, "a"),	// 95
	OP(STATIC_U24_STORE, 4, "a"),	// 96
	OP(GLOBAL_U24, 4, "a"),		// 97 - upper 6 bits are the block index
	OP(GLOBAL_U24_LOAD, 4, "a"),	// 98 - upper 6 bits are the block index
	OP(GLOBAL_U24_STORE, 4, "a"),	// 99 - upper 6 bits are the block index
	OP(PUSH_CONST_U24, 4, "a"),	// 100
	// END OPCODES WITH 24BIT IMMEDIATES; DO NOT MOVE (expected by scrThread::SwapForPlatform)

	// Switch statement; pops int off the stack and uses it to look up a match in the following table
	// The table starts with a u8 for the number of labels, followed by a s32, s16 pair for each match
	// value and PC to jump to.	 The compiler emits a jump instruction immediately after the switch
	// that either goes to the DEFAULT label or the end of the switch statement.  (Disassembler assumes
	// there are at least two cases for every switch).  Could use u16 for offset without too much trouble.
	OP(SWITCH, 0, "S"),		// 101

	// Convert TOS to address relative to string heap
	OP(STRING, 1, ""),		// 102

	OP(STRINGHASH, 1, ""),	// 103

	// Top of stack is the address of a text label, next is the address of a string to assign to it
	OP(TEXT_LABEL_ASSIGN_STRING, 2, "b"),	// 104
	OP(TEXT_LABEL_ASSIGN_INT, 2, "b"),	// 105
	OP(TEXT_LABEL_APPEND_STRING, 2, "b"),	// 106
	OP(TEXT_LABEL_APPEND_INT, 2, "b"),	// 107
	OP(TEXT_LABEL_COPY, 1, ""),			// 108

	OP(CATCH, 1, ""),						// 109
	OP(THROW, 1, ""),						// 110

	OP(CALLINDIRECT, 1, ""),				// 111

	OP(PUSH_CONST_M1, 1, ""),				// 112
	OP(PUSH_CONST_0, 1, ""),				// 113
	OP(PUSH_CONST_1, 1, ""),				// 114
	OP(PUSH_CONST_2, 1, ""),				// 115
	OP(PUSH_CONST_3, 1, ""),				// 116
	OP(PUSH_CONST_4, 1, ""),				// 117
	OP(PUSH_CONST_5, 1, ""),				// 118
	OP(PUSH_CONST_6, 1, ""),				// 119
	OP(PUSH_CONST_7, 1, ""),				// 120

	OP(PUSH_CONST_FM1, 1, ""),			// 121
	OP(PUSH_CONST_F0, 1, ""),				// 122
	OP(PUSH_CONST_F1, 1, ""),				// 123
	OP(PUSH_CONST_F2, 1, ""),				// 124
	OP(PUSH_CONST_F3, 1, ""),				// 125
	OP(PUSH_CONST_F4, 1, ""),				// 126
	OP(PUSH_CONST_F5, 1, ""),				// 127
	OP(PUSH_CONST_F6, 1, ""),				// 128
	OP(PUSH_CONST_F7, 1, ""),				// 129

	OP(IS_BIT_SET, 1, ""),				// 130
};