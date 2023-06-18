/*
*   This file is part of 3DZwei
*   Copyright (C) 2020-2023 Universal-Team
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

#include "GFX.hpp"
#include "screenCommon.hpp"
#include "Utils.hpp"

/* All used Spritesheets. */
C2D_SpriteSheet GFX::Cards = nullptr, GFX::Characters = nullptr, GFX::Sprites = nullptr;


/* Load all Spritesheets. */
void GFX::LoadSheets() {
	Gui::loadSheet("romfs:/gfx/sprites.t3x", GFX::Sprites);
}


/* Unload all Spritesheets. */
void GFX::UnloadSheets() {
	if (GFX::Cards) Gui::unloadSheet(GFX::Cards); // Need a special check in place, because it potential could also be not loaded.
	if (GFX::Characters) Gui::unloadSheet(GFX::Characters); // Need a special check in place, because it potential could also be not loaded.
	Gui::unloadSheet(GFX::Sprites);
}


/* Draws the Top Screen base. */
void GFX::DrawTop() {
	Gui::ScreenDraw(Top);
	Gui::Draw_Rect(0, 0, 400, 25, BAR_BLUE);
	Gui::Draw_Rect(0, 25, 400, 215, BG_BLUE);
}


/* Draws the Bottom Screen base. */
void GFX::DrawBottom() {
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, BG_BLUE);
}


/*
	Draw the Corner Edges, being used for the Overlays like SetSelection.

	const bool LeftSide: If that Corner should be drawn on the Left (true) or Right (false) side.
	const int XPos: The X Start Position where to draw.
	const int YPos: The Y Start Position where to draw (usually 25 with the bar, or 0 without).
	const int YSize: The Y Size of the Corners.
	const bool DrawArrow: If the Arrow should be drawn or not.
*/
void GFX::DrawCornerEdge(const bool LeftSide, const int XPos, const int YPos, const int YSize, const bool DrawArrow) {
	if (LeftSide) {
		Gui::DrawSprite(GFX::Sprites, sprites_corner_idx, XPos, YPos);
		Gui::Draw_Rect(XPos, YPos + 25, 25, YSize - 50, BAR_BLUE);
		Gui::DrawSprite(GFX::Sprites, sprites_corner_idx, XPos, YPos + YSize - 25, 1.0f, -1.0f);
		if (DrawArrow) Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, XPos + 5, YPos + ((YSize / 2) - (39 / 2)));

	} else {
		Gui::DrawSprite(GFX::Sprites, sprites_corner_idx, XPos, YPos, -1.0f, 1.0f);
		Gui::Draw_Rect(XPos, YPos + 25, 25, YSize - 50, BAR_BLUE);
		Gui::DrawSprite(GFX::Sprites, sprites_corner_idx, XPos, YPos + YSize - 25, -1.0f, -1.0f);
		if (DrawArrow) Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, XPos, YPos + ((YSize / 2) - (39 / 2)), -1.0f, 1.0f);
	}
}


/*
	Draws a Checkbox.

	const int PosX: The X Start Position where to draw the checkbox.
	const int PosY: The Y Start Position where to draw the checkbox.
	const bool Checked: If it's checked or not.
	const bool OnList: If on the list or not. List uses the lighter checkbox, while otherwise the darker one.
*/
void GFX::DrawCheckbox(const int PosX, const int PosY, const bool Checked, const bool OnList) {
	Gui::DrawSprite(GFX::Sprites, (OnList ? sprites_checkbox_light_idx : sprites_checkbox_dark_idx), PosX, PosY);
	if (Checked) Gui::DrawSprite(GFX::Sprites, sprites_mark_idx, PosX + 4, PosY + 4);
}


/*
	Draw the Cards.. but with size checks to make sure no bad behavior happens.

	const size_t Idx: The card index.
	const int X: The X-Position where to draw the card.
	const int Y: The Y-Position where to draw the card.
	const float ScaleX: The X-Scale of the card.
	const float ScaleY: The Y-Scale of the card.
*/
void GFX::DrawCard(const size_t Idx, const int X, const int Y, const float ScaleX, const float ScaleY) {
	if (Idx <= Utils::Cards.size()) Gui::DrawSprite(GFX::Cards, Utils::Cards[Idx], X, Y, ScaleX, ScaleY);
}