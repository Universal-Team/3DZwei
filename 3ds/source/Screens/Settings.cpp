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

#include "CardSetPreview.hpp"
#include "CharSetPreview.hpp"
#include "Common.hpp"
#include "LanguageSelector.hpp"
#include "Numpad.hpp"
#include "SetSelector.hpp"
#include "Settings.hpp"
#include "SplashOverlay.hpp"
#include "Utils.hpp"

void Settings::Draw(void) const {
	this->DrawTop();
	this->DrawTabs();
};

/* Tab switches. */
void Settings::ConfigTab() { this->Tab = SettingsTab::Configuration; };
void Settings::AppinfoTab() { this->Tab = SettingsTab::AppInfo; };

void Settings::SelectLang() {
	if (this->Tab == SettingsTab::Configuration) {
		std::unique_ptr<LanguageSelector> Ovl = std::make_unique<LanguageSelector>();
		Ovl->Action();
	}
};

void Settings::SelectCardSet() {
	if (this->Tab == SettingsTab::Configuration) {
		std::unique_ptr<SetSelector> Ovl = std::make_unique<SetSelector>();
		const std::string Set = Ovl->Action();

		if (Set != "") {
			std::unique_ptr<CardSetPreview> Prev = std::make_unique<CardSetPreview>(Set);
			const bool Res = Prev->Action();

			if (Res) { // Because the set is good, load it and set it into the Config as active set.
				_3DZwei::CFG->CardSet(Set);
				Utils::LoadCardSet(Set);
				Utils::InitCards();
			}
		}
	}
};

void Settings::SelectCharSet() {
	if (this->Tab == SettingsTab::Configuration) {
		std::unique_ptr<SetSelector> Ovl = std::make_unique<SetSelector>(true);
		const std::string Set = Ovl->Action();

		if (Set != "") {
			std::unique_ptr<CharSetPreview> Prev = std::make_unique<CharSetPreview>(Set);
			const bool Res = Prev->Action();

			if (Res) { // Because the set is good, load it and set it into the Config as active set.
				_3DZwei::CFG->CharSet(Set);
				Utils::LoadCharSet(Set);
			}
		}
	}
};

void Settings::ToggleSplash() {
	if (this->Tab == SettingsTab::Configuration) _3DZwei::CFG->ShowSplash(!_3DZwei::CFG->ShowSplash());
};

void Settings::ShowSplash() {
	if (this->Tab == SettingsTab::Configuration) {
		std::unique_ptr<SplashOverlay> Ovl = std::make_unique<SplashOverlay>();
		Ovl->Action();
	}
};

void Settings::SetPointerSpeed() {
	if (this->Tab == SettingsTab::Configuration) {
		std::unique_ptr<Numpad> Ovl = std::make_unique<Numpad>(3, _3DZwei::CFG->PointerSpeed(), 255, Lang::Get("SETTINGS_POINTER_SPEED_TXT"));
		_3DZwei::CFG->PointerSpeed(Ovl->Action());
	}
};

/* Go back to MainMenu. */
void Settings::Back() {
	Gui::screenBack();
	return;
};


void Settings::Logic(uint32_t Down, uint32_t Held, touchPosition T) {
	Pointer::ScrollHandling(Held); // Pointer Handling.

	if (Down & KEY_B) this->Back();
	if (Down & KEY_L) this->ConfigTab();
	if (Down & KEY_R) this->AppinfoTab();
	if (Down & KEY_SELECT) _3DZwei::CFG->PointerSpeed(4); // Reset.
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
};


void Settings::DrawTop() const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, "3DZwei", 395);
	Gui::DrawString(340, 3, 0.6f, TEXT_COLOR, V_STRING);
	Gui::DrawSprite(GFX::Sprites, sprites_stackz_idx, 10, 62);
	Gui::DrawSprite(GFX::Sprites, sprites_logo_idx, 130, 58);
};

void Settings::DrawTabs() const {
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 160, 20, (this->Tab == SettingsTab::Configuration ? KBD_KEYPRESSED : KBD_KEYUNPRESSED));
	Gui::Draw_Rect(160, 0, 160, 20, (this->Tab == SettingsTab::AppInfo ? KBD_KEYPRESSED : KBD_KEYUNPRESSED));

	/* Draw Tab Names. */
	for (uint8_t Idx = 0; Idx < 2; Idx++) {
		Gui::DrawStringCentered((Idx ? + 80 : - 80), 3, 0.45f, TEXT_COLOR, Lang::Get(this->TabNames[Idx]), 150);
	}

	switch(this->Tab) {
		case SettingsTab::Configuration:
			/* Language. */
			Gui::DrawString(30, this->Positions[2].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_LANGUAGE"), 100);
			Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, this->Positions[2].X, this->Positions[2].Y);

			/* Current Language Display. */
			if (_3DZwei::CFG->Lang() == "en") Gui::DrawString(200, this->Positions[2].Y + 5, 0.4f, TEXT_COLOR, "English"); // English.
			else if (_3DZwei::CFG->Lang() == "de") Gui::DrawString(200, this->Positions[2].Y + 5, 0.4f, TEXT_COLOR, "Deutsch"); // Deutsch.

			/* Active Cardset. */
			Gui::DrawString(30, this->Positions[3].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_ACTIVE_CARDSET"), 100);
			Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, this->Positions[3].X, this->Positions[3].Y);
			Gui::DrawString(200, this->Positions[3].Y + 5, 0.4f, TEXT_COLOR, _3DZwei::CFG->CardSet(), 110);

			/* Active Charset. */
			Gui::DrawString(30, this->Positions[4].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_ACTIVE_CHARSET"), 100);
			Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, this->Positions[4].X, this->Positions[4].Y);
			Gui::DrawString(200, this->Positions[4].Y + 5, 0.4f, TEXT_COLOR, _3DZwei::CFG->CharSet(), 110);

			/* If showing splash on startup, or nah. */
			Gui::DrawString(30, this->Positions[5].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_SHOW_SPLASH"), 100);
			GFX::DrawCheckbox(this->Positions[5].X, this->Positions[6].Y, _3DZwei::CFG->ShowSplash());
			Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, this->Positions[6].X, this->Positions[6].Y);

			/* Pointer Speed. */
			Gui::DrawString(30, this->Positions[7].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_POINTER_SPEED"), 100);
			Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, this->Positions[7].X, this->Positions[7].Y);
			Gui::DrawString(200, this->Positions[7].Y + 5, 0.4f, TEXT_COLOR, std::to_string(_3DZwei::CFG->PointerSpeed()), 110);
			break;

		case SettingsTab::AppInfo:
			Gui::DrawString(10, 50, 0.5f, TEXT_COLOR, Lang::Get("SETTINGS_BUILD_VERSION") + std::string(V_STRING));
			Gui::DrawString(10, 80, 0.5f, TEXT_COLOR, Lang::Get("SETTINGS_BUILD_HASH") + std::string(V_SHA));
			Gui::DrawStringCentered(0, 110, 0.6f, TEXT_COLOR, Lang::Get("SETTINGS_LATEST_CHANGES"));

			/* Change this, after changes. */
			Gui::DrawStringCentered(0, 130, 0.4f, TEXT_COLOR,
				"* [CORE]: Initial StackMemCore.\n"
				"* [APP]: UI Changes.",
			310);
			break;
	}

	/* Back Icon. */
	Gui::Draw_Rect(this->Positions[8].X, this->Positions[8].Y, this->Positions[8].W, this->Positions[8].H, KBD_KEYPRESSED);
	Gui::DrawSprite(GFX::Sprites, sprites_back_btn_idx, this->Positions[8].X, this->Positions[8].Y);

	Pointer::Draw();
};