#pragma once
#include "pgPtr.hpp"
#include "BlockMap.hpp"

namespace rage {
	class datBase {
	public:
		virtual ~datBase() = default;
	}; //Size: 0x0008
	static_assert(sizeof(datBase) == 0x8);
	class pgBase {
	public:
		virtual ~pgBase() = default;
	private:
		pgPtr<BlockMap> m_block_map;
	};
	template <typename T, typename Base = datBase>
	class atDNode : public Base {
	public:
		T m_data;
		void* m_unk;
		atDNode<T, Base>* m_next;
	};
	template <typename Node>
	class atDList {
	public:
		Node* m_head;
		Node* m_tail;
	};
}