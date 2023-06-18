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
#include "CharacterSelector.hpp"
#include "Utils.hpp"

#define CHARS_PER_PAGE 15


/*
	Draw a page of 15 characters.

	const size_t Page: The page to draw.
	const int AddOffs: The additional offsets to draw to.
*/
void CharacterSelector::DrawPage(const size_t Page, const int AddOffs) {
	for (size_t Idx = (Page * CHARS_PER_PAGE), Idx2 = 0; Idx < (Page * CHARS_PER_PAGE) + CHARS_PER_PAGE && Idx < Utils::GetCharSheetSize(); Idx++, Idx2++) {
		Gui::DrawSprite(GFX::Characters, Idx, this->Characters[Idx2].X + AddOffs, this->Characters[Idx2].Y, 0.4f, 0.4f);
	}
}


/*
	Main character select logic.

	const uint8_t Idx: The click index.

	If 15: It will randomly select the character.
*/
void CharacterSelector::SelectCharacter(const uint8_t Idx) {
	if (Idx == 15) {
		this->Res = (rand() % (Utils::GetCharSheetSize() - 1));
		this->Done = true;
		return;
	}

	if ((this->Page * CHARS_PER_PAGE) + Idx < Utils::GetCharSheetSize()) {
		this->Res = (this->Page * CHARS_PER_PAGE) + Idx;
		this->Done = true;
	}
}


/* Go to the previous page. */
void CharacterSelector::PrevPage() {
	if (this->Page > 0) {
		this->SwipeDir = true;
		this->DoSwipe = true;
	}
}


/* Go to the next page. */
void CharacterSelector::NextPage() {
	if (this->CanGoNext()) {
		this->SwipeDir = false;
		this->DoSwipe = true;
	}
}


void CharacterSelector::Cancel() { this->Done = true; }


/* Returns, if you can go to the next page. */
bool CharacterSelector::CanGoNext() const {
	return ((this->Page * CHARS_PER_PAGE) + CHARS_PER_PAGE < Utils::GetCharSheetSize());
}


/* Main action. */
int CharacterSelector::Action() {
	if (Utils::GetCharSheetSize() == 0) return 0;

	while(aptMainLoop() && !this->FullDone) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_WHITE, Lang::Get("CHARACTER_SELECT_TITLE"), 395);

		if (this->Res < (int)Utils::GetCharSheetSize()) {
			Gui::DrawSprite(GFX::Characters, this->Res, 100, 40, 1.5f, 1.5f);
		}

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		}

		GFX::DrawBottom();

		if (this->DoSwipe || this->InitialSwipe) { // We swipe.
			this->DrawPage(this->Page, this->CurPos); // Draw current page.

			if (this->SwipeDir) this->DrawPage(this->Page - 1, this->PrevPos);
			else this->DrawPage(this->Page + 1, this->NextPos);

		} else { // No swipe.
			this->DrawPage(this->Page, 0); // Draw current page only.
		}

		Gui::DrawSprite(GFX::Sprites, sprites_random_idx, this->Characters[15].X, this->Characters[15].Y);
		GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, this->Page > 0);
		GFX::DrawCornerEdge(false, this->BottomPos[1].X, this->BottomPos[1].Y, this->BottomPos[1].H, this->CanGoNext());
		Pointer::Draw();

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		}

		C3D_FrameEnd(0);
		this->Handler();
	}

	return this->Res;
}


/* Action Handler. */
void CharacterSelector::Handler() {
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
	}

	/* Fade-Out Handler. */
	if (this->Done) {
		if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade() || Down) this->FullDone = true;

		if (this->FAlpha < 255) {
			this->FAlpha += 5;

			if (this->FAlpha >= 255) this->FullDone = true;
		}
	}

	/* Initial Swipe. */
	if (this->InitialSwipe) {
		if (!_3DZwei::CFG->DoAnimation()) {
			this->InitialSwipe = false, this->CurPos = 0.0f;
			return;
		}

		if (this->Cubic < 320.0f) {
			this->Cubic = std::lerp(this->Cubic, 321.0f, 0.1f);
			this->CurPos = -320 + this->Cubic;

			if (this->Cubic >= 320.0f) {
				this->CurPos = 0.0f, this->Cubic = 0.0f, this->InitialSwipe = false;
			}
		}

		return;
	}

	/* Swipe Logic. */
	if (this->DoSwipe) {
		if (!_3DZwei::CFG->DoAnimation() || Repeat) {
			this->CurPos = 0.0f, this->PrevPos = -320.0f, this->NextPos = 320.0f;
			this->Cubic = 0.0f, this->DoSwipe = false;
			this->Page = (this->SwipeDir ? (this->Page - 1) : (this->Page + 1));
			return;
		}

		if (this->Cubic < 320.0f) {
			this->Cubic = std::lerp(this->Cubic, 321.0f, 0.1f);

			if (this->SwipeDir) this->CurPos = this->Cubic, this->PrevPos = -320 + this->Cubic; // -> (Last).
			else this->CurPos = 0 - this->Cubic, this->NextPos = 320 - this->Cubic; // <- (Next).

			if (this->Cubic >= 320.0f) {
				this->CurPos = 0.0f, this->PrevPos = -320.0f, this->NextPos = 320.0f;
				this->Cubic = 0.0f, this->DoSwipe = false;
				this->Page = (this->SwipeDir ? (this->Page - 1) : (this->Page + 1));
			}
		}

		return;
	}

	Pointer::ScrollHandling(Held);
	if (Down & KEY_B || Down & KEY_START) this->Cancel();
	if (Repeat & KEY_L) this->PrevPage();
	if (Repeat & KEY_R) this->NextPage();

	if (Repeat & KEY_A) {
		bool Clicked = false;
		for (auto &Position : this->BottomPos) {
			if (Pointer::Clicked(Position, true)) {
				Clicked = true;
				break;
			}
		}

		if (Clicked) return;
	}

	if (Down & KEY_A) {
		for (auto &Position : this->Characters) {
			if (Pointer::Clicked(Position, true)) break;
		}
	}

	if (Repeat & KEY_TOUCH) {
		bool Clicked = false;

		for (auto &Position : this->BottomPos) {
			if (Touched(Position, T, true)) {
				Clicked = true;
				break;
			}
		}

		if (Clicked) return;
	}

	if (Down & KEY_TOUCH) {
		for (auto &Position : this->Characters) {
			if (Touched(Position, T, true)) break;
		}
	}
}