#pragma once
#include <cstdint>
#include "JSONSerialiser.hpp"

namespace rage {
	class rlMetric {
	public:
		virtual ~rlMetric() = default; //Deconstructor
		virtual int _0x08() { return 0; }; //Returns a constant integerlike 4, 5, 6
		virtual int _0x10() { return 0; }; //Returns a constant integer like 0
		virtual char const* get_name() { return ""; }; //Short name of the metric
		virtual bool to_json(JSONSerialiser* jsonStream) { return false; }; //Prints the metric out to a JSON stream
		virtual int get_size() { return 0; }; //Size in bytes of derived object (for copying)
		virtual uint32_t get_name_hash() { return 0; };     //Joaat of short name
	};
}