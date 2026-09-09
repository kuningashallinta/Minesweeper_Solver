#pragma once

#include "imgui.h"
#include <cstdint>

namespace Colors
{
	inline constexpr std::uint32_t Accent = IM_COL32(55, 161, 241, 255);
	inline constexpr std::uint32_t Canvas = IM_COL32(234, 234, 234, 255);
	inline constexpr std::uint32_t Frame = IM_COL32(60, 60, 60, 255);
	inline constexpr std::uint32_t HoverFill = IM_COL32(55, 161, 241, 70);
	inline constexpr std::uint32_t Border = IM_COL32(61, 61, 61, 255);
	inline constexpr std::uint32_t ActiveBorder = IM_COL32(66, 146, 214, 255);
	inline constexpr std::uint32_t CellHover = IM_COL32(220, 220, 255, 255);
	inline constexpr std::uint32_t DisabledOverlay = IM_COL32(0, 0, 0, 100);
}
