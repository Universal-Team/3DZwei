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
#include "LanguageSelector.hpp"


/* Select a Language. */
void LanguageSelector::SelectLang(const uint8_t Idx) {
	switch(Idx) {
		case 0: // English.
			if (_3DZwei::CFG->Lang() == "en") break;
			_3DZwei::CFG->Lang("en");
			Lang::Load();
			break;

		case 1: // German.
			if (_3DZwei::CFG->Lang() == "de") break;
			_3DZwei::CFG->Lang("de");
			Lang::Load();
			break;
	}

	this->Done = true;
};


void LanguageSelector::Cancel() { this->Done = true; }; // Cancel Selection.


/* Main Language Selection Logic. */
void LanguageSelector::Action() {
	Pointer::SetPos(0, 0);

	while(aptMainLoop() && !this->FullDone) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("LANG_SELECTOR_TITLE"), 395);
		Gui::DrawSprite(GFX::Sprites, sprites_logo_idx, 72, 69); // Display Logo.
		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		}

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.

		/* English. */
		Gui::DrawString(10, 40, 0.5f, TEXT_COLOR, "English");
		GFX::DrawCheckbox(this->Positions[0].X, this->Positions[0].Y, _3DZwei::CFG->Lang() == "en");

		/* Deutsch. */
		Gui::DrawString(10, 80, 0.5f, TEXT_COLOR, "Deutsch");
		GFX::DrawCheckbox(this->Positions[1].X, this->Positions[1].Y, _3DZwei::CFG->Lang() == "de");

		Gui::Draw_Rect(this->Positions[2].X, this->Positions[2].Y, this->Positions[2].W, this->Positions[2].H, KBD_KEYPRESSED); // Back.
		Gui::DrawSprite(GFX::Sprites, sprites_back_btn_idx, this->Positions[2].X, this->Positions[2].Y);

		Pointer::Draw();
		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		}
		C3D_FrameEnd(0);

		if (this->FadeIn) {
			if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade()) this->FAlpha = 0, this->FadeIn = false;
			if (this->FAlpha > 0) this->FAlpha -= 5;
			if (this->FAlpha <= 0) this->FadeIn = false;

		} else {
			if (this->Done) {
				hidScanInput();
				const uint32_t Down = hidKeysDown();

				if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade() || Down) this->FullDone = true;
				if (this->FAlpha < 255) this->FAlpha += 5;
				if (this->FAlpha >= 255) this->FullDone = true;

			} else {
				hidScanInput();
				touchPosition T;
				hidTouchRead(&T);
				const uint32_t Down = hidKeysDown();
				const uint32_t Held = hidKeysHeld();
				Pointer::ScrollHandling(Held);

				if (Down & KEY_START || Down & KEY_B) this->Done = true; // START or B can exit as well.

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
			}
		}
	};

	Pointer::SetPos(0, 0);
};