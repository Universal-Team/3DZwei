/*
*   This file is part of 3DZwei
*   Copyright (C) 2020-2021 Universal-Team
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

#include "Common.hpp"
#include "CreditsOverlay.hpp"
#include "GameScreen.hpp"
#include "GameSettings.hpp"
#include "MainMenu.hpp"
#include "RulesOverlay.hpp"
#include "Settings.hpp"
#include "Utils.hpp"

void MainMenu::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, "3DZwei", 395);
	Gui::DrawSprite(GFX::Sprites, sprites_logo_idx, 72, 69); // Display Logo.
	if (_3DZwei::FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, _3DZwei::FAlpha));

	GFX::DrawBottom();

	for (uint8_t Idx = 0; Idx < (int)this->Positions.size(); Idx++) {
		Gui::Draw_Rect(this->Positions[Idx].X, this->Positions[Idx].Y, this->Positions[Idx].W, this->Positions[Idx].H, KBD_KEYPRESSED);

		Gui::DrawStringCentered(0, this->Positions[Idx].Y + 8, 0.6f, TEXT_COLOR, Lang::Get(this->ButtonNames[Idx]), 130);
	}

	Pointer::Draw();
	if (_3DZwei::FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, _3DZwei::FAlpha));
};

/* Prepare a Game. */
void MainMenu::GotoGame() {
	std::unique_ptr<GameSettings> Ovl = std::make_unique<GameSettings>();
	const GameSettings::GameParams Params = Ovl->Action();

	if (Utils::Cards.size() > 0 && !Params.CancelGame) { // At least 1 pair should exist and the game NOT cancelled!!!
		Gui::setScreen(std::make_unique<GameScreen>(Params), false, true);
	}
};

/* Show the 3DZwei Rules. */
void MainMenu::ShowRules() {
	std::unique_ptr<RulesOverlay> Ovl = std::make_unique<RulesOverlay>();
	Ovl->Action();
};

/* Access the Settings. */
void MainMenu::GotoSettings() { Gui::setScreen(std::make_unique<Settings>(), false, true); };

/* Show the 3DZwei Credits. */
void MainMenu::ShowCredits() {
	std::unique_ptr<CreditsOverlay> Ovl = std::make_unique<CreditsOverlay>();
	Ovl->Action();
};

void MainMenu::Logic(uint32_t Down, uint32_t Held, touchPosition T) {
	Pointer::ScrollHandling(Held); // Pointer Handling.

	if (Down & KEY_A) {
		for (auto &Position : this->Positions) {
			if (Pointer::Clicked(Position, true)) break;
		}
	}

	if (Down & KEY_TOUCH) {
		for (auto &Position : this->Positions) {
			if (Touched(Position, T, true)) break;
		}
	}

	if (Down & KEY_START) _3DZwei::Exiting = true; // Exit App.
};