#pragma once
#include "Pch/Common.hpp"
#include "Vectors.hpp"
#include "scrValue.hpp"

namespace rage {
	class scrVectorSpace {
	public:
		void Reset() {
			BufferCount = 0;
		}
		void CopyReferencedParametersOut() {
			while (BufferCount--) {
				Orig[BufferCount][0].Float = Buffer[BufferCount].x;
				Orig[BufferCount][1].Float = Buffer[BufferCount].y;
				Orig[BufferCount][2].Float = Buffer[BufferCount].z;
			}
		}
		uint32_t BufferCount; //0x0000
		scrValue* Orig[4]; //0x0004
		scrVector Buffer[4]; //0x0020
	}; //Size: 0x0060
	class scrThreadInfo {
	public:
		void reset() {
			ParamCount = 0;
			VectorSpace.Reset();
		}
		template <typename t>
		void pushArg(t&& value) {
			static_assert(sizeof(t) <= sizeof(std::uint64_t));
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<t>>*>(reinterpret_cast<std::uint64_t*>(Params) + (ParamCount++)) = std::forward<t>(value);
		}
		template <typename t>
		t& getArg(std::size_t index) {
			static_assert(sizeof(t) <= sizeof(std::uint64_t));
			return *reinterpret_cast<t*>(reinterpret_cast<std::uint64_t*>(Params) + index);
		}
		template <typename t>
		void setArg(std::size_t index, t&& value) {
			static_assert(sizeof(t) <= sizeof(std::uint64_t));
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<t>>*>(reinterpret_cast<std::uint64_t*>(Params) + index) = std::forward<t>(value);
		}
		template <typename t>
		t* getRetValue() {
			return reinterpret_cast<t*>(ResultPtr);
		}
		template <typename t>
		void setRetValue(t value) {
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<t>>*>(ResultPtr) = value;
		}
		void CopyReferencedParametersOut() {
			VectorSpace.CopyReferencedParametersOut();
		}
	public:
		scrValue* ResultPtr; //0x0000
		uint32_t ParamCount; //0x0008
		scrValue* Params; //0x0010
		scrVectorSpace VectorSpace;
	}; //Size: 0x00E0
}