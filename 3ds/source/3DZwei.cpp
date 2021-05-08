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
#include "MainMenu.hpp"
#include "SplashOverlay.hpp"
#include "Utils.hpp"
#include <3ds.h>
#include <dirent.h>

std::unique_ptr<Config> _3DZwei::CFG = nullptr;
bool _3DZwei::Exiting = false;
uint8_t _3DZwei::FAlpha = 255;

void _3DZwei::Start() {
	gfxInitDefault();
	romfsInit();
	Gui::init();

	/* Create necessary directories. */
	mkdir("sdmc:/3ds", 0x777);
	mkdir("sdmc:/3ds/3DZwei", 0x777); // Main directory.
	mkdir("sdmc:/3ds/3DZwei/sets", 0x777); // Main set path.
	mkdir("sdmc:/3ds/3DZwei/sets/cards", 0x777); // For the Card Sets.
	mkdir("sdmc:/3ds/3DZwei/sets/characters", 0x777); // For the Character Sets.

	_3DZwei::CFG = std::make_unique<Config>();
	Lang::Load();
	GFX::LoadSheets();

	Utils::LoadCardSet(CFG->CardSet());
	Utils::LoadCharSet(CFG->CharSet());
	Utils::InitCards(true);

	/* Display Splash, if enabled. */
	if (_3DZwei::CFG->ShowSplash()) {
		std::unique_ptr<SplashOverlay> Ovl = std::make_unique<SplashOverlay>();
		Ovl->Action();
	}

	_3DZwei::FAlpha = 0;

	Gui::setScreen(std::make_unique<MainMenu>(), false, true);
};

int _3DZwei::Logic() {
	bool FullExit = false;

	_3DZwei::Start();

	while(aptMainLoop() && !FullExit) {
		hidScanInput();
		touchPosition T;
		const uint32_t Down = hidKeysDown();
		const uint32_t Held = hidKeysHeld();
		hidTouchRead(&T);

		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		Gui::DrawScreen(true);
		if (!_3DZwei::Exiting) Gui::ScreenLogic(Down, Held, T, false, true); // Only logic, if doesn't Exit.
		C3D_FrameEnd(0);

		if (_3DZwei::Exiting) {
			hidScanInput(); // Re-Scan.
			if (hidKeysDown() & KEY_START) _3DZwei::FAlpha = 255;

			if (_3DZwei::FAlpha < 252) _3DZwei::FAlpha += 3;
			if (_3DZwei::FAlpha >= 252) FullExit = true;
		}
	};

	return _3DZwei::Exit();
};

int _3DZwei::Exit() {
	CFG->Sav();
	GFX::UnloadSheets();
	Gui::exit();
	romfsExit();
	gfxExit();

	return 0;
};

int main() { return _3DZwei::Logic(); };