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

#include "cardUtils.hpp"
#include "config.hpp"
#include "gui.hpp"
#include "overlay.hpp"
#include <unistd.h>

extern std::unique_ptr<Config> config;
extern C2D_SpriteSheet BGs, cards; // Our default sheets.

static const std::vector<Structs::ButtonPos> cardPos = {
	{60, 30, 55, 55},
	{120, 30, 55, 55},
	{180, 30, 55, 55},
	{240, 30, 55, 55},
	{300, 30, 55, 55},

	{60, 90, 55, 55},
	{120, 90, 55, 55},
	{180, 90, 55, 55},
	{240, 90, 55, 55},
	{300, 90, 55, 55}
};

// Draw.
static void Draw(C2D_SpriteSheet &sheet, C2D_SpriteSheet &BG, int page, const bool &hasBG) {
	const std::string temp = std::to_string(page + 1) + " | " + std::to_string((((C2D_SpriteSheetCount(sheet) - 1) / (10 + 1)) + 1));
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	
	GFX::DrawTop();
	if (hasBG && BG) Gui::DrawSprite(BG, 0, 0, 30); // Draw BG if has BG.

	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, -2, 0.8f, config->textColor(), "3DZwei - " + Lang::get("CARDSET_PREVIEW"), 390);

	// Preview cards.
	if (sheet) {
		for (int i = 0 + (page * 10), i2 = 0; (i < (int)C2D_SpriteSheetCount(sheet) - 1) && (i < (0 + (page * 10) + 10)); i++, i2++) {
			Gui::DrawSprite(sheet, i, cardPos[i2].x, cardPos[i2].y);
		}

		Gui::DrawSprite(sheet, C2D_SpriteSheetCount(sheet)-1, 180, 150);
	}

	Gui::DrawString(397-Gui::GetStringWidth(0.6f, temp), 237-Gui::GetStringHeight(0.6f, temp), 0.6f, config->textColor(), temp);

	GFX::DrawBottom();
	if (BG && hasBG && C2D_SpriteSheetCount(sheet) >= 2) Gui::DrawSprite(BG, 1, 0, 0); // Draw BG, if has BG.

	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, -2, 0.7f, config->textColor(), Lang::get("CARDSET_PREVIEW_MSG"), 310);
	C3D_FrameEnd(0);
}

static bool checkForValidate(std::string file) {
	if (access(file.c_str(), F_OK) != 0) {
		return false;
	} else {
		return true;
	}
}

static Result loadSet(std::string folder, C2D_SpriteSheet &sheet, C2D_SpriteSheet &BG, bool &hasBG) {
	hasBG = false;

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

	if (checkForValidate(folder)) {
		if (checkForValidate(folder + "/bg.t3x")) {
			Msg::DisplayMsg(Lang::get("LOADING_SPRITESHEET"));
			// Load.
			Gui::loadSheet((folder + "/bg.t3x").c_str(), BG);
			hasBG = true;
		}
	}

	return 0; // Should not go here.
}

static void finalize(const std::string folder, const bool &hasBG) {
	if (folder == "3DZWEI_DEFAULT_ROMFS") {
		Msg::DisplayMsg(Lang::get("LOADING_SPRITESHEET"));
		Gui::unloadSheet(cards);
		Gui::loadSheet("romfs:/gfx/cards.t3x", cards);
		config->cardFile("romfs:/gfx/cards.t3x");
		config->Set("_3DZWEI_ROMFS");
		CardUtils::fillIndex(); // Fill normally here.
	} else {
		Msg::DisplayMsg(Lang::get("LOADING_SPRITESHEET"));
		Gui::unloadSheet(cards);
		Gui::loadSheet((folder + "/cards.t3x").c_str(), cards);
		config->cardFile((folder + "/cards.t3x"));
		CardUtils::fillIndex(); // Fill normally here.
		config->Set(folder + "/");

		// BG stuff.
		if (hasBG) {
			config->BG((folder + "/bg.t3x"));
			if (BGs) Gui::unloadSheet(BGs);
			Gui::loadSheet((folder + "/bg.t3x").c_str(), BGs);
			BGLoaded = true;
		}
	}
}

void Overlays::PreviewCards(C2D_SpriteSheet &sheet, C2D_SpriteSheet &BG, std::string folder) {
	int page = 0;
	bool hasBG;
	// Do prev logic here.
	if (loadSet(folder, sheet, BG, hasBG) != 0) return; // No No No.

	while(1) {
		Draw(sheet, BG, page, hasBG);
		hidScanInput();

		if (hidKeysDown() & KEY_A) {
			finalize(folder, hasBG);
			if (sheet) Gui::unloadSheet(sheet);
			if (hasBG && BG) Gui::unloadSheet(BG);
			break;
		}
		
		if (hidKeysDown() & KEY_R || hidKeysDown() & KEY_RIGHT) {
			if (C2D_SpriteSheetCount(sheet) - 1 > 10) {
				if (page < (int)((C2D_SpriteSheetCount(sheet) - 1) / (10 + 1))) page++;
			}
		}

		if (hidKeysDown() & KEY_L || hidKeysDown() & KEY_LEFT) {
			if (page > 0) page--;
		}

		if (hidKeysDown() & KEY_B) {
			if (sheet) Gui::unloadSheet(sheet);
			if (BG) Gui::unloadSheet(BG);
			break;
		}
	}
}