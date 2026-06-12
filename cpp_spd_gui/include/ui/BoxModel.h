#pragma once
#include <ui/Style.h>
#include <ui/types.h>

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
		ImVec2 GetContentAreaSize() const;

		// calc position of content (position + margin + padding)
        ImVec2 CalcContentPosition(ImVec2 position) const;

		// get position of border (position + margin)
        ImVec2 CalcBoxPosition(ImVec2 position) const;

		// position another box model horizontally aligned inside this box
		float CalcPositionX(const BoxModel& other, Alignment align) const;

		// position another box model vertically aligned inside this box
		float CalcPositionY(const BoxModel& other, Alignment align) const;

		// position another box model aligned inside this box
		ImVec2 CalcPosition(const BoxModel& other, Alignment align) const;

		// calc content start position (position + margin + padding + alignment)
		ImVec2 CalcAlignedContentStart(ImVec2 position, Alignment align) const;
	};
}
