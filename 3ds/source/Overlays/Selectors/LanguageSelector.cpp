/*
*   This file is part of 3DZwei
*   Copyright (C) 2020-2023 Universal-Team
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
		case 0: // Bruh.
			if (_3DZwei::CFG->Lang() == "br") break;
			_3DZwei::CFG->Lang("br");
			Lang::Load();
			break;

		case 1: // Deutsch.
			if (_3DZwei::CFG->Lang() == "de") break;
			_3DZwei::CFG->Lang("de");
			Lang::Load();
			break;

		case 2: // English.
			if (_3DZwei::CFG->Lang() == "en") break;
			_3DZwei::CFG->Lang("en");
			Lang::Load();
			break;

		case 3: // Español.
			if (_3DZwei::CFG->Lang() == "es") break;
			_3DZwei::CFG->Lang("es");
			Lang::Load();
			break;

		case 4: // Français.
			if (_3DZwei::CFG->Lang() == "fr") break;
			_3DZwei::CFG->Lang("fr");
			Lang::Load();
			break;

		case 5: // Italiano.
			if (_3DZwei::CFG->Lang() == "it") break;
			_3DZwei::CFG->Lang("it");
			Lang::Load();
			break;

		case 6: // 日本語.
			if (_3DZwei::CFG->Lang() == "ja") break;
			_3DZwei::CFG->Lang("ja");
			Lang::Load();
			break;

		case 7: // Polski.
			if (_3DZwei::CFG->Lang() == "pl") break;
			_3DZwei::CFG->Lang("pl");
			Lang::Load();
			break;

		case 8: // Português.
			if (_3DZwei::CFG->Lang() == "pt") break;
			_3DZwei::CFG->Lang("pt");
			Lang::Load();
			break;

		case 9: // Русский.
			if (_3DZwei::CFG->Lang() == "ru") break;
			_3DZwei::CFG->Lang("ru");
			Lang::Load();
			break;
	}

	this->Done = true;
}


void LanguageSelector::Cancel() { this->Done = true; } // Cancel Selection.


/* Main Language Selection Logic. */
void LanguageSelector::Action() {
	Pointer::SetPos(0, 0);

	while(aptMainLoop() && !this->FullDone) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		Gui::DrawStringCentered(0 - this->Delay, 3, 0.6f, TEXT_WHITE, Lang::Get("LANG_SELECTOR_TITLE"), 395);
		Gui::DrawSprite(GFX::Sprites, sprites_logo_idx, 72 - this->Delay, 69); // Display Logo.

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->Delay > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->Delay));
		}

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.

		/* Bruh. */
		Gui::DrawString(this->Positions[0].X - 70 - this->Delay, this->Positions[0].Y + 5, 0.5f, TEXT_WHITE, "Bruh");
		GFX::DrawCheckbox(this->Positions[0].X - this->Delay, this->Positions[0].Y, _3DZwei::CFG->Lang() == "br");

		/* Deutsch. */
		Gui::DrawString(this->Positions[1].X - 70 - this->Delay, this->Positions[1].Y + 5, 0.5f, TEXT_WHITE, "Deutsch");
		GFX::DrawCheckbox(this->Positions[1].X - this->Delay, this->Positions[1].Y, _3DZwei::CFG->Lang() == "de");

		/* English. */
		Gui::DrawString(this->Positions[2].X - 70 - this->Delay, this->Positions[2].Y + 5, 0.5f, TEXT_WHITE, "English");
		GFX::DrawCheckbox(this->Positions[2].X - this->Delay, this->Positions[2].Y, _3DZwei::CFG->Lang() == "en");

		/* Español. */
		Gui::DrawString(this->Positions[3].X - 70 - this->Delay, this->Positions[3].Y + 5, 0.5f, TEXT_WHITE, "Español");
		GFX::DrawCheckbox(this->Positions[3].X - this->Delay, this->Positions[3].Y, _3DZwei::CFG->Lang() == "es");

		/* Français. */
		Gui::DrawString(this->Positions[4].X - 70 - this->Delay, this->Positions[4].Y + 5, 0.5f, TEXT_WHITE, "Français");
		GFX::DrawCheckbox(this->Positions[4].X - this->Delay, this->Positions[4].Y, _3DZwei::CFG->Lang() == "fr");

		/* Italiano. */
		Gui::DrawString(this->Positions[5].X - 70 + this->Delay, this->Positions[5].Y + 5, 0.5f, TEXT_WHITE, "Italiano");
		GFX::DrawCheckbox(this->Positions[5].X + this->Delay, this->Positions[5].Y, _3DZwei::CFG->Lang() == "it");

		/* 日本語. */
		Gui::DrawString(this->Positions[6].X - 70 + this->Delay, this->Positions[6].Y + 5, 0.5f, TEXT_WHITE, "日本語");
		GFX::DrawCheckbox(this->Positions[6].X + this->Delay, this->Positions[6].Y, _3DZwei::CFG->Lang() == "ja");

		/* Polski. */
		Gui::DrawString(this->Positions[7].X - 70 + this->Delay, this->Positions[7].Y + 5, 0.5f, TEXT_WHITE, "Polski");
		GFX::DrawCheckbox(this->Positions[7].X + this->Delay, this->Positions[7].Y, _3DZwei::CFG->Lang() == "pl");

		/* Português. */
		Gui::DrawString(this->Positions[8].X - 70 + this->Delay, this->Positions[8].Y + 5, 0.5f, TEXT_WHITE, "Português");
		GFX::DrawCheckbox(this->Positions[8].X + this->Delay, this->Positions[8].Y, _3DZwei::CFG->Lang() == "pt");

		/* Русский. */
		Gui::DrawString(this->Positions[9].X - 70 + this->Delay, this->Positions[9].Y + 5, 0.5f, TEXT_WHITE, "Русский");
		GFX::DrawCheckbox(this->Positions[9].X + this->Delay, this->Positions[9].Y, _3DZwei::CFG->Lang() == "ru");

		Gui::Draw_Rect(this->Positions[10].X, this->Positions[10].Y, this->Positions[10].W, this->Positions[10].H, BAR_BLUE); // Back.
		Gui::DrawSprite(GFX::Sprites, sprites_back_btn_idx, this->Positions[10].X, this->Positions[10].Y);

		Pointer::Draw();
		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->Delay > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->Delay));
		}

		C3D_FrameEnd(0);

		if (this->Start) {
			if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade()) this->Delay = 0, this->Start = false;
			else {
				if (this->Cubic < 255.0f) {
					this->Cubic = std::lerp(this->Cubic, 256.0f, 0.1f);
					this->Delay = 255 - this->Cubic;

					if (this->Cubic >= 255.0f) {
						this->Delay = 0, this->Cubic = 0.0f, this->Start = false;
					}
				}
			}

		} else {
			if (this->Done) {
				hidScanInput();
				const uint32_t Down = hidKeysDown();

				if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade() || Down) this->Delay = 255, this->FullDone = true;
				else {
					if (this->Cubic < 255.0f) {
						this->Cubic = std::lerp(this->Cubic, 256.0f, 0.1f);
						this->Delay = this->Cubic;

						if (this->Cubic >= 255.0f) {
							this->Delay = 255, this->Cubic = 0.0f, this->FullDone = true;
						}
					}
				}

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
	}

	Pointer::SetPos(0, 0);
}