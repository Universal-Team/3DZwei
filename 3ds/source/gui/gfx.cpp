/*
*   This file is part of 3DZwei
*   Copyright (C) 2020 SuperSaiyajinStackZ
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

void GFX::DrawTop(bool useBars) {
	Gui::ScreenDraw(Top);
	if (useBars) {
		Gui::Draw_Rect(0, 0, 400, 25, config->barColor());
		Gui::Draw_Rect(0, 25, 400, 190, config->bgColor());
		Gui::Draw_Rect(0, 215, 400, 25, config->barColor());
	} else {
		Gui::Draw_Rect(0, 0, 400, 240, config->bgColor());
	}
}

// False actually only draws 1 bar on the top screen. Special case for the Game screen. xD
void GFX::DrawBottom(bool useBars) {
	Gui::ScreenDraw(Bottom);
	if (useBars) {
		Gui::Draw_Rect(0, 0, 320, 25, config->barColor());
		Gui::Draw_Rect(0, 25, 320, 190, config->bgColor());
		Gui::Draw_Rect(0, 215, 320, 25, config->barColor());
	} else {
		Gui::Draw_Rect(0, 0, 320, 240, config->bgColor());
	}
}

extern C2D_SpriteSheet cards, characters, sprites;

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
}

// Select something from a list.
int GFX::selectList(std::vector<std::string> content, std::string msg, int oldIndex) {
	int selection = 0;
	int keyRepeatDelay = 5;
	while(1) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		GFX::DrawFileBrowseBG();
		Gui::DrawStringCentered(0, 0, 0.7f, config->textColor(), msg, 400);
		std::string cnts;

		for (uint i = (selection < 5) ? 0 : (uint)selection - 5; i < content.size() && i < (((uint)selection < 5) ? 6 : (uint)selection + 1); i++) {
			if (i == (uint)selection) {
				cnts +=  "> " + content[i] + "\n\n";
			} else {
				cnts +=  content[i] + "\n\n";
			}
		}

		for (uint i = 0; i < ((content.size() < 6) ? 6 - content.size() : 0); i++) {
			cnts += "\n\n";
		}
		
		Gui::DrawString(26, 32, 0.53f, config->textColor(), cnts);
		GFX::DrawFileBrowseBG(false);
		C3D_FrameEnd(0);

		hidScanInput();
		if (keyRepeatDelay)	keyRepeatDelay--;

		if (hidKeysHeld() & KEY_DOWN && !keyRepeatDelay) {
			if ((uint)selection < content.size()-1) {
				selection++;
				keyRepeatDelay = 5;
			}
		}

		if (hidKeysHeld() & KEY_UP && !keyRepeatDelay) {
			if (selection > 0) {
				selection--;
				keyRepeatDelay = 5;
			}
		}

		if (hidKeysDown() & KEY_A) {
			return selection;
		}

		if (hidKeysDown() & KEY_B) {
			return oldIndex;
		}
	}
}

void GFX::DrawButtonSelector(int x, int y, float ScaleX, float ScaleY, bool useSmall) {
	static float timer			= 0.0f;
	float highlight_multiplier	= fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
	u8 r						= config->selectorColor() & 0xFF;
	u8 g						= (config->selectorColor() >> 8) & 0xFF;
	u8 b						= (config->selectorColor() >> 16) & 0xFF;
	u32 color = C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);

	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, useSmall ? sprites_btnSelector2_idx : sprites_btnSelector_idx), x, y, 0.5f, &tint, ScaleX, ScaleY);

	timer += .030;
}

void GFX::DrawCardSelector(int x, int y, float ScaleX, float ScaleY) {
	static float timer			= 0.0f;
	float highlight_multiplier	= fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
	u8 r						= config->selectorColor() & 0xFF;
	u8 g						= (config->selectorColor() >> 8) & 0xFF;
	u8 b						= (config->selectorColor() >> 16) & 0xFF;
	u32 color = C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);

	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, sprites_card_selector_idx), x, y, 0.5f, &tint, ScaleX, ScaleY);

	timer += .030;
}

void GFX::DrawCard(int index, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(cards, index, x, y, ScaleX, ScaleY);
}

// Player Character.
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