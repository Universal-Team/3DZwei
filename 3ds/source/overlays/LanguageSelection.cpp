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

#include "config.hpp"
#include "overlay.hpp"
#include "structs.hpp"

extern touchPosition touch;
extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

static const std::vector<Structs::ButtonPos> langBlocks = {
	{37, 32, 20, 20},
	{37, 72, 20, 20}
};

static void Draw(int selection) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	GFX::DrawTop();
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, -2, 0.8f, config->textColor(), Lang::get("SELECT_LANG"), 390);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));

	for (int language = 0; language < 2; language++) {
		Gui::Draw_Rect(langBlocks[language].x, langBlocks[language].y, langBlocks[language].w, langBlocks[language].h, config->buttonColor());
	}

	Gui::Draw_Rect(langBlocks[selection].x, langBlocks[selection].y, langBlocks[selection].w, langBlocks[selection].h, config->selectorColor());

	Gui::DrawString(langBlocks[0].x+25, langBlocks[0].y, 0.7f, config->textColor(), "Deutsch", 320);
	Gui::DrawString(langBlocks[1].x+25, langBlocks[1].y, 0.7f, config->textColor(), "English", 320);
	C3D_FrameEnd(0);
}

// Select a Language.
void Overlays::SelectLanguage() {
	int selection = config->language();

	while(1) {
		Draw(selection);

		// The input part.
		hidScanInput();
		hidTouchRead(&touch);

		if (hidKeysDown() & KEY_TOUCH) {
			for(int language = 0; language < 2; language++) {
				if (touching(touch, langBlocks[language])) {
					config->language(language);
					Lang::load();
					return; // Exit overlay.
				}
			}
		}

		if (hidKeysDown() & KEY_DOWN) {
			if (selection < 1) selection++;
		}

		if (hidKeysDown() & KEY_UP) {
			if (selection > 0) selection--;
		}

		if (hidKeysDown() & KEY_A) {
			config->language(selection);
			Lang::load();
			return; // Exit overlay.
		}

		if (hidKeysDown() & KEY_B) {
			return; // Exit overlay.
		}
	}
}