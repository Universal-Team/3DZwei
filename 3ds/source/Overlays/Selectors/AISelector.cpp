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

#include "AISelector.hpp"
#include "Common.hpp"


AISelector::AISelector(const StackMem::AIMethod Method) {
	this->Res = Method;

	switch(this->Res) {
		case StackMem::AIMethod::Random:
			this->Mode = 0;
			break;

		case StackMem::AIMethod::Hard:
			this->Mode = 1;
			break;

		case StackMem::AIMethod::Extreme:
			this->Mode = 2;
			break;
	}
};


void AISelector::PrevMode() {
	if (this->Mode > 0) {
		this->SwipeDirection = true;
		this->DoSwipe = true;
	}
};


void AISelector::NextMode() {
	if (this->Mode < 2) {
		this->SwipeDirection = false;
		this->DoSwipe = true;
	}
};

void AISelector::OK() { this->Done = true; };


void AISelector::Cancel() {
	this->Cancelled = true;
	this->Done = true;
};


void AISelector::DrawPage(const int16_t Pg, const int AddOffs) {
	if (Pg < 3) {
		Gui::DrawStringCentered(0 + AddOffs, 3, 0.6f, TEXT_WHITE, Lang::Get("AI_METHOD") + Lang::Get(this->Modes[Pg]), 395);
		Gui::DrawStringCentered(0 + AddOffs, 70, 0.5f, TEXT_WHITE, Lang::Get(this->ModeDesc[Pg]), 330, 140, nullptr, C2D_WordWrap);
	}
};


StackMem::AIMethod AISelector::Action() {
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
			this->DrawPage(this->Mode, this->CurPos); // Draw current page.

			if (this->SwipeDirection) this->DrawPage(this->Mode - 1, this->PrevPos);
			else this->DrawPage(this->Mode + 1, this->NextPos);

		} else { // No swipe.
			this->DrawPage(this->Mode, 0); // Draw current page only.
		}

		GFX::DrawCornerEdge(true, this->Positions[0].X, this->Positions[0].Y, this->Positions[0].H, this->Mode > 0);
		GFX::DrawCornerEdge(false, this->Positions[1].X, this->Positions[1].Y, this->Positions[1].H, this->Mode < 2);
		Gui::Draw_Rect(this->Positions[2].X, this->Positions[2].Y, this->Positions[2].W, this->Positions[2].H, BAR_BLUE);
		Gui::DrawStringCentered(0, this->Positions[2].Y + 3, 0.5f, TEXT_WHITE, Lang::Get("SELECT"));

		Pointer::Draw();
		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		};

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.
		GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, this->Mode > 0);
		GFX::DrawCornerEdge(false, this->BottomPos[2].X, this->BottomPos[2].Y, this->BottomPos[2].H, this->Mode < 2);

		Gui::Draw_Rect(95, 105, 130, 30, BAR_BLUE);
		Gui::Draw_Rect(this->BottomPos[1].X, this->BottomPos[1].Y, this->BottomPos[1].W, this->BottomPos[1].H, BG_BLUE);
		Gui::DrawStringCentered(0, this->BottomPos[1].Y + 3, 0.5f, TEXT_WHITE, Lang::Get("SELECT"));

		Gui::Draw_Rect(this->BottomPos[3].X, this->BottomPos[3].Y, this->BottomPos[3].W, this->BottomPos[3].H, BAR_BLUE); // Back.
		Gui::DrawSprite(GFX::Sprites, sprites_back_btn_idx, this->BottomPos[3].X, this->BottomPos[3].Y);

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		};
		C3D_FrameEnd(0);
		this->Handler();
	}

	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);
	if (!this->Cancelled) this->Res = (StackMem::AIMethod)this->Mode; // Only set if NOT cancelled.
	return this->Res;
};


/* Action Handler. */
void AISelector::Handler() {
	hidScanInput();
	touchPosition T;
	hidTouchRead(&T);
	const uint32_t Down = hidKeysDown();
	const uint32_t Held = hidKeysHeld();

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
			this->InitialSwipe = false, this->CurPos = 0.0f;
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.2f);
			this->CurPos = -400 + this->Cubic;

			if (this->Cubic >= 400.0f) {
				this->CurPos = 0.0f, this->Cubic = 0.0f, this->InitialSwipe = false;
			}
		}

		return;
	};

	/* Swipe Logic. */
	if (this->DoSwipe) {
		if (!_3DZwei::CFG->DoAnimation() || Down) {
			this->CurPos = 0.0f, this->PrevPos = -400.0f, this->NextPos = 400.0f;
			this->Cubic = 0.0f, this->DoSwipe = false;
			this->Mode = (this->SwipeDirection ? (this->Mode - 1) : (this->Mode + 1));
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.2f);

			if (this->SwipeDirection) { // -> (Last).
				this->CurPos = this->Cubic, this->PrevPos = -400 + this->Cubic;

			} else { // <- (Next).
				this->CurPos = 0 - this->Cubic, this->NextPos = 400 - this->Cubic;
			}

			if (this->Cubic >= 400.0f) {
				this->CurPos = 0.0f, this->PrevPos = -400.0f, this->NextPos = 400.0f;
				this->Cubic = 0.0f, this->DoSwipe = false;

				this->Mode = (this->SwipeDirection ? (this->Mode - 1) : (this->Mode + 1));
			}
		}

		return;
	};

	Pointer::ScrollHandling(Held);
	if (Down & KEY_L) this->PrevMode();
	if (Down & KEY_R) this->NextMode();

	if (Down & KEY_A) {
		for (auto &Position : this->Positions) {
			if (Pointer::Clicked(Position, true)) break;
		}
	}

	if (Down & KEY_TOUCH) {
		for (auto &Position : this->BottomPos) {
			if (Touched(Position, T, true)) break;
		}
	}

	if (Down & KEY_START || Down & KEY_B) this->OK(); // Exit with START or B as well.
};