#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/scrNativeCallContext.hpp"
#include "R.A.G.Engine/Vectors.hpp"
#include "R.A.G.Engine/scrValue.hpp"

struct nativeContext : public rage::scrThreadInfo {
public:
	nativeContext() {
		reset();
	}
public:
	void reset() {
		ResultPtr = &m_return;
		ParamCount = 0;
		Params = &m_arguments[0];
		VectorSpace.Reset();
		memset(&m_return, NULL, sizeof(m_return));
		memset(&m_arguments, NULL, sizeof(m_arguments));
	}
public:
	template <typename type>
	void push(type value) {
		static_assert(sizeof(type) <= 8);
		rage::scrValue data;
		*(type*)(&data) = value;
		m_arguments[ParamCount++] = data;
	}
	template <>
	void push<Vector3>(Vector3 value) {
		push(value.x);
		push(value.y);
		push(value.z);
	}
public:
	template <typename t>
	t getRetVal() {
		return *reinterpret_cast<t*>((uint64_t)ResultPtr);
	}
	template <>
	void getRetVal<void>() {
	}
	uint64_t* getResultPtr() {
		return (uint64_t*)ResultPtr;
	}
public:
	rage::scrValue m_return{};
	rage::scrValue m_arguments[32]{};
};