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
#include <ctime>

extern std::unique_ptr<Config> config;

#define _SPLASH_X_LOGO_POS 135
#define _SPLASH_LOGO_INIT_DELAY 50
#define _SPLASH_WAIT_DELAY 200

static void Draw(const int &logoPos, const int &fadeAlpha, const uint32_t &year) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	GFX::DrawTop();
	Gui::DrawStringCentered(0, 1, 0.7f, config->textColor(), Lang::get("UNIVERSAL_TEAM_PRESENTS"));
	GFX::DrawSprite(sprites_UT_idx, 30, 71);
	//GFX::DrawSprite(sprites_stackz_idx, -4, 70);

	if (logoPos < 400) GFX::DrawSprite(sprites_Logo_idx, logoPos, 56);
	Gui::DrawStringCentered(0, 217, 0.7f, config->textColor(), "2020 - " + std::to_string(year), 390);
	if (fadeAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, fadeAlpha));

	GFX::DrawBottom(false);
	GFX::DrawSprite(sprites_universal_core_idx, 0, 26);
	if (fadeAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, fadeAlpha));
	C3D_FrameEnd(0);
}

void Overlays::SplashOverlay() {
	int delay = _SPLASH_WAIT_DELAY, logoPos = 402, swipeDelay = _SPLASH_LOGO_INIT_DELAY, fadeAlpha = 255;
	bool doOut = false, swipedIn = false, doSwipe = false, fadeInSplash = true;

	time_t currentTime = time(NULL);
	struct tm *currentTimeStruct = localtime(&currentTime);
	const uint32_t year = 1900 + currentTimeStruct->tm_year;

	while(!doOut) {
		/* Fade in effect. */
		if (fadeInSplash) {
			if (fadeAlpha > 0) {
				fadeAlpha -= 2;
				if (fadeAlpha <= 0) {
					fadeAlpha = 0;
					fadeInSplash = 0;
				}
			}
		}

		Draw(logoPos, fadeAlpha, year);
		hidScanInput();

		if (hidKeysDown()) doOut = true;

		/* The initial delay here.. */
		if (!swipedIn) {
			if (swipeDelay > 0) {
				swipeDelay--;

				if (swipeDelay == 0) doSwipe = true;
			}
		}

		/* The logo swipe animation. */
		if (doSwipe) {
			if (logoPos > _SPLASH_X_LOGO_POS) {
				logoPos--;

				if (logoPos == _SPLASH_X_LOGO_POS) swipedIn = true;
			}
		}

		/* Now the rest of the logic. */
		if (swipedIn) {
			if (delay > 0) {
				delay--;

				/* If Delay == 0, exit the Overlay. */
				if (delay == 0) doOut = true;
			}
		}
	}
}