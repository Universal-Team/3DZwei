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

#include "colorChanger.hpp"
#include "config.hpp"
#include "keyboard.hpp"
#include "setChanger.hpp"
#include "uiSettings.hpp"


extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

void UISettings::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), "3DZwei - UISettings", 390);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();

	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, config->buttonColor());
		if (this->Selection == i) {
			GFX::DrawButtonSelector(mainButtons[i].x, mainButtons[i].y);
		}
	}

	Gui::DrawStringCentered(-80, mainButtons[0].y+12, 0.6f, config->textColor(), "Color Settings", 130);
	Gui::DrawStringCentered(80, mainButtons[1].y+12, 0.6f, config->textColor(), "CardSets", 130);
	Gui::DrawStringCentered(-80, mainButtons[2].y+12, 0.6f, config->textColor(), "Card Delay", 130);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void UISettings::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			Gui::setScreen(std::make_unique<ColorChanger>(), true, true);
		} else if (touching(touch, mainButtons[1])) {
			Gui::setScreen(std::make_unique<SetChanger>(), true, true);
		} else if (touching(touch, mainButtons[2])) {
			int tempDelay = Keyboard::setInt(999, "Enter the Card Delay. 70 is default.");
			if (tempDelay != -1) config->delay(tempDelay);
		}
	}

	if (hDown & KEY_RIGHT) {
		if (this->Selection == 0) this->Selection = 1;
	}

	if (hDown & KEY_UP) {
		if (this->Selection == 2) this->Selection = 0;
	}

	if (hDown & KEY_DOWN) {
		if (this->Selection == 0) this->Selection = 2;
	}

	if (hDown & KEY_LEFT) {
		if (this->Selection == 1) this->Selection = 0;
	}

	if (hDown & KEY_A) {
		if (this->Selection == 0) {
			Gui::setScreen(std::make_unique<ColorChanger>(), true, true);
		} else if (this->Selection == 1) {
			Gui::setScreen(std::make_unique<SetChanger>(), true, true);
		} else if (this->Selection == 2) {
			int tempDelay = Keyboard::setInt(999, "Enter the Card Delay. 70 is default.");
			if (tempDelay != -1) config->delay(tempDelay);
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack(true);
		return;
	}
}