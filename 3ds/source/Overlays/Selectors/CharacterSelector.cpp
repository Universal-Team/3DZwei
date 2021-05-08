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
void CharacterSelector::PrevChar() const {
	if (this->Res > 0) this->Res--;
	else this->Res = Utils::GetCharSheetSize() - 1;
};

/* Go to the next Character. */
void CharacterSelector::NextChar() const {
	if (this->Res < (int)Utils::GetCharSheetSize() - 1) this->Res++;
	else this->Res = 0;
};

/* Set the Select State. */
void CharacterSelector::Select() const { this->Done = true; };
void CharacterSelector::Cancel() const { this->Res = -1, this->Done = true; };

int CharacterSelector::Action() const {
	if (Utils::GetCharSheetSize() == 0) return -1;

	Pointer::OnTop = true;
	Pointer::SetPos(0, 0);
	if (this->Res > (int)Utils::GetCharSheetSize() - 1) this->Res = Utils::GetCharSheetSize() - 1;

	while(!this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("CHARACTER_SELECT_TITLE"), 395);

		/* Draw Edge boxes. */
		GFX::DrawCornerEdge(true, this->Positions[0].X, this->Positions[0].Y, this->Positions[0].H, Utils::GetCharSheetSize() > 1);
		GFX::DrawCornerEdge(false, this->Positions[1].X, this->Positions[1].Y, this->Positions[1].H, Utils::GetCharSheetSize() > 1);


		/* Draw Character Images. */
		switch(Utils::GetCharSheetSize()) {
			case 0: // 0 -> Bad.
				break;

			case 1:
				Gui::DrawSprite(GFX::Characters, 0, 140, 50);
				break;

			case 2:
				if (this->Res == 0) {
					Gui::DrawSprite(GFX::Characters, 0, 140, 50);
					Gui::DrawSprite(GFX::Characters, 1, 255, 50);

				} else {
					Gui::DrawSprite(GFX::Characters, 0, 25, 50);
					Gui::DrawSprite(GFX::Characters, 1, 140, 50);
				}
				break;

			case 3:
				if (this->Res == 0) { // 0 -> Draw first two.
					Gui::DrawSprite(GFX::Characters, 0, 140, 50);
					Gui::DrawSprite(GFX::Characters, 1, 255, 50);

				} else if (this->Res == 1) { // 1 -> Draw all three.
					Gui::DrawSprite(GFX::Characters, 0, 25, 50);
					Gui::DrawSprite(GFX::Characters, 1, 140, 50);
					Gui::DrawSprite(GFX::Characters, 2, 255, 50);

				} else { // 2 -> Draw last two.
					Gui::DrawSprite(GFX::Characters, 1, 25, 50);
					Gui::DrawSprite(GFX::Characters, 2, 140, 50);
				}
				break;
		};


		if (Utils::GetCharSheetSize() > 3) {
			if (this->Res == 0) {
				Gui::DrawSprite(GFX::Characters, 0, 140, 50);
				Gui::DrawSprite(GFX::Characters, 1, 255, 50);

			} else if (this->Res > 0 && this->Res < (int)Utils::GetCharSheetSize() - 1) { // 1 until sheet size minus 1, draw all 3.
				Gui::DrawSprite(GFX::Characters, this->Res - 1, 25, 50);
				Gui::DrawSprite(GFX::Characters, this->Res, 140, 50);
				Gui::DrawSprite(GFX::Characters, this->Res + 1, 255, 50);

			} else { // Draw last two.
				Gui::DrawSprite(GFX::Characters, this->Res - 1, 25, 50);
				Gui::DrawSprite(GFX::Characters, this->Res, 140, 50);
			}
		}

		Gui::Draw_Rect(147, 180, 106, 20, KBD_KEYUNPRESSED);
		Pointer::Draw();

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.
		GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, Utils::GetCharSheetSize() > 1);
		GFX::DrawCornerEdge(false, this->BottomPos[2].X, this->BottomPos[2].Y, this->BottomPos[2].H, Utils::GetCharSheetSize() > 1);

		Gui::Draw_Rect(95, 105, 130, 30, KBD_KEYUNPRESSED);
		Gui::Draw_Rect(this->BottomPos[1].X, this->BottomPos[1].Y, this->BottomPos[1].W, this->BottomPos[1].H, CORNER_COLOR);
		Gui::DrawStringCentered(0, this->BottomPos[1].Y + 3, 0.5f, TEXT_COLOR, Lang::Get("SELECT"));

		Gui::Draw_Rect(this->BottomPos[3].X, this->BottomPos[3].Y, this->BottomPos[3].W, this->BottomPos[3].H, KBD_KEYPRESSED); // Back.
		Gui::DrawSprite(GFX::Sprites, sprites_back_btn_idx, this->BottomPos[3].X, this->BottomPos[3].Y);

		C3D_FrameEnd(0);

		hidScanInput();
		touchPosition T;
		hidTouchRead(&T);
		const uint32_t Down = hidKeysDown();
		const uint32_t Held = hidKeysHeld();
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
	}

	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);
	return this->Res;
};