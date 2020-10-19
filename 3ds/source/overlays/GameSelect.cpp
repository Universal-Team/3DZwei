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

/*
	TODO: Display new banner for each mode?
*/
static void Draw(int selection) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	GFX::DrawTop();
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, 1, 0.7f, config->textColor(), Lang::get("SELECT_GAME_MODE"), 390);

	/* Game Mode name here. */
	switch(selection) {
		case 0:
			Gui::DrawStringCentered(0, 215, 0.8f, config->textColor(), Lang::get("SELECTED_MODE") + Lang::get("MULTIPLAY"), 390);
			break;

		case 1:
			Gui::DrawStringCentered(0, 215, 0.8f, config->textColor(), Lang::get("SELECTED_MODE") + Lang::get("PLAY_WITH_TIME"), 390);
			break;
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	C3D_FrameEnd(0);
}

/*
	Returns 0 if MultiPlay and 1 if TimePlay. -1 if cancel.
*/
int Overlays::SelectGame() {
	int selection = 0;

	while(1) {
		Draw(selection);
		hidScanInput();

		if (hidKeysDown() & KEY_A) return selection;

		if (hidKeysDown() & KEY_B) return -1;

		if (hidKeysDown() & KEY_RIGHT) {
			if (selection == 0) selection = 1;
		}

		if (hidKeysDown() & KEY_LEFT) {
			if (selection == 1) selection = 0;
		}
	}
}