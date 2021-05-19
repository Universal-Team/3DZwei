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

#include "3DZwei.hpp"
#include "Common.hpp"
#include "Utils.hpp"
#include <3ds.h> // aptMainLoop().
#include <dirent.h> // mkdir.
#include <ctime> // time.

/* Include all Overlays here. */
#include "CreditsOverlay.hpp"
#include "GameSettings.hpp"
#include "GameOverlay.hpp"
#include "RulesOverlay.hpp"
#include "SettingsOverlay.hpp"
#include "SplashOverlay.hpp"

std::unique_ptr<Config> _3DZwei::CFG = nullptr;


/* Constructor of the app. */
_3DZwei::_3DZwei() {
	gfxInitDefault();
	romfsInit();
	Gui::init();

	/* Create necessary directories. */
	mkdir("sdmc:/3ds", 0x777);
	mkdir("sdmc:/3ds/ut-games", 0x777); // Universal-Team Games.
	mkdir("sdmc:/3ds/ut-games/3DZwei", 0x777); // Main directory.
	mkdir("sdmc:/3ds/ut-games/sets", 0x777); // Main set path.
	mkdir("sdmc:/3ds/ut-games/sets/3DZwei", 0x777); // For the Card Sets.
	mkdir("sdmc:/3ds/ut-games/sets/characters", 0x777); // For the Character Sets.

	_3DZwei::CFG = std::make_unique<Config>();
	Lang::Load();
	GFX::LoadSheets();

	Utils::LoadCardSet(CFG->CardSet());
	Utils::LoadCharSet(CFG->CharSet());
	Utils::InitCards(true);
	_3DZwei::CFG->FetchDefaults(); // Need to do this after init, because relies on Utils::GetCharSheetSize().

	/* Display Splash, if enabled. */
	if (_3DZwei::CFG->ShowSplash()) {
		std::unique_ptr<SplashOverlay> Ovl = std::make_unique<SplashOverlay>();
		Ovl->Action();
	};

	hidSetRepeatParameters(25, 5);
	srand(time(nullptr)); // Seed for rand() usage on animation.
};


/* Prepare a Game. */
void _3DZwei::PrepareGame() {
	this->FadeOutHandler(); // Fade out.

	std::unique_ptr<GameSettings> Ovl = std::make_unique<GameSettings>(_3DZwei::CFG->GetDefault());
	const GameSettings::GameParams Params = Ovl->Action();

	if (Utils::Cards.size() > 0 && !Params.CancelGame) { // At least 1 pair should exist and the game NOT cancelled!!!
		std::unique_ptr<GameOverlay> Game = std::make_unique<GameOverlay>(Params);
		Game->Action();
	}

	this->OverlayReturn();
};


/* Show the 3DZwei Rules. */
void _3DZwei::ShowRules() {
	this->FadeOutHandler(); // Fade out.

	std::unique_ptr<RulesOverlay> Ovl = std::make_unique<RulesOverlay>();
	Ovl->Action();

	this->OverlayReturn();
};


/* Access the Settings. */
void _3DZwei::AccessSettings() {
	this->FadeOutHandler(); // Fade out.

	std::unique_ptr<SettingsOverlay> Ovl = std::make_unique<SettingsOverlay>();
	Ovl->Action();

	this->OverlayReturn();
	if ((!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade()) && (this->FAlpha > 0)) this->FAlpha = 0; // Don't blackscreen.
};


/* Show the 3DZwei Credits. */
void _3DZwei::ShowCredits() {
	this->FadeOutHandler(); // Fade out.

	std::unique_ptr<CreditsOverlay> Ovl = std::make_unique<CreditsOverlay>();
	Ovl->Action();

	this->OverlayReturn();
};


/* Returns back to an overlay. */
void _3DZwei::OverlayReturn() { this->FadeInHandler(); };


/* Fade Handlers. */
void _3DZwei::FadeOutHandler() {
	if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade()) return;

	this->FadeOut = true;
	this->FAlpha = 0;

	while(aptMainLoop() && this->FadeOut) {
		this->Draw();

		hidScanInput();
		const uint32_t Down = hidKeysDown();
		if (Down) this->FAlpha = 255, this->FadeOut = false;

		if (this->FAlpha < 255) {
			this->FAlpha += 5;

			if (this->FAlpha >= 255) this->FadeOut = false;
		}
	};
};


void _3DZwei::FadeInHandler() {
	if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade()) return;

	this->FadeIn = true;
	this->FAlpha = 255;

	while(aptMainLoop() && this->FadeIn) {
		this->Draw();

		hidScanInput();
		const uint32_t Down = hidKeysDown();
		if (Down) this->FAlpha = 0, this->FadeIn = false;

		if (this->FAlpha > 0) {
			this->FAlpha -= 5;

			if (this->FAlpha <= 0) this->FadeIn = false;
		}
	};
};


void _3DZwei::Draw() {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

	GFX::DrawTop();
	Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, "3DZwei", 395);
	Gui::DrawSprite(GFX::Sprites, sprites_logo_idx, 72, 69); // Display Logo.
	if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));

	GFX::DrawBottom();
	for (uint8_t Idx = 0; Idx < (int)this->Positions.size(); Idx++) {
		Gui::Draw_Rect(this->Positions[Idx].X, this->Positions[Idx].Y, this->Positions[Idx].W, this->Positions[Idx].H, KBD_KEYPRESSED);
		Gui::DrawStringCentered(0, this->Positions[Idx].Y + 8, 0.6f, TEXT_COLOR, Lang::Get(this->ButtonNames[Idx]), 130);
	}

	Pointer::Draw();
	if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
	C3D_FrameEnd(0);
};


/* MAIN Logic. */
int _3DZwei::Action() {
	this->FadeInHandler();

	while(aptMainLoop() && !this->FullExit) {
		this->Draw();

		if (!this->FullExit) {
			hidScanInput();
			touchPosition T;
			const uint32_t Down = hidKeysDown();
			const uint32_t Held = hidKeysHeld();
			hidTouchRead(&T);

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

			if (Down & KEY_START) {
				this->FadeOutHandler();
				this->FullExit = true;
			}
		}
	};

	/* De-init everything. */
	_3DZwei::CFG->Sav();
	GFX::UnloadSheets();
	Gui::exit();
	romfsExit();
	gfxExit();

	return 0;
};


int main() {
	std::unique_ptr<_3DZwei> Ovl = std::make_unique<_3DZwei>(); // Init everything.
	return Ovl->Action(); // Run app + handle main loop.
};