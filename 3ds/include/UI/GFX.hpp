/*
*   This file is part of 3DZwei
*   Copyright (C) 2020-2021 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#ifndef _3DZWEI_GFX_HPP
#define _3DZWEI_GFX_HPP

#include "screenCommon.hpp"
#include "sprites.h"
#include <vector>

#define BAR_BLUE C2D_Color32(36, 52, 81, 255) // Darker Blue.
#define BG_BLUE C2D_Color32(52, 64, 120, 255) // Lighter Blue.
#define TEXT_WHITE C2D_Color32(216, 228, 228, 255) // Not #FFFFFF, it's a bit darker White.

namespace GFX {
	extern C2D_SpriteSheet Cards, Characters, Sprites;

	void LoadSheets();
	void UnloadSheets();

	void DrawTop();
	void DrawBottom();

	void DrawCornerEdge(const bool LeftSide, const int XPos, const int YPos, const int YSize, const bool DrawArrow = true);
	void DrawCheckbox(const int PosX, const int PosY, const bool Checked = false, const bool OnList = false);
	void DrawCard(const size_t Idx, const int X, const int Y);
};

#endif