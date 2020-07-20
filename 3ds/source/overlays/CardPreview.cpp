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
#include <unistd.h>

extern std::unique_ptr<Config> config;
extern C2D_SpriteSheet cards; // Our default cards sheet.

static const std::vector<Structs::ButtonPos> cardPos = {
	{60, 40, 55, 55},
	{120, 40, 55, 55},
	{180, 40, 55, 55},
	{240, 40, 55, 55},
	{300, 40, 55, 55},

	{60, 100, 55, 55},
	{120, 100, 55, 55},
	{180, 100, 55, 55},
	{240, 100, 55, 55},
	{300, 100, 55, 55}
};

// Draw.
static void Draw(C2D_SpriteSheet &sheet) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	GFX::DrawTop();
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), "3DZwei - " + Lang::get("CARDSET_PREVIEW"), 390);

	// Preview cards.
	if (sheet) {
		for (int i = 0; i < 10; i++) {
			Gui::DrawSprite(sheet, i, cardPos[i].x, cardPos[i].y);
		}

		Gui::DrawSprite(sheet, cards_card_empty_idx, 180, 160);
	}

	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, 0, 0.7f, config->textColor(), Lang::get("CARDSET_PREVIEW_MSG"), 390);
	C3D_FrameEnd(0);
}

static bool checkForValidate(std::string file) {
	if (access(file.c_str(), F_OK) != -1 ) {
		return true;
	} else {
		return false;
	}
}

static Result loadSet(std::string folder, C2D_SpriteSheet &sheet) {
	if (folder == "3DZWEI_DEFAULT_ROMFS") {
		// ROMFS logic.
		char message [100];
		snprintf(message, sizeof(message), Lang::get("LOADING_SET_PROMPT").c_str(), "3DZWEI_DEFAULT");
		if (Msg::promptMsg2(message)) {
			Msg::DisplayMsg(Lang::get("LOADING_SPRITESHEET"));
			// Load.
			Gui::loadSheet("romfs:/gfx/cards.t3x", sheet);
			return 0; // All good.
		} else {
			return -1; // Abort.
		}
	}

	if (checkForValidate(folder)) {
		if (checkForValidate(folder + "/cards.t3x")) {
			char message [100];
			snprintf(message, sizeof(message), Lang::get("LOADING_SET_PROMPT").c_str(), folder.c_str());
			if (Msg::promptMsg2(message)) {
				Msg::DisplayMsg(Lang::get("LOADING_SPRITESHEET"));
				// Load.
				Gui::loadSheet((folder + "/cards.t3x").c_str(), sheet);
				return 0; // All good.
			} else {
				return -1; // Abort.
			}
		} else {
			char message [100];
			snprintf(message, sizeof(message), Lang::get("FILE_NOT_EXIST").c_str(), "cards.t3x");
			Msg::DisplayWaitMsg(message);
			return -1; // Not all good.
		}
	}

	return -2; // Should not go here.
}

static void finalize(const std::string folder) {
	if (folder == "3DZWEI_DEFAULT_ROMFS") {
		Msg::DisplayMsg(Lang::get("LOADING_SPRITESHEET"));
		Gui::unloadSheet(cards);
		Gui::loadSheet("romfs:/gfx/cards.t3x", cards);
		config->cardFile("romfs:/gfx/cards.t3x");
	} else {
		Msg::DisplayMsg(Lang::get("LOADING_SPRITESHEET"));
		Gui::unloadSheet(cards);
		Gui::loadSheet((folder + "/cards.t3x").c_str(), cards);
		config->cardFile((folder + "/cards.t3x"));
	}
}

void Overlays::PreviewCards(C2D_SpriteSheet &sheet, std::string folder) {
	// Do prev logic here.
	if (loadSet(folder, sheet) != 0) return; // No No No.

	while(1) {
		Draw(sheet);
		hidScanInput();

		if (hidKeysDown() & KEY_A) {
			finalize(folder);
			Gui::unloadSheet(sheet);
			break;
		}
		
		if (hidKeysDown() & KEY_B) {
			Gui::unloadSheet(sheet);
			break;
		}
	}
}