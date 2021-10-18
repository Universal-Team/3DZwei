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

#include "CardSelector.hpp"
#include "Common.hpp"
#include "Utils.hpp"


CardSelector::CardSelector() {
	for (size_t Idx = 0; Idx < Utils::GetCardSheetSize(); Idx++) this->Cards.push_back(false);

	/* Enable all current enabled cards. */
	if (!Utils::Cards.empty()) {
		for (size_t Idx = 0; Idx < Utils::Cards.size(); Idx++) this->Cards[Utils::Cards[Idx]] = true;
	}
};


/* Toggle the specified indexes card. */
void CardSelector::ToggleCard(const uint8_t Idx) {
	if (((this->Page * 9) + Idx) < this->Cards.size()) {
		this->Cards[(this->Page * 9) + Idx] = !this->Cards[(this->Page * 9) + Idx];
		if (!this->Changed) this->Changed = true;
	}
};


/* Select ALL cards. */
void CardSelector::SelectAll() {
	for (size_t Idx = 0; Idx < this->Cards.size(); Idx++) this->Cards[Idx] = true;
	if (!this->Changed) this->Changed = true;
};


/* De-Select ALL cards. */
void CardSelector::SelectNone() {
	for (size_t Idx = 0; Idx < this->Cards.size(); Idx++) this->Cards[Idx] = false;
	if (!this->Changed) this->Changed = true;
};


/* Go to the previous page. */
void CardSelector::PrevPage() {
	if (this->Page > 0) {
		this->SwipeDir = true;
		this->DoSwipe = true;
	}
};


/* Go to the next page. */
void CardSelector::NextPage() {
	if (this->CanGoNext()) {
		this->SwipeDir = false;
		this->DoSwipe = true;
	}
};


/* Give the OK state. */
void CardSelector::OK() { this->Done = true; };


/* Return, if a next page is available. */
bool CardSelector::CanGoNext() const {
	return (((this->Page * 9) + 9) < this->Cards.size());
};


/* Handle Page Switches + Fades. */
void CardSelector::PageFadeHandler() {
	hidScanInput();
	const uint32_t Down = hidKeysDown();
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
	}

	/* Page Swipe Handler. */
	if (this->DoSwipe) {
		if (!_3DZwei::CFG->DoAnimation() || Repeat) {
			this->CurPos = 0.0f, this->PrevPos = -400.0f, this->NextPos = 400.0f;
			this->Cubic = 0.0f;
			this->DoSwipe = false;

			this->Page = (this->SwipeDir ? (this->Page - 1) : (this->Page + 1));
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.2f);

			if (this->SwipeDir) { // -> (Last).
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

				this->Page = (this->SwipeDir ? (this->Page - 1) : (this->Page + 1));
			}
		}
	}
};


/* Draw the top. */
void CardSelector::DrawTop() {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 3, 0.6f, TEXT_WHITE, Lang::Get("CARD_SELECTOR_TITLE"), 395);

	/* Current Page. */
	if (this->DoSwipe || this->InitialSwipe) {
		for (size_t Idx = (this->Page * 9), Idx2 = 0; Idx < (this->Page * 9) + 9 && Idx < this->Cards.size(); Idx++, Idx2++) {
			Gui::DrawSprite(GFX::Cards, Idx, this->Positions[Idx2 + 2].X + this->CurPos, this->Positions[Idx2 + 2].Y);
			GFX::DrawCheckbox(this->Positions[Idx2 + 11].X + this->CurPos, this->Positions[Idx2 + 11].Y, this->Cards[Idx]);
		}

		/* Prev Page. */
		if (this->Page >= 1) {
			for (size_t Idx = ((this->Page - 1) * 9), Idx2 = 0; Idx < ((this->Page - 1) * 9) + 9 && Idx < this->Cards.size(); Idx++, Idx2++) {
				Gui::DrawSprite(GFX::Cards, Idx, this->Positions[Idx2 + 2].X + this->PrevPos, this->Positions[Idx2 + 2].Y);
				GFX::DrawCheckbox(this->Positions[Idx2 + 11].X + this->PrevPos, this->Positions[Idx2 + 11].Y, this->Cards[Idx]);
			}
		}

		/* Next Page. */
		if (this->CanGoNext()) {
			for (size_t Idx = ((this->Page + 1) * 9), Idx2 = 0; Idx < ((this->Page + 1) * 9) + 9 && Idx < this->Cards.size(); Idx++, Idx2++) {
				Gui::DrawSprite(GFX::Cards, Idx, this->Positions[Idx2 + 2].X + this->NextPos, this->Positions[Idx2 + 2].Y);
				GFX::DrawCheckbox(this->Positions[Idx2 + 11].X + this->NextPos, this->Positions[Idx2 + 11].Y, this->Cards[Idx]);
			}
		}

	} else { // No switch in progress, display normally.
		for (size_t Idx = (this->Page * 9), Idx2 = 0; Idx < (this->Page * 9) + 9 && Idx < this->Cards.size(); Idx++, Idx2++) {
			Gui::DrawSprite(GFX::Cards, Idx, this->Positions[Idx2 + 2].X, this->Positions[Idx2 + 2].Y);
			GFX::DrawCheckbox(this->Positions[Idx2 + 11].X, this->Positions[Idx2 + 11].Y, this->Cards[Idx]);
		}
	}

	GFX::DrawCornerEdge(true, this->Positions[0].X, this->Positions[0].Y, this->Positions[0].H, this->Page > 0);
	GFX::DrawCornerEdge(false, this->Positions[1].X, this->Positions[1].Y, this->Positions[1].H, this->CanGoNext());
	Pointer::Draw();

	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));
	}
};


/* Draw the Bottom. */
void CardSelector::DrawBottom() {
	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.

	GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, this->Page > 0);
	GFX::DrawCornerEdge(false, this->BottomPos[1].X, this->BottomPos[1].Y, this->BottomPos[1].H, this->CanGoNext());

	/* Draw the Checkboxes. */
	for (size_t Idx = (this->Page * 9), Idx2 = 0; Idx < (this->Page * 9) + 9 && Idx < this->Cards.size(); Idx++, Idx2++) {
		GFX::DrawCheckbox(this->BottomPos[Idx2 + 2].X, this->BottomPos[Idx2 + 2].Y, this->Cards[Idx]);
	}

	Gui::Draw_Rect(this->BottomPos[11].X, this->BottomPos[11].Y, this->BottomPos[11].W, this->BottomPos[11].H, BAR_BLUE);
	Gui::DrawStringCentered(0, this->BottomPos[11].Y + 3, 0.6f, TEXT_WHITE, Lang::Get("OK"));

	/* Select All / Deselect All. */
	Gui::Draw_Rect(this->BottomPos[12].X, this->BottomPos[12].Y, this->BottomPos[12].W, this->BottomPos[12].H, BAR_BLUE);
	Gui::DrawStringCentered(-60, this->BottomPos[12].Y + 5, 0.5f, TEXT_WHITE, Lang::Get("SELECT_ALL"), 100);
	Gui::Draw_Rect(this->BottomPos[13].X, this->BottomPos[13].Y, this->BottomPos[13].W, this->BottomPos[13].H, BAR_BLUE);
	Gui::DrawStringCentered(60, this->BottomPos[13].Y + 5, 0.5f, TEXT_WHITE, Lang::Get("DESELECT_ALL"), 100);

	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
	}
};


/* Main Action Logic. */
void CardSelector::Action() {
	Pointer::OnTop = true;
	Pointer::SetPos(0, 0);

	while(aptMainLoop() && !this->FullDone) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		this->DrawTop();
		this->DrawBottom();
		C3D_FrameEnd(0);

		if (this->FadeIn || this->Done || this->DoSwipe || this->InitialSwipe) this->PageFadeHandler();
		else {
			hidScanInput();
			touchPosition T;
			hidTouchRead(&T);
			const uint32_t Down = hidKeysDown();
			const uint32_t Held = hidKeysHeld();
			const uint32_t Repeat = hidKeysDownRepeat();
			Pointer::ScrollHandling(Held);

			if (Repeat & KEY_L) this->PrevPage();
			if (Repeat & KEY_R) this->NextPage();

			if (Down & KEY_Y) this->SelectAll();
			if (Down & KEY_X) this->SelectNone();

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

			if (Down & KEY_START) this->OK();
		}
	}

	/* Set the cards now, that we finished. */
	if (this->Changed) {
		Utils::Cards.clear();

		for (size_t Idx = 0; Idx < this->Cards.size(); Idx++) {
			/* If card enabled -> Push. */
			if (this->Cards[Idx]) Utils::Cards.push_back(Idx);
		}

		_3DZwei::CFG->ActivatedCards(Utils::Cards); // Set the activated cards to config.
	}

	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);
};