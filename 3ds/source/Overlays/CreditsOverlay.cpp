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


/* Go to the previous Page. */
void CreditsOverlay::PrevPage() {
	if (this->Page > 0) {
		this->SwipeDirection = true;
		this->DoSwipe = true;
	}
};


/* Go to the next Page. */
void CreditsOverlay::NextPage() {
	if (this->Page < 6) {
		this->SwipeDirection = false;
		this->DoSwipe = true;
	}
};


/* Give an OK. */
void CreditsOverlay::OK() { this->Done = true; };


/*
	Draws a page.

	const int16_t Pg: The page to draw.
	const int AddOffs: In case for swipes, the add offset.
*/
void CreditsOverlay::DrawPage(const int16_t Pg, const int AddOffs) {
	switch(Pg) {
		case 0: // Contributors.
			Gui::DrawStringCentered(0 + AddOffs, 3, 0.6f, TEXT_WHITE, Lang::Get("CREDITS_CONTRIBUTORS"), 380);

			Gui::DrawString(40 + AddOffs, 50, 0.5f, TEXT_WHITE, "devkitPro: ");
			Gui::DrawStringCentered(0 + AddOffs, 70, 0.45f, TEXT_WHITE, "devkitARM, libctru, citro2d, citro3d.", 300, 20, nullptr, C2D_WordWrap);

			Gui::DrawString(40 + AddOffs, 100, 0.5f, TEXT_WHITE, "SuperSaiyajinStackZ: ");
			Gui::DrawStringCentered(0 + AddOffs, 120, 0.45f, TEXT_WHITE, Lang::Get("CREDITS_CONTRIBUTORS_SZ"), 300, 20, nullptr, C2D_WordWrap);

			Gui::DrawString(40 + AddOffs, 150, 0.5f, TEXT_WHITE, "Universal-Team: ");
			Gui::DrawStringCentered(0 + AddOffs, 170, 0.45f, TEXT_WHITE, "Universal-Core.", 300, 20, nullptr, C2D_WordWrap);
			break;

		case 1: // StackMem-Core.
			Gui::DrawStringCentered(0 + AddOffs, 3, 0.6f, TEXT_WHITE, "StackMem-Core", 380);
			Gui::DrawSprite(GFX::Sprites, sprites_stackmemcore_idx, 40 + AddOffs, 60);
			break;

		case 2: // Universal-Core.
			Gui::DrawStringCentered(0 + AddOffs, 3, 0.6f, TEXT_WHITE, "Universal-Core", 380);
			Gui::DrawSprite(GFX::Sprites, sprites_universal_core_idx, 92 + AddOffs, 69);
			break;

		case 3: // Translators 1 / 3.
			Gui::DrawStringCentered(0 + AddOffs, 3, 0.6f, TEXT_WHITE, Lang::Get("CREDITS_TRANSLATORS") + " 1 / 3", 380);

			Gui::DrawString(40 + AddOffs, 60, 0.5f, TEXT_WHITE, "Bruh: ");
			Gui::DrawStringCentered(0 + AddOffs, 80, 0.45f, TEXT_WHITE, "Dhalian", 300, 20, nullptr, C2D_WordWrap);

			Gui::DrawString(40 + AddOffs, 100, 0.5f, TEXT_WHITE, "Deutsch: ");
			Gui::DrawStringCentered(0 + AddOffs, 120, 0.45f, TEXT_WHITE, "Crafto1337, SuperSaiyajinStackZ", 300, 20, nullptr, C2D_WordWrap);

			Gui::DrawString(40 + AddOffs, 140, 0.5f, TEXT_WHITE, "English: ");
			Gui::DrawStringCentered(0 + AddOffs, 160, 0.45f, TEXT_WHITE, "Pk11, SuperSaiyajinStackZ", 300, 20, nullptr, C2D_WordWrap);

			Gui::DrawString(40 + AddOffs, 180, 0.5f, TEXT_WHITE, "Español: ");
			//Gui::DrawStringCentered(0 + AddOffs, 200, 0.45f, TEXT_WHITE, "", 300, 20, nullptr, C2D_WordWrap);
			break;

		case 4: // Translators 2 / 3.
			Gui::DrawStringCentered(0 + AddOffs, 3, 0.6f, TEXT_WHITE, Lang::Get("CREDITS_TRANSLATORS") + " 2 / 3", 380);

			Gui::DrawString(40 + AddOffs, 60, 0.5f, TEXT_WHITE, "Français: ");
			Gui::DrawStringCentered(0 + AddOffs, 80, 0.45f, TEXT_WHITE, "LinuxCat", 300, 20, nullptr, C2D_WordWrap);

			Gui::DrawString(40 + AddOffs, 100, 0.5f, TEXT_WHITE, "Italiano: ");
			//Gui::DrawStringCentered(0 + AddOffs, 120, 0.45f, TEXT_WHITE, "", 300, 20, nullptr, C2D_WordWrap);

			Gui::DrawString(40 + AddOffs, 140, 0.5f, TEXT_WHITE, "Polski: ");
			//Gui::DrawStringCentered(0 + AddOffs, 160, 0.45f, TEXT_WHITE, "", 300, 20, nullptr, C2D_WordWrap);

			Gui::DrawString(40 + AddOffs, 180, 0.5f, TEXT_WHITE, "Português: ");
			//Gui::DrawStringCentered(0 + AddOffs, 200, 0.45f, TEXT_WHITE, "", 300, 20, nullptr, C2D_WordWrap);
			break;

		case 5: // Translators 3 / 3.
			Gui::DrawStringCentered(0 + AddOffs, 3, 0.6f, TEXT_WHITE, Lang::Get("CREDITS_TRANSLATORS") + " 3 / 3", 380);

			Gui::DrawString(40 + AddOffs, 60, 0.5f, TEXT_WHITE, "Русский: ");
			//Gui::DrawStringCentered(0 + AddOffs, 80, 0.45f, TEXT_WHITE, "", 300, 20, nullptr, C2D_WordWrap);

			Gui::DrawString(40 + AddOffs, 100, 0.5f, TEXT_WHITE, "日本語: ");
			Gui::DrawStringCentered(0 + AddOffs, 120, 0.45f, TEXT_WHITE, "Pk11", 300, 20, nullptr, C2D_WordWrap);
			break;

		case 6: // Special Thanks.
			Gui::DrawStringCentered(0 + AddOffs, 3, 0.6f, TEXT_WHITE, Lang::Get("CREDITS_SPECIAL_THANKS"), 380);

			Gui::DrawString(40 + AddOffs, 40, 0.5f, TEXT_WHITE, "NightScript: ");
			Gui::DrawStringCentered(0 + AddOffs, 60, 0.45f, TEXT_WHITE, Lang::Get("CREDITS_SPECIAL_THANKS_NS"), 300, 20, nullptr, C2D_WordWrap);

			Gui::DrawString(40 + AddOffs, 100, 0.5f, TEXT_WHITE, "Pk11: ");
			Gui::DrawStringCentered(0 + AddOffs, 120, 0.45f, TEXT_WHITE, Lang::Get("CREDITS_SPECIAL_THANKS_PK11"), 300, 20, nullptr, C2D_WordWrap);

			Gui::DrawString(40 + AddOffs, 160, 0.5f, TEXT_WHITE, "Universal-Microwave: ");
			Gui::DrawStringCentered(0 + AddOffs, 180, 0.45f, TEXT_WHITE, Lang::Get("CREDITS_SPECIAL_THANKS_UM"), 300, 20, nullptr, C2D_WordWrap);
	};
};


/* Main Action. */
void CreditsOverlay::Action() {
	Pointer::OnTop = true;
	Pointer::SetPos(0, 0);

	while(aptMainLoop() && !this->FullDone) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		/* Draw Content. */
		if (this->DoSwipe || this->InitialSwipe) { // We swipe.
			this->DrawPage(this->Page, this->CurPos); // Draw current page.

			if (this->SwipeDirection) this->DrawPage(this->Page - 1, this->PrevPos);
			else this->DrawPage(this->Page + 1, this->NextPos);

		} else { // No swipe.
			this->DrawPage(this->Page, 0); // Draw current page only.
		}

		GFX::DrawCornerEdge(true, this->Positions[0].X, this->Positions[0].Y, this->Positions[0].H, this->Page > 0);
		GFX::DrawCornerEdge(false, this->Positions[1].X, this->Positions[1].Y, this->Positions[1].H, this->Page < 6);
		Pointer::Draw();

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		};

		/* Bottom. */
		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.
		GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, this->Page > 0);
		GFX::DrawCornerEdge(false, this->BottomPos[2].X, this->BottomPos[2].Y, this->BottomPos[2].H, this->Page < 6);

		Gui::Draw_Rect(95, 105, 130, 30, BAR_BLUE);
		Gui::Draw_Rect(this->BottomPos[1].X, this->BottomPos[1].Y, this->BottomPos[1].W, this->BottomPos[1].H, BG_BLUE);
		Gui::DrawStringCentered(0, this->BottomPos[1].Y + 3, 0.5f, TEXT_WHITE, Lang::Get("OK"));

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		};
		C3D_FrameEnd(0);
		this->Handler();
	}

	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);
};


/* Action Handler. */
void CreditsOverlay::Handler() {
	hidScanInput();
	touchPosition T;
	hidTouchRead(&T);
	const uint32_t Down = hidKeysDown();
	const uint32_t Held = hidKeysHeld();
	const uint32_t Repeat = hidKeysDownRepeat();

	/* Fade-In Handler. */
	if (this->FadeIn) {
		if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade()) this->FAlpha = 0, this->FadeIn = false;

		if (this->FAlpha > 0) {
			this->FAlpha -= 5;

			if (this->FAlpha <= 0) this->FadeIn = false;
		}
	};

	/* Fade-Out Handler. */
	if (this->Done) {
		if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade() || Down) this->FullDone = true;

		if (this->FAlpha < 255) {
			this->FAlpha += 5;

			if (this->FAlpha >= 255) this->FullDone = true;
		}
	};

	/* Initial Swipe. */
	if (this->InitialSwipe) {
		if (!_3DZwei::CFG->DoAnimation()) {
			this->InitialSwipe = false;
			this->CurPos = 0.0f;
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.2f);
			this->CurPos = -400 + this->Cubic;

			if (this->Cubic >= 400.0f) {
				this->CurPos = 0.0f;
				this->Cubic = 0.0f;
				this->InitialSwipe = false;
			}
		}

		return;
	};

	/* Swipe Logic. */
	if (this->DoSwipe) {
		if (!_3DZwei::CFG->DoAnimation() || Repeat) {
			this->CurPos = 0.0f, this->PrevPos = -400.0f, this->NextPos = 400.0f;
			this->Cubic = 0.0f;
			this->DoSwipe = false;

			this->Page = (this->SwipeDirection ? (this->Page - 1) : (this->Page + 1));
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.2f);

			if (this->SwipeDirection) { // -> (Last).
				this->CurPos = this->Cubic;
				this->PrevPos = -400 + this->Cubic;

			} else { // <- (Next).
				this->CurPos = 0 - this->Cubic;
				this->NextPos = 400 - this->Cubic;
			}

			if (this->Cubic >= 400.0f) {
				this->CurPos = 0.0f, this->PrevPos = -400.0f, this->NextPos = 400.0f;
				this->Cubic = 0.0f;
				this->DoSwipe = false;

				this->Page = (this->SwipeDirection ? (this->Page - 1) : (this->Page + 1));
			}
		}

		return;
	};

	Pointer::ScrollHandling(Held);
	if (Repeat & KEY_L) this->PrevPage();
	if (Repeat & KEY_R) this->NextPage();

	if (Repeat & KEY_A) {
		for (auto &Position : this->Positions) {
			if (Pointer::Clicked(Position, true)) break;
		}
	}

	if (Repeat & KEY_TOUCH) {
		for (auto &Position : this->BottomPos) {
			if (Touched(Position, T, true)) break;
		}
	}

	if (Down & KEY_START || Down & KEY_B) this->OK(); // Exit with START or B as well.
};