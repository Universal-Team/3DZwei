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

#include "overlay.hpp"

extern std::unique_ptr<Config> config;

static void Draw(int page) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	GFX::DrawTop();
	Gui::DrawStringCentered(0, 1, 0.7f, config->textColor(), "3DZwei - " + Lang::get("CREDITS"), 390);
	GFX::DrawSprite(sprites_stackz_idx, -2, 70);
	GFX::DrawSprite(sprites_Logo_idx, 137, 65);
	Gui::DrawStringCentered(0, 30, 0.6f, config->textColor(), Lang::get("DEVELOPED_BY"), 390);
	Gui::DrawStringCentered(0, 45, 0.6f, config->textColor(), Lang::get("MAIN_DEV"), 390);
	Gui::DrawString(395-Gui::GetStringWidth(0.6f, (Lang::get("CURRENT_VERSION") + V_STRING)), 219, 0.6f, config->textColor(), (Lang::get("CURRENT_VERSION") + V_STRING), 390);

	/* Bottom with pages. */
	GFX::DrawBottom();
	Gui::DrawStringCentered(0, 217, 0.7f, config->textColor(), Lang::get("CURRENT_PAGE") + std::to_string(page + 1) + " | 1", 310);

	switch(page) {
		case 0:
			Gui::DrawStringCentered(0, 1, 0.7f, config->textColor(), Lang::get("GENERAL_CREDITS"), 310);
			Gui::DrawStringCentered(0, 30, 0.7f, config->textColor(), "devkitPro", 310);
			Gui::DrawStringCentered(0, 60, 0.6f, config->textColor(), Lang::get("DEVKIT"), 310);
			Gui::DrawStringCentered(0, 90, 0.7f, config->textColor(), "SuperSaiyajinStackZ", 310);
			Gui::DrawStringCentered(0, 120, 0.6f, config->textColor(), Lang::get("DEVELOPING_CORE"), 310);
			Gui::DrawStringCentered(0, 150, 0.7f, config->textColor(), "Universal-Team", 310);
			Gui::DrawStringCentered(0, 180, 0.6f, config->textColor(), Lang::get("UNIVERSAL_CORE"), 310);
			break;
	}

	C3D_FrameEnd(0);
}

/*
	Display Credits.
*/
void Overlays::showCredits() {
	bool doOut = false;
	int page = 0;

	while(!doOut) {
		Draw(page);
		hidScanInput();

		if (hidKeysDown() & KEY_B) doOut = true;
	}
}