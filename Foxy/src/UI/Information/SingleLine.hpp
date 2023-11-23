#pragma once
#include "UI/Information/Line.hpp"
#include "UI/Information/FormattedString.hpp"

namespace base::ui::informational {
	struct singleLine : public line {
		singleLine(formattedString str) : line(str.midSplit(':').first, str.midSplit(':').second) {}
		void draw(dxUiManager* mgr, ImVec2 pos, float offset, float& drawBase) override {
			line::draw(mgr, pos, offset, drawBase);
		}
	};
}