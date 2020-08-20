/*
*   This file is part of 3DZwei
*   Copyright (C) 2020 Universal-Team
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

#include "common.hpp"
#include "config.hpp"

extern std::unique_ptr<Config> config;
std::vector<int> cardIndex;

void GFX::DrawTop(bool useBars) {
	Gui::ScreenDraw(Top);
	if (useBars) {
		Gui::Draw_Rect(0, 0, 400, 30, config->barColor());
		Gui::Draw_Rect(0, 30, 400, 180, config->bgColor());
		Gui::Draw_Rect(0, 210, 400, 30, config->barColor());
		GFX::DrawSprite(sprites_top_screen_top_idx, 0, 0);
		GFX::DrawSprite(sprites_top_screen_bot_idx, 0, 215);
	} else {
		Gui::Draw_Rect(0, 0, 400, 240, config->bgColor());
	}
}

void GFX::DrawBottom(bool useBars) {
	Gui::ScreenDraw(Bottom);
	if (useBars) {
		Gui::Draw_Rect(0, 0, 320, 30, config->barColor());
		Gui::Draw_Rect(0, 30, 320, 180, config->bgColor());
		Gui::Draw_Rect(0, 210, 320, 30, config->barColor());
		GFX::DrawSprite(sprites_bottom_screen_top_idx, 0, 0);
		GFX::DrawSprite(sprites_bottom_screen_bot_idx, 0, 215);
	} else {
		Gui::Draw_Rect(0, 0, 320, 240, config->bgColor());
	}
}

extern C2D_SpriteSheet BGs, cards, characters, sprites;

void GFX::DrawSprite(int index, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(sprites, index, x, y, ScaleX, ScaleY);
}

void GFX::DrawFileBrowseBG(bool isTop) {
	isTop ? Gui::ScreenDraw(Top) : Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, isTop ? 400 : 320, 27, config->barColor());
	Gui::Draw_Rect(0, 27, isTop ? 400 : 320, 31, config->bgColor());
	Gui::Draw_Rect(0, 58, isTop ? 400 : 320, 31, config->bgColor() & C2D_Color32(255, 255, 255, 200));
	Gui::Draw_Rect(0, 89, isTop ? 400 : 320, 31, config->bgColor());
	Gui::Draw_Rect(0, 120, isTop ? 400 : 320, 31, config->bgColor() & C2D_Color32(255, 255, 255, 200));
	Gui::Draw_Rect(0, 151, isTop ? 400 : 320, 31, config->bgColor());
	Gui::Draw_Rect(0, 182, isTop ? 400 : 320, 31, config->bgColor() & C2D_Color32(255, 255, 255, 200));
	Gui::Draw_Rect(0, 213, isTop ? 400 : 320, 27, config->barColor());

	/* Bars here. */
	isTop ? GFX::DrawSprite(sprites_top_screen_top_idx, 0, 0) : GFX::DrawSprite(sprites_bottom_screen_top_idx, 0, 0);
	isTop ? GFX::DrawSprite(sprites_top_screen_bot_idx, 0, 215) : GFX::DrawSprite(sprites_bottom_screen_bot_idx, 0, 215);
}

void GFX::DrawGameBG(bool top) {
	if (BGs && BGLoaded && C2D_SpriteSheetCount(BGs) >= 2) {
		top ? GFX::DrawTop() : Gui::ScreenDraw(Bottom);
		top ? Gui::DrawSprite(BGs, 0, 0, 30) : Gui::DrawSprite(BGs, 1, 0, 0);
	} else {
		top ? GFX::DrawTop() : GFX::DrawBottom(false);
	}
}

void GFX::DrawGrid(int x, int y) {
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, config->gridColor(), 1);
	C2D_SetImageTint(&tint, C2D_TopRight, config->gridColor(), 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, config->gridColor(), 1);
	C2D_SetImageTint(&tint, C2D_BotRight, config->gridColor(), 1);

	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, sprites_grid_idx), x, y, 0.5f, &tint);
}

void GFX::DrawCard(int index, int x, int y, float ScaleX, float ScaleY) {
	/* -1 would do the "empty back" card. */
	if (index == -1) {
		Gui::DrawSprite(cards, C2D_SpriteSheetCount(cards)-1, x, y, ScaleX, ScaleY);
		return;
	}

	/* Do Nothing, when index size is smaller than 1, or index larger than size. */
	if (((int)cardIndex.size() < 1) && (index > (int)cardIndex.size()-1)) return;

	Gui::DrawSprite(cards, cardIndex[index], x, y, ScaleX, ScaleY);
}

/* Player Character. */
void GFX::DrawChar(int image, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(characters, image, x, y, ScaleX, ScaleY);
}

void GFX::DrawPlayer(int x, int y, float ScaleX, float ScaleY, int player) {
	switch (player) {
		case 0:
			DrawChar(chars_char1_idx, x, y, ScaleX, ScaleY);
			break;
		case 1:
			DrawChar(chars_char2_idx, x, y, ScaleX, ScaleY);
			break;
		case 2:
			DrawChar(chars_char3_idx, x, y, ScaleX, ScaleY);
			break;
		case 3:
			DrawChar(chars_char4_idx, x, y, ScaleX, ScaleY);
			break;
		case 4:
			DrawChar(chars_char5_idx, x, y, ScaleX, ScaleY);
			break;
		case 5:
			DrawChar(chars_char6_idx, x, y, ScaleX, ScaleY);
			break;
		case 6:
			DrawChar(chars_char7_idx, x, y, ScaleX, ScaleY);
			break;
		case 7:
			DrawChar(chars_char8_idx, x, y, ScaleX, ScaleY);
			break;
	}
}

void GFX::Button(const ButtonStruct btn) {
	Gui::Draw_Rect(btn.X, btn.Y, btn.xSize, btn.ySize, config->buttonColor());
	Gui::DrawStringCentered(btn.X - 160 + (btn.xSize/2), btn.Y + (btn.ySize/2) - 10, 0.6f, config->textColor(), btn.Text, btn.X-10, btn.Y-5);
}