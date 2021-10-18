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
#include "SplashOverlay.hpp"


/* STATE 1: Handle Going to first card. */
void SplashOverlay::State1() {
	if (this->X > this->Cards[0].X + 20) {
		this->X -= 2;

		if (this->X != this->Cards[0].X + 20) return;
	}

	if (this->Y > this->Cards[0].Y + 20) {
		this->Y -= 2;

		if (this->Y != this->Cards[0].Y + 20) return;
	}

	this->State = States::ClickOnFirst;
};


/* STATE 2: Handle flip on the first card + Swipe the StackMem Logo. */
void SplashOverlay::State2() {
	if (this->CoreMoved < 160) this->CoreMoved += 2;

	/* Card Flip. */
	if (this->CoreMoved == 160 && !this->FlipDone[0]) {
		if (!this->CardFlipped[0]) { // Scale down the back cover -- first operation.
			if (this->Cubic < 1.0f) {
				this->Cubic = std::lerp(this->Cubic, 2.0f, 0.1f);

				this->CardScale[0] = 1.0f - Cubic;

				if (this->Cubic >= 1.0f) {
					this->Cubic = 0.0f;
					this->CardScale[0] = 0.0f;
					this->CardFlipped[0] = true;
				}
			}

		} else { // Scale up the front cover -- second operation.
			if (this->Cubic < 1.0f) {
				this->Cubic = std::lerp(this->Cubic, 2.0f, 0.1f);
				this->CardScale[0] = this->Cubic;

				if (this->Cubic >= 1.0f) {
					this->Cubic = 0.0f;
					this->CardScale[0] = 1.0f;
					this->FlipDone[0] = true;
				}
			}
		}
	}

	if (this->FlipDone[0] && this->CoreMoved == 160) this->State = States::GotoSecond;
};


/* STATE 3: Handle Going to second card. */
void SplashOverlay::State3() {
	if (this->X < this->Cards[8].X + 20) {
		this->X += 2;

		if (this->X != this->Cards[8].X + 20) return;
	}

	if (this->Y < this->Cards[8].Y + 20) {
		this->Y += 2;

		if (this->Y != this->Cards[8].Y + 20) return;
	}

	this->State = States::ClickOnSecond;
};


/* STATE 4: Swipe the StackMem Logo further, flip the second card and go into the fade-out State. */
void SplashOverlay::State4() {
	if (this->CoreMoved < 320) this->CoreMoved += 2;

	/* Card Flip. */
	if (this->CoreMoved == 320 && !this->FlipDone[1]) {
		if (!this->CardFlipped[1]) { // Scale down the back cover -- first operation.
			if (this->Cubic < 1.0f) {
				this->Cubic = std::lerp(this->Cubic, 2.0f, 0.1f);

				this->CardScale[1] = 1.0f - Cubic;

				if (this->Cubic >= 1.0f) {
					this->Cubic = 0.0f;
					this->CardScale[1] = 0.0f;
					this->CardFlipped[1] = true;
				}
			}

		} else { // Scale up the front cover -- second operation.
			if (this->Cubic < 1.0f) {
				this->Cubic = std::lerp(this->Cubic, 2.0f, 0.1f);
				this->CardScale[1] = this->Cubic;

				if (this->Cubic >= 1.0f) {
					this->Cubic = 0.0f;
					this->CardScale[1] = 1.0f;
					this->FlipDone[1] = true;
				}
			}
		}
	}

	if (this->FlipDone[1] && this->CoreMoved == 320) this->State = States::FadeOut;
};


/* STATE 5: Fade out. */
void SplashOverlay::State5() {
	if (this->FAlpha < 255) this->FAlpha += 2;
	if (this->FAlpha == 255) this->Done = true;
};


void SplashOverlay::StateHandler() {
	if (this->State != States::FadeOut) {
		if (this->FAlpha > 0) this->FAlpha -= 2; // Handle Alpha Fading.
	}

	switch(this->State) {
		case States::GotoFirst:
			this->State1();
			break;

		case States::ClickOnFirst:
			this->State2();
			break;

		case States::GotoSecond:
			this->State3();
			break;

		case States::ClickOnSecond:
			this->State4();
			break;

		case States::FadeOut:
			this->State5();
			break;
	}
};


void SplashOverlay::Action() {
	while(aptMainLoop() && !this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		GFX::DrawTop();

		/* First real card. */
		if (this->CardFlipped[0]) Gui::DrawSprite(GFX::Sprites, sprites_voltcard_idx, this->Cards[0].X + (1.0f - this->CardScale[0]) * 55 / 2, this->Cards[0].Y, this->CardScale[0], 1.0f);
		else Gui::DrawSprite(GFX::Sprites, sprites_icon_idx, this->Cards[0].X + (1.0f - this->CardScale[0]) * 55 / 2, this->Cards[0].Y, this->CardScale[0], 1.0f);

		/* Hidden cards. */
		for (uint8_t Idx = 1; Idx < 8; Idx++) {
			Gui::DrawSprite(GFX::Sprites, sprites_icon_idx, this->Cards[Idx].X, this->Cards[Idx].Y);
		}

		/* Second real card. */
		if (this->CardFlipped[1]) Gui::DrawSprite(GFX::Sprites, sprites_voltcard_idx, this->Cards[8].X + (1.0f - this->CardScale[1]) * 55 / 2, this->Cards[8].Y, this->CardScale[1], 1.0f);
		else Gui::DrawSprite(GFX::Sprites, sprites_icon_idx, this->Cards[8].X + (1.0f - this->CardScale[1]) * 55 / 2, this->Cards[8].Y, this->CardScale[1], 1.0f);

		Gui::DrawSprite(GFX::Sprites, sprites_pointer_idx, this->X, this->Y);
		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		}

		GFX::DrawBottom();
		if (this->State != States::FadeOut) Gui::DrawSprite(GFX::Sprites, sprites_stackmemcore_idx, 32 - this->CoreMoved, 52);
		Gui::DrawSprite(GFX::Sprites, sprites_universal_core_idx, 320 - this->CoreMoved, 56);

		if (this->State == States::FadeOut) { // The last State -> Copyright + StackZ Image.
			Gui::DrawStringCentered(0, 190, 0.55f, TEXT_WHITE, "3DZwei\nCopyright (C) Universal-Team 2020 - 2021");
			Gui::DrawSprite(GFX::Sprites, sprites_stackz_idx, 213, 50);
		}

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		}

		C3D_FrameEnd(0);
		hidScanInput();
		const uint32_t Down = hidKeysDown();
		if (Down) this->Done = true; // Any key -> Skip.
		this->StateHandler();
	}
};