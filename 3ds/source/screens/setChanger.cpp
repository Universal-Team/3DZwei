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
#include "overlay.hpp"
#include "setChanger.hpp"
#include <unistd.h>

extern std::unique_ptr<Config> config;
// For the sets.
extern C2D_SpriteSheet cards;

void SetChanger::DrawPreview(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), "3DZwei - CardSet Preview", 390);

	// Preview cards.
	for (int i = 0; i < 10; i++) {
		Gui::DrawSprite(this->previewCards, i, this->cardPos[i].x, this->cardPos[i].y);
	}

	Gui::DrawSprite(this->previewCards, cards_card_empty_idx, 180, 160);

	GFX::DrawBottom();
	Gui::DrawStringCentered(0, 0, 0.7f, config->textColor(), "Press \uE000 to use and \uE001 to cancel.", 390);
}

void SetChanger::Draw(void) const {
	if (this->mode != 0) {
		this->DrawPreview();
	}
}

void SetChanger::previewLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_A) {
		if (this->setPath == "3DZWEI_DEFAULT_ROMFS") {
			Msg::DisplayMsg("Loading SpriteSheet...");
			Gui::unloadSheet(cards);
			Gui::loadSheet("romfs:/gfx/cards.t3x", cards);
			C2D_SpriteSheetFree(this->previewCards);
			config->cardFile("romfs:/gfx/cards.t3x");
			this->setPath = "";
			this->mode = 0;
		} else {
			Msg::DisplayMsg("Loading SpriteSheet...");
			Gui::unloadSheet(cards);
			Gui::loadSheet((this->setPath + "/cards.t3x").c_str(), cards);
			C2D_SpriteSheetFree(this->previewCards);
			config->cardFile((this->setPath + "/cards.t3x"));
			this->setPath = "";
			this->mode = 0;
		}
	}

	if (hDown & KEY_B) {
		Gui::unloadSheet(this->previewCards);
		this->setPath = "";
		this->mode = 0;
	}
}


void SetChanger::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->mode == 0) {
		std::string path = Overlays::SelectCardSet();

		if (path == "3DZWEI_DEFAULT_ROMFS") {
			this->loadDefault();
		}

		if (path != "") {
			this->loadSet(path);
		} else {
			Gui::screenBack(true);
			return;
		}

	} else {
		this->previewLogic(hDown, hHeld, touch);
	}
}

// Check if files of the set exist.
bool SetChanger::checkForValidate(std::string file) {
	if (access(file.c_str(), F_OK) != -1 ) {
		return true;
	} else {
		return false;
	}
}

Result SetChanger::loadDefault() {
	char message [100];
	snprintf(message, sizeof(message), "Would you like to load this set?\n\n%s", "3DZWEI_DEFAULT");
	if (Msg::promptMsg2(message)) {
		Msg::DisplayMsg("Loading SpriteSheet...");
		// Load.
		Gui::loadSheet("romfs:/gfx/cards.t3x", this->previewCards);
		this->setPath = "3DZWEI_DEFAULT_ROMFS";
		this->mode = 1;
	}

	return 0;
}

// Load a set.
Result SetChanger::loadSet(std::string folder) {
	if (this->checkForValidate(folder)) {
		if (this->checkForValidate(folder + "/cards.t3x")) {
			char message [100];
			snprintf(message, sizeof(message), "Would you like to load this set?\n\n%s", folder.c_str());
			if (Msg::promptMsg2(message)) {
				Msg::DisplayMsg("Loading SpriteSheet...");
				// Load.
				Gui::loadSheet((folder + "/cards.t3x").c_str(), this->previewCards);
				this->setPath = folder;
				this->mode = 1;
			}
		} else {
			char message [100];
			snprintf(message, sizeof(message), "%s does not exist!", "cards.t3x");
			Msg::DisplayWaitMsg(message);
		}
	}

	return 0;
}