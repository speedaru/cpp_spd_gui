#pragma once
#include <ui/Style.h>
#include <ui/Types.h>

struct Style;

namespace spd::ui {
	struct BoxModel {
		BoxModel(const std::optional<Offsets>* padding, const std::optional<Offsets>* margin)
			: padding(padding), margin(margin) {}

		// layout styles
		const std::optional<Offsets>* padding;
		const std::optional<Offsets>* margin;

		// box
		ImVec2 contentSize{ 0, 0 }; // total children size
		ImVec2 boxSize{ 0, 0 }; // content + padding (border rect)
		ImVec2 totalSize{ 0, 0 }; // box + margin

		void Recalculate(ImVec2 contentSize, ImVec2 baseSize);

		// box size without padding
		ImVec2 GetContentAreaSize();

		// calc position of content (position + margin + padding)
        ImVec2 CalcContentPosition(ImVec2 position) const;

		// get position of border (position + margin)
        ImVec2 CalcBorderPosition(ImVec2 position) const;

		float CalcPositionX(const BoxModel& other, Alignment align) const;
		float CalcPositionY(const BoxModel& other, Alignment align) const;
		ImVec2 CalcPosition(const BoxModel& other, Alignment align) const;
	};
}
