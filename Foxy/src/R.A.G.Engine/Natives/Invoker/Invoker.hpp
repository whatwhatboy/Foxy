#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/scrNativeCallContext.hpp"
#include "R.A.G.Engine/scrNativeRegistrationTable.hpp"
#include "NativeContext.hpp"
#include "../NativeDefines.hpp"

namespace base {
	class invoker {
	public:
		void cache();
	public:
		void begin();
		void end(rage::scrNativeHash hash);
	public:
		template <typename t>
		void push(t&& value) {
			m_context.push(std::forward<t>(value));
		}
		template <typename t>
		t getRet() {
			return m_context.getRetVal<t>();
		}
	public:
		std::map<rage::scrNativeHash, rage::Cmd> m_cache{};
		nativeContext m_context{};
	};
	inline invoker g_invoker{};
}