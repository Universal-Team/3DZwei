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

#include "config.hpp"
#include "gui.hpp"
#include "overlay.hpp"
#include "structs.hpp"

extern std::unique_ptr<Config> config;

static const std::vector<Structs::ButtonPos> cardPos = {
	{71, 61, 55, 55},
	{132, 61, 55, 55},
	{193, 61, 55, 55},
	{71, 122, 55, 55},
	{132, 122, 55, 55},
	{193, 122, 55, 55}
};

// Draw.
static void Draw() {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	GFX::DrawTop();
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), "3DZwei - " + Lang::get("GAME_RULES"), 390);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.55f, Lang::get("GAME_LOGIC")))/2, 0.55f, config->textColor(), Lang::get("GAME_LOGIC"), 390, 70);
}

static void DrawAnimRight(int delay) {
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));

	// Logic here!
	if (delay > 330) {
		for (int i = 0; i < (int)cardPos.size(); i++) {
			GFX::DrawCard(cards_card_empty_idx, cardPos[i].x, cardPos[i].y);
		}
	}

	if (delay > 260 && delay < 330) {
		for (int i = 1; i < (int)cardPos.size(); i++) {
			GFX::DrawCard(cards_card_empty_idx, cardPos[i].x, cardPos[i].y);
		}

		GFX::DrawCard(0, cardPos[0].x, cardPos[0].y);
	}

	if (delay > 190 && delay < 260) {
		for (int i = 2; i < (int)cardPos.size(); i++) {
			GFX::DrawCard(cards_card_empty_idx, cardPos[i].x, cardPos[i].y);
		}

		GFX::DrawCard(0, cardPos[0].x, cardPos[0].y);
		GFX::DrawCard(0, cardPos[1].x, cardPos[1].y);
	}

	if (delay > 50 && delay < 190) {
		for (int i = 2; i < (int)cardPos.size(); i++) {
			GFX::DrawCard(cards_card_empty_idx, cardPos[i].x, cardPos[i].y);
		}

		GFX::DrawCard(0, cardPos[0].x, cardPos[0].y);
		GFX::DrawCard(0, cardPos[1].x, cardPos[1].y);

		GFX::DrawSprite(sprites_right_idx, 100, 100);
	}

	C3D_FrameEnd(0);
}

static void DrawAnimFalse(int delay) {
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));

	// Logic here!
	if (delay > 330) {
		for (int i = 0; i < (int)cardPos.size(); i++) {
			GFX::DrawCard(cards_card_empty_idx, cardPos[i].x, cardPos[i].y);
		}
	}

	if (delay > 260 && delay < 330) {
		for (int i = 1; i < (int)cardPos.size(); i++) {
			GFX::DrawCard(cards_card_empty_idx, cardPos[i].x, cardPos[i].y);
		}

		GFX::DrawCard(0, cardPos[0].x, cardPos[0].y);
	}

	if (delay > 190 && delay < 260) {
		for (int i = 2; i < (int)cardPos.size(); i++) {
			GFX::DrawCard(cards_card_empty_idx, cardPos[i].x, cardPos[i].y);
		}

		GFX::DrawCard(0, cardPos[0].x, cardPos[0].y);
		GFX::DrawCard(1, cardPos[1].x, cardPos[1].y);
	}

	if (delay > 50 && delay < 190) {
		for (int i = 2; i < (int)cardPos.size(); i++) {
			GFX::DrawCard(cards_card_empty_idx, cardPos[i].x, cardPos[i].y);
		}

		GFX::DrawCard(0, cardPos[0].x, cardPos[0].y);
		GFX::DrawCard(1, cardPos[1].x, cardPos[1].y);

		GFX::DrawSprite(sprites_wrong_idx, 100, 100);
	}

	C3D_FrameEnd(0);
}

void Overlays::ShowRules() {
	int delay = 400;
	bool showCorrect = true;
	while(1) {
		Draw();
		if (showCorrect) DrawAnimRight(delay);
		else DrawAnimFalse(delay);

		hidScanInput();

		if (delay > 0) {
			delay--;

			if (delay == 0) {
				if (showCorrect) showCorrect = false;
				else showCorrect = true;
				delay = 400;
			}
		}


		if (hidKeysDown()) break;
	}
}