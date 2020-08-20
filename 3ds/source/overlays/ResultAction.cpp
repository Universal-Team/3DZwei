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
#include "gui.hpp"
#include "overlay.hpp"

extern std::unique_ptr<Config> config;

/* Draw. */
static void Draw(std::unique_ptr<Game> &game, int avatar1, int avatar2, int neededWins) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	GFX::DrawTop();
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, -2, 0.8f, config->textColor(), Lang::get("GAME_RESULTS"), 390);

	/* Player 1. */
	GFX::DrawChar(avatar1, 10, 35);
	Gui::DrawString(16, 170, 0.6f, config->textColor(), Lang::get("WINS") + std::to_string(game->getWins(Players::Player1)), 110);
	/* Player 2. */
	GFX::DrawChar(avatar2, 280, 35);
	Gui::DrawString(286, 170, 0.6f, config->textColor(), Lang::get("WINS") + std::to_string(game->getWins(Players::Player2)), 110);

	Gui::DrawStringCentered(0, 217, 0.8f, config->textColor(), Lang::get("NEEDED_WINS") + std::to_string(neededWins), 390);

	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
	C3D_FrameEnd(0);
}


bool Overlays::ResultOverlay(std::unique_ptr<Game> &game, int neededWins, int avatar1, int avatar2) {
	while(1) {
		Draw(game, avatar1, avatar2, neededWins);
		hidScanInput();
		if (hidKeysDown()) {
			if (game->getWins(Players::Player1) >= neededWins || game->getWins(Players::Player2) >= neededWins) {
				return true; // Max wins reached, no plays needed.
			} else {
				return false; // We need to play again.
			}
		}
	}
}