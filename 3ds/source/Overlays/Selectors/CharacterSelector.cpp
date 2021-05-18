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
#include "CharacterSelector.hpp"
#include "Utils.hpp"


/* Go to the previous Character. */
void CharacterSelector::PrevChar() {
	if (this->Res > 0) {
		this->SwipeDir = true;
		this->DoSwipe = true;
	};
};


/* Go to the next Character. */
void CharacterSelector::NextChar() {
	if (this->Res < (int)Utils::GetCharSheetSize() - 1) {
		this->SwipeDir = false;
		this->DoSwipe = true;
	}
};


/* Set the Select State. */
void CharacterSelector::Select() { this->Done = true; };
void CharacterSelector::Cancel() { this->Res = -1, this->Done = true; };


int CharacterSelector::Action() {
	if (Utils::GetCharSheetSize() == 0) return -1;

	Pointer::OnTop = true;
	Pointer::SetPos(0, 0);
	if (this->Res > (int)Utils::GetCharSheetSize() - 1) this->Res = Utils::GetCharSheetSize() - 1;

	while(aptMainLoop() && !this->FullDone) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("CHARACTER_SELECT_TITLE"), 395);

		/* Display Characters. */
		if (this->Res < (int)Utils::GetCharSheetSize()) {
			/* Current Character. */
			if (this->DoSwipe || this->InitialSwipe) {
				if (this->SwipeDir) { // Swipe to -> (Last).
					Gui::DrawSprite(GFX::Characters, this->Res, 140 + this->CurPos, 72);
					Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 138 + this->CurPos, 70);

				} else { // Swipe to <- (Next).
					Gui::DrawSprite(GFX::Characters, this->Res, 140 + this->CurPos, 72);
					Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 138 + this->CurPos, 70);
				}

			} else { // No Swipe.
				Gui::DrawSprite(GFX::Characters, this->Res, 140, 72);
				Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 138, 70);
			}


			if (this->DoSwipe) {
				/* Prev Character. */
				if (this->Res >= 1) {
					Gui::DrawSprite(GFX::Characters, this->Res - 1, 140 + this->PrevPos, 72);
					Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 138 + this->PrevPos, 70);
				};

				/* Next Character. */
				if (this->Res + 1 < (int)Utils::GetCharSheetSize()) {
					Gui::DrawSprite(GFX::Characters, this->Res + 1, 140 + this->NextPos, 72);
					Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 138 + this->NextPos, 70);
				};
			}
		};

		Gui::Draw_Rect(this->Positions[2].X, this->Positions[2].Y, this->Positions[2].W, this->Positions[2].H, KBD_KEYUNPRESSED);
		Gui::DrawStringCentered(0, this->Positions[2].Y + 2, 0.5f, TEXT_COLOR, Lang::Get("SELECT"), 395);

		/* Draw Edge boxes. */
		GFX::DrawCornerEdge(true, this->Positions[0].X, this->Positions[0].Y, this->Positions[0].H, this->Res >= 1);
		GFX::DrawCornerEdge(false, this->Positions[1].X, this->Positions[1].Y, this->Positions[1].H, this->Res + 1 < (int)Utils::GetCharSheetSize());
		Pointer::Draw();

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		}

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.
		GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, this->Res >= 1);
		GFX::DrawCornerEdge(false, this->BottomPos[2].X, this->BottomPos[2].Y, this->BottomPos[2].H, this->Res + 1 < (int)Utils::GetCharSheetSize());

		Gui::Draw_Rect(95, 105, 130, 30, KBD_KEYUNPRESSED);
		Gui::Draw_Rect(this->BottomPos[1].X, this->BottomPos[1].Y, this->BottomPos[1].W, this->BottomPos[1].H, CORNER_COLOR);
		Gui::DrawStringCentered(0, this->BottomPos[1].Y + 3, 0.5f, TEXT_COLOR, Lang::Get("SELECT"));

		Gui::Draw_Rect(this->BottomPos[3].X, this->BottomPos[3].Y, this->BottomPos[3].W, this->BottomPos[3].H, KBD_KEYPRESSED); // Back.
		Gui::DrawSprite(GFX::Sprites, sprites_back_btn_idx, this->BottomPos[3].X, this->BottomPos[3].Y);

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		}

		C3D_FrameEnd(0);
		this->CharLogic();
	}

	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);
	return this->Res;
};


/* The Main Logic of this Overlay. */
void CharacterSelector::CharLogic() {
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
	}

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
			this->Res = (this->SwipeDir ? (this->Res - 1) : (this->Res + 1));
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.2f);

			if (this->SwipeDir) this->CurPos = this->Cubic, this->PrevPos = -400 + this->Cubic; // -> (Last).
			else this->CurPos = 0 - this->Cubic, this->NextPos = 400 - this->Cubic; // <- (Next).

			if (this->Cubic >= 400.0f) {
				this->CurPos = 0.0f, this->PrevPos = -400.0f, this->NextPos = 400.0f;
				this->Cubic = 0.0f, this->DoSwipe = false;
				this->Res = (this->SwipeDir ? (this->Res - 1) : (this->Res + 1));
			}
		}

		return;
	};

	Pointer::ScrollHandling(Held);
	if (Down & KEY_L) this->PrevChar();
	if (Down & KEY_R) this->NextChar();
	if (Down & KEY_B) this->Cancel();
	if (Down & KEY_START) this->Select();

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
};