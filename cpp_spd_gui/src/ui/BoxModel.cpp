#include "pch.h"
#include <ui/BoxModel.h>

namespace spd::ui {
	void BoxModel::Recalculate(ImVec2 contentSize, ImVec2 baseSize) {
		// set content area
        this->contentSize = contentSize;

		// calculate box size max(content + padding, base selected size)
		const Offsets& padding = this->padding->value_or(Offsets::ZERO);
        ImVec2 sizeWithPadding = { contentSize.x + padding.Width(), contentSize.y + padding.Height() };

        boxSize.x = std::max(baseSize.x, sizeWithPadding.x);
        boxSize.y = std::max(baseSize.y, sizeWithPadding.y);

        // calculate total size box + margin
		const Offsets& margin = this->margin->value_or(Offsets::ZERO);
        totalSize.x = boxSize.x + margin.Width();
        totalSize.y = boxSize.y + margin.Height();
	}

	ImVec2 BoxModel::GetContentAreaSize() {
		const Offsets& padding = this->padding->value_or(Offsets::ZERO);
		return { boxSize.x - padding.Width(), boxSize.y - padding.Height() };
	}

	ImVec2 BoxModel::CalcContentPosition(ImVec2 position) const {
		Offsets margin = this->margin->value_or(Offsets::ZERO);
        Offsets padding = this->padding->value_or(Offsets::ZERO);

		return {
			position.x + margin.left + padding.left,
			position.y + margin.top + padding.top,
		};
	}

	ImVec2 BoxModel::CalcBoxPosition(ImVec2 position) const {
		Offsets margin = this->margin->value_or(Offsets::ZERO);
		return { position.x + margin.left, position.y + margin.top };
	}

	float BoxModel::CalcPositionX(const BoxModel& other, Alignment align) const {
		return CalcAlignmentX(contentSize.x, other.totalSize.x, align);
	}

	float BoxModel::CalcPositionY(const BoxModel& other, Alignment align) const {
		return CalcAlignmentY(contentSize.y, other.totalSize.y, align);
	}

	ImVec2 BoxModel::CalcPosition(const BoxModel& other, Alignment align) const {
		return CalcAlignmentPos(contentSize, other.totalSize, align);
	}
}