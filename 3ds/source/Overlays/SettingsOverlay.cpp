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

#include "CardSetSelector.hpp"
#include "CharacterSetSelector.hpp"
#include "Common.hpp"
#include "LanguageSelector.hpp"
#include "Numpad.hpp"
#include "SettingsOverlay.hpp"
#include "SplashOverlay.hpp"
#include "Utils.hpp"


void SettingsOverlay::Action() {
	while(aptMainLoop() && !this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		this->DrawTop();
		this->DrawTabs();
		C3D_FrameEnd(0);

		/* Tab Animation handler. */
		if (this->DoSwipe || this->InitialSwipe) {
			this->TabHandler();

		} else {
			hidScanInput();
			touchPosition T;
			const uint32_t Down = hidKeysDown();
			const uint32_t Held = hidKeysHeld();
			hidTouchRead(&T);
			Pointer::ScrollHandling(Held); // Pointer Handling.

			if (Down & KEY_B) this->Back();
			if (Down & KEY_SELECT) _3DZwei::CFG->PointerSpeed(4); // Reset.
			if (Down & KEY_A) {
				for (auto &Position : (this->Tab == SettingsTab::Configuration ? this->Positions : this->AnimPos)) {
					if (Pointer::Clicked(Position, true)) break;
				}
			}

			if (Down & KEY_TOUCH) {
				for (auto &Position : (this->Tab == SettingsTab::Configuration ? this->Positions : this->AnimPos)) {
					if (Touched(Position, T, true)) break;
				}
			}
		}
	};
};


/* Switch to the config tab. */
void SettingsOverlay::ConfigTab() {
	if (this->Tab != SettingsTab::Configuration) {
		this->SwipeDir = false; // We are going back.

		this->ToSwipe = (this->Tab == SettingsTab::Animation ? 320.0f : 640.0f);
		this->DoSwipe = true;
	}
};


/* Switch to the app info tab. */
void SettingsOverlay::AppinfoTab() {
	if (this->Tab != SettingsTab::AppInfo) {
		this->SwipeDir = true; // We are going forward.

		this->ToSwipe = (this->Tab == SettingsTab::Configuration ? 640.0f : 320.0f);
		this->DoSwipe = true;
	}
};


/* Switch to the animation tab. */
void SettingsOverlay::AnimationTab() {
	if (this->Tab != SettingsTab::Animation) {
		this->SwipeDir = (this->Tab == SettingsTab::Configuration ? true : false);

		this->ToSwipe = 320.0f;
		this->DoSwipe = true;
	}
};


/* Select the Language. */
void SettingsOverlay::SelectLang() {
	if (this->Tab == SettingsTab::Configuration) {
		this->FadeOutHandler();

		std::unique_ptr<LanguageSelector> Ovl = std::make_unique<LanguageSelector>();
		Ovl->Action();

		this->FadeInHandler();
	}
};


/* Select a cardset. */
void SettingsOverlay::SelectCardSet() {
	if (this->Tab == SettingsTab::Configuration) {
		this->FadeOutHandler();

		std::unique_ptr<CardSetSelector> Ovl = std::make_unique<CardSetSelector>();
		const std::string Set = Ovl->Action();

		if (Set != "") {
			_3DZwei::CFG->CardSet(Set);
			Utils::LoadCardSet(Set);
			Utils::InitNewCardSheet();
		}

		this->FadeInHandler();
	}
};


/* Select a character set. */
void SettingsOverlay::SelectCharSet() {
	if (this->Tab == SettingsTab::Configuration) {
		this->FadeOutHandler();

		std::unique_ptr<CharacterSetSelector> Ovl = std::make_unique<CharacterSetSelector>();
		const std::string Set = Ovl->Action();

		if (Set != "") {
			_3DZwei::CFG->CharSet(Set);
			Utils::LoadCharSet(Set);
		}

		this->FadeInHandler();
	}
};


/* Toggle if the splash is shown at startup. */
void SettingsOverlay::ToggleSplash() {
	if (this->Tab == SettingsTab::Configuration) _3DZwei::CFG->ShowSplash(!_3DZwei::CFG->ShowSplash());
};


/* Display the Splash cause why not. */
void SettingsOverlay::ShowSplash() {
	if (this->Tab == SettingsTab::Configuration) {
		this->FadeOutHandler();

		std::unique_ptr<SplashOverlay> Ovl = std::make_unique<SplashOverlay>();
		Ovl->Action();

		this->FadeInHandler();
	}
};


/* Set the Pointer movement speed. */
void SettingsOverlay::SetPointerSpeed() {
	if (this->Tab == SettingsTab::Configuration) {
		std::unique_ptr<Numpad> Ovl = std::make_unique<Numpad>(3, _3DZwei::CFG->PointerSpeed(), 255, Lang::Get("SETTINGS_POINTER_SPEED_TXT"));
		_3DZwei::CFG->PointerSpeed(Ovl->Action());
	}
};


/* Toggle if animations are allowed. */
void SettingsOverlay::ToggleAnimation() {
	if (this->Tab == SettingsTab::Animation) _3DZwei::CFG->DoAnimation(!_3DZwei::CFG->DoAnimation());
};


/* Change the Game-Start Animation. */
void SettingsOverlay::SwitchGameAnim() {
	if (this->Tab == SettingsTab::Animation && _3DZwei::CFG->DoAnimation()) {
		if (_3DZwei::CFG->GameAnimation() < 2) _3DZwei::CFG->GameAnimation(_3DZwei::CFG->GameAnimation() + 1);
		else _3DZwei::CFG->GameAnimation(0);
	};
};


/* Toggle if allowing to visually switch the game pages. */
void SettingsOverlay::TogglePageSwitch() {
	if (this->Tab == SettingsTab::Animation) _3DZwei::CFG->PageSwitch(!_3DZwei::CFG->PageSwitch());
};


/* Toggle Fade Effects. */
void SettingsOverlay::ToggleFade() {
	if (this->Tab == SettingsTab::Animation) _3DZwei::CFG->DoFade(!_3DZwei::CFG->DoFade());
};


/* Go back to the Main 3DZwei Overlay. */
void SettingsOverlay::Back() {
	this->FadeOutHandler();
	this->Done = true;
};


/* Draw the Top. */
void SettingsOverlay::DrawTop() const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, "3DZwei", 395);
	Gui::DrawString(340, 3, 0.6f, TEXT_COLOR, V_STRING);
	Gui::DrawSprite(GFX::Sprites, sprites_stackz_idx, 10, 62);
	Gui::DrawSprite(GFX::Sprites, sprites_logo_idx, 130, 58);

	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));
	}
};


/* Draw the Tabs. */
void SettingsOverlay::DrawTabs() const {
	GFX::DrawBottom();

	if (this->Tab != SettingsTab::Configuration) Gui::Draw_Rect(1, 0, 106, 20, KBD_KEYUNPRESSED);
	if (this->Tab != SettingsTab::Animation)  Gui::Draw_Rect(107, 0, 106, 20, KBD_KEYUNPRESSED);
	if (this->Tab != SettingsTab::AppInfo) Gui::Draw_Rect(213, 0, 106, 20, KBD_KEYUNPRESSED);

	/* I'm too lazy to center right now, so TODO. */
	Gui::DrawString(1 + 40, 3, 0.45f, TEXT_COLOR, Lang::Get("SETTINGS_TAB_CONFIG"));
	Gui::DrawString(107 + 19, 3, 0.45f, TEXT_COLOR, Lang::Get("SETTINGS_TAB_ANIMATION"));
	Gui::DrawString(213 + 23, 3, 0.45f, TEXT_COLOR, Lang::Get("SETTINGS_TAB_APPINFO"));

	/* Only draw between -320 and 320. */
	if (this->CurTabOffs[0] >= -320 && this->CurTabOffs[0] <= 320) { // Configuration.
		/* Language. */
		Gui::DrawString(30 + this->CurTabOffs[0], this->Positions[3].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_LANGUAGE"), 100);
		Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, this->Positions[3].X + this->CurTabOffs[0], this->Positions[3].Y);

		/* Current Language Display. */
		if (_3DZwei::CFG->Lang() == "en") Gui::DrawString(200 + this->CurTabOffs[0], this->Positions[3].Y + 5, 0.4f, TEXT_COLOR, "English"); // English.
		else if (_3DZwei::CFG->Lang() == "de") Gui::DrawString(200 + this->CurTabOffs[0], this->Positions[3].Y + 5, 0.4f, TEXT_COLOR, "Deutsch"); // Deutsch.

		/* Active Cardset. */
		Gui::DrawString(30 + this->CurTabOffs[0], this->Positions[4].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_ACTIVE_CARDSET"), 100);
		Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, this->Positions[4].X + this->CurTabOffs[0], this->Positions[4].Y);
		Gui::DrawString(200 + this->CurTabOffs[0], this->Positions[4].Y + 5, 0.4f, TEXT_COLOR, _3DZwei::CFG->CardSet(), 110);

		/* Active Charset. */
		Gui::DrawString(30 + this->CurTabOffs[0], this->Positions[5].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_ACTIVE_CHARSET"), 100);
		Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, this->Positions[5].X + this->CurTabOffs[0], this->Positions[5].Y);
		Gui::DrawString(200 + this->CurTabOffs[0], this->Positions[5].Y + 5, 0.4f, TEXT_COLOR, _3DZwei::CFG->CharSet(), 110);

		/* If showing splash on startup, or nah. */
		Gui::DrawString(30 + this->CurTabOffs[0], this->Positions[6].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_SHOW_SPLASH"), 100);
		GFX::DrawCheckbox(this->Positions[6].X + this->CurTabOffs[0], this->Positions[6].Y, _3DZwei::CFG->ShowSplash());
		Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, this->Positions[7].X + this->CurTabOffs[0], this->Positions[7].Y);

		/* Pointer Speed. */
		Gui::DrawString(30 + this->CurTabOffs[0], this->Positions[8].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_POINTER_SPEED"), 100);
		Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, this->Positions[8].X + this->CurTabOffs[0], this->Positions[8].Y);
		Gui::DrawString(200 + this->CurTabOffs[0], this->Positions[8].Y + 5, 0.4f, TEXT_COLOR, std::to_string(_3DZwei::CFG->PointerSpeed()), 110);
	};

	/* Only draw between -320 and 320. */
	if (this->CurTabOffs[1] >= -320 && this->CurTabOffs[1] <= 320) { // Animations.
		/* If doing Animation or nah. */
		Gui::DrawString(30 + this->CurTabOffs[1], this->AnimPos[3].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_ANIMATION"), 100);
		GFX::DrawCheckbox(this->AnimPos[3].X + this->CurTabOffs[1], this->AnimPos[3].Y, _3DZwei::CFG->DoAnimation());

		if (_3DZwei::CFG->DoAnimation()) { // Only show those options, if Animations are globally enabled.
			/* The Game Animation, which to use. */
			Gui::DrawString(30 + this->CurTabOffs[1], this->AnimPos[4].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_GAME_ANIMATION"), 100);
			Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, this->AnimPos[4].X + this->CurTabOffs[1], this->AnimPos[4].Y);

			switch(_3DZwei::CFG->GameAnimation()) {
				case 0: // Random.
					Gui::DrawString(200 + this->CurTabOffs[1], this->AnimPos[4].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_GAME_ANIMATION_RANDOM"), 110);
					break;

				case 1: // Falling cards.
					Gui::DrawString(200 + this->CurTabOffs[1], this->AnimPos[4].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_GAME_ANIMATION_FALL"), 110);
					break;

				case 2: // Growing Cards.
					Gui::DrawString(200 + this->CurTabOffs[1], this->AnimPos[4].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_GAME_ANIMATION_GROW"), 110);
					break;
			};

			/* Visual Page Switch. */
			Gui::DrawString(30 + this->CurTabOffs[1], this->AnimPos[5].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_PAGE_SWITCH"), 100);
			GFX::DrawCheckbox(this->AnimPos[5].X + this->CurTabOffs[1], this->AnimPos[5].Y, _3DZwei::CFG->PageSwitch());

			/* Do Fade. */
			Gui::DrawString(30 + this->CurTabOffs[1], this->AnimPos[6].Y + 5, 0.4f, TEXT_COLOR, Lang::Get("SETTINGS_FADE"), 100);
			GFX::DrawCheckbox(this->AnimPos[6].X + this->CurTabOffs[1], this->AnimPos[6].Y, _3DZwei::CFG->DoFade());
		}
	};

	/* Only draw between -320 and 320. */
	if (this->CurTabOffs[2] >= -320 && this->CurTabOffs[2] <= 320) { // App Info.
		Gui::DrawString(10 + this->CurTabOffs[2], 50, 0.5f, TEXT_COLOR, Lang::Get("SETTINGS_BUILD_VERSION") + std::string(V_STRING));
		Gui::DrawString(10 + this->CurTabOffs[2], 80, 0.5f, TEXT_COLOR, Lang::Get("SETTINGS_BUILD_HASH") + std::string(V_SHA));
		Gui::DrawStringCentered(0 + this->CurTabOffs[2], 110, 0.6f, TEXT_COLOR, Lang::Get("SETTINGS_LATEST_CHANGES"));

		/* Change this, after changes. */
		Gui::DrawStringCentered(0 + this->CurTabOffs[2], 130, 0.4f, TEXT_COLOR,
			"* Using `std=gnu++20` now.\n"
			"* Using cubic bezier animations.\n"
			"* Massive code overhaul.\n"
			"* Added Special Thanks page to credits.\n"
			"* Fancy Game Animations such as: card flips.\n"
			"* Overhauled Character + Cardset Selectors.\n"
			"* Using system keyboard now for inputs.",
		310);
	};

	/* Back Icon. */
	Gui::Draw_Rect(this->Positions[9].X, this->Positions[9].Y, this->Positions[9].W, this->Positions[9].H, KBD_KEYPRESSED);
	Gui::DrawSprite(GFX::Sprites, sprites_back_btn_idx, this->Positions[9].X, this->Positions[9].Y);

	Pointer::Draw();
	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
	}
};


/* Fade Handlers. */
void SettingsOverlay::FadeOutHandler() {
	if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade()) return;

	this->FadeOut = true;
	this->FAlpha = 0;

	while(aptMainLoop() && this->FadeOut) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		this->DrawTop();
		this->DrawTabs();
		C3D_FrameEnd(0);

		hidScanInput();
		const uint32_t Down = hidKeysDown();
		if (Down) this->FAlpha = 255, this->FadeOut = false;

		if (this->FAlpha < 255) {
			this->FAlpha += 5;

			if (this->FAlpha >= 255) this->FadeOut = false;
		}
	};
};


void SettingsOverlay::FadeInHandler() {
	if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade()) return;

	this->FadeIn = true;
	this->FAlpha = 255;

	while(aptMainLoop() && this->FadeIn) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		this->DrawTop();
		this->DrawTabs();
		C3D_FrameEnd(0);

		if (this->FAlpha > 0) {
			this->FAlpha -= 5;

			if (this->FAlpha <= 0) this->FadeIn = false;
		}
	};
};


void SettingsOverlay::TabHandler() {
	if (this->InitialSwipe) {
		if (!_3DZwei::CFG->DoAnimation()) { // No animation.
			this->InitialSwipe = false;
			this->CurTabOffs[0] = 0.0f;
			this->PrevTabOffs[0] = 0.0f;
			this->ToSwipe = 0.0f, this->Swipe = 0.0f; // Reset.

		} else {
			if (_3DZwei::CFG->DoFade()) {
				if (this->FadeIn) {
					this->FAlpha -= 5;

					if (this->FAlpha <= 0) this->FadeIn = false;
				}

			} else {
				if (this->FadeIn) {
					this->FadeIn = false;
					this->FAlpha = 0;
				}
			}

			if (this->Swipe < 320.0f) {
				this->Swipe = std::lerp(this->Swipe, 320.0f, 0.1f);

				this->CurTabOffs[0] = this->PrevTabOffs[0] + this->Swipe;

				if (this->Swipe >= 319.95f) {
					this->InitialSwipe = false;

					this->CurTabOffs[0] = 0.0f;
					this->PrevTabOffs[0] = 0.0f;
					this->ToSwipe = 0.0f, this->Swipe = 0.0f; // Reset.
				}
			}

			return; // Don't do the below action if initial state.
		}
	};

	hidScanInput();
	const uint32_t Down = hidKeysDown();

	if (this->Swipe < this->ToSwipe) {
		if (!_3DZwei::CFG->DoAnimation() || Down) { // No animation.
			this->DoSwipe = false;

			for (size_t Idx = 0; Idx < 3; Idx++) { // Properly set positions.
				if (this->SwipeDir) { // --;
					this->CurTabOffs[Idx] = this->PrevTabOffs[Idx] - this->ToSwipe;
					this->PrevTabOffs[Idx] = this->CurTabOffs[Idx];

				} else { // ++;
					this->CurTabOffs[Idx] = this->PrevTabOffs[Idx] + this->ToSwipe;
					this->PrevTabOffs[Idx] = this->CurTabOffs[Idx];
				}
			}

			/* We check which Tab we are actually on. */
			if (this->CurTabOffs[0] == 0) this->Tab = SettingsTab::Configuration; // Config.
			else if (this->CurTabOffs[1] == 0) this->Tab = SettingsTab::Animation; // Animation.
			else if (this->CurTabOffs[2] == 0) this->Tab = SettingsTab::AppInfo; // App Info.

			this->ToSwipe = 0.0f, this->Swipe = 0.0f; // Reset.

		} else {
			this->Swipe = std::lerp(this->Swipe, this->ToSwipe, 0.1f);

			if (this->SwipeDir) { // We are going forward, soo --;
				this->CurTabOffs[0] = this->PrevTabOffs[0] - this->Swipe; // Tab 1.
				this->CurTabOffs[1] = this->PrevTabOffs[1] - this->Swipe; // Tab 2.
				this->CurTabOffs[2] = this->PrevTabOffs[2] - this->Swipe; // Tab 3.

			} else { // We are going backwards, soo ++;
				this->CurTabOffs[0] = this->PrevTabOffs[0] + this->Swipe; // Tab 1.
				this->CurTabOffs[1] = this->PrevTabOffs[1] + this->Swipe; // Tab 2.
				this->CurTabOffs[2] = this->PrevTabOffs[2] + this->Swipe; // Tab 3.
			}

			if (this->Swipe >= this->ToSwipe - 0.05f) {
				this->DoSwipe = false;

				for (size_t Idx = 0; Idx < 3; Idx++) { // Properly set positions.
					if (this->SwipeDir) { // --;
						this->CurTabOffs[Idx] = this->PrevTabOffs[Idx] - this->ToSwipe;
						this->PrevTabOffs[Idx] = this->CurTabOffs[Idx];

					} else { // ++;
						this->CurTabOffs[Idx] = this->PrevTabOffs[Idx] + this->ToSwipe;
						this->PrevTabOffs[Idx] = this->CurTabOffs[Idx];
					}
				}

				/* We check which Tab we are actually on. */
				if (this->CurTabOffs[0] == 0) this->Tab = SettingsTab::Configuration; // Config.
				else if (this->CurTabOffs[1] == 0) this->Tab = SettingsTab::Animation; // Animation.
				else if (this->CurTabOffs[2] == 0) this->Tab = SettingsTab::AppInfo; // App Info.

				this->ToSwipe = 0.0f, this->Swipe = 0.0f; // Reset.
			}
		}
	};
};