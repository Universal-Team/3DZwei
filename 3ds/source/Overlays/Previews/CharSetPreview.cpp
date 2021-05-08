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

#include "CharSetPreview.hpp"
#include "Common.hpp"
#include "Utils.hpp"

CharSetPreview::CharSetPreview(const std::string &Set) { this->Set = Set; };

/* Go to the previous character. */
void CharSetPreview::PrevChar() {
	if (this->SetGood) {
		if (this->Idx > 0) this->Idx--;
		else this->Idx = C2D_SpriteSheetCount(this->PreviewSheet) - 1;
	}
};

/* Go to the next character. */
void CharSetPreview::NextChar() {
	if (this->SetGood) {
		if (this->Idx < C2D_SpriteSheetCount(this->PreviewSheet) - 1) this->Idx++;
		else this->Idx = 0;
	}
};

/* Return, if a next page is available. */
bool CharSetPreview::CanGoNext() const {
	if (this->SetGood) {
		if (this->PreviewSheet && C2D_SpriteSheetCount(this->PreviewSheet) > 3) return true;
	}

	return false;
};

void CharSetPreview::Cancel() { this->Res = false, this->Done = true; };
void CharSetPreview::Confirm() { this->Res = true, this->Done = true; };

bool CharSetPreview::Action() {
	if (this->Set == "3DZwei-RomFS") return true; // Already good due to RomFS always existing.

	/* Check, if a cardset is good. */
	if (this->Set != "" && this->Set != "3DZwei-RomFS" && Utils::CheckSetContent(Set, true)) {
		this->PreviewSheet = C2D_SpriteSheetLoad((std::string("sdmc:/3ds/3DZwei/sets/characters/") + this->Set).c_str());
		this->SetGood = this->PreviewSheet;

		Pointer::OnTop = true;
		Pointer::SetPos(0, 0);
	}

	while(!this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		if (this->SetGood) {
			Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("SET_PREVIEW_TITLE"), 395);

			/* Draw Character Images. */
			switch(C2D_SpriteSheetCount(this->PreviewSheet)) {
				case 0:
					break;

				case 1:
					Gui::DrawSprite(this->PreviewSheet, 0, 140, 50);
					break;

				case 2:
					Gui::DrawSprite(this->PreviewSheet, 0, 140, 50);
					Gui::DrawSprite(this->PreviewSheet, 1, 255, 50);
					break;

				case 3:
					Gui::DrawSprite(this->PreviewSheet, 0, 25, 50);
					Gui::DrawSprite(this->PreviewSheet, 1, 140, 50);
					Gui::DrawSprite(this->PreviewSheet, 2, 255, 50);
					break;
			};

			if (C2D_SpriteSheetCount(this->PreviewSheet) > 3) {
				if (this->Idx == 0) {
					Gui::DrawSprite(this->PreviewSheet, 0, 140, 50);
					Gui::DrawSprite(this->PreviewSheet, 1, 255, 50);

				} else if (this->Idx > 0 && this->Idx < C2D_SpriteSheetCount(this->PreviewSheet) - 1) { // 1 until sheet size minus 1, draw all 3.
					Gui::DrawSprite(this->PreviewSheet, this->Idx - 1, 25, 50);
					Gui::DrawSprite(this->PreviewSheet, this->Idx, 140, 50);
					Gui::DrawSprite(this->PreviewSheet, this->Idx + 1, 255, 50);

				} else { // Draw last two.
					Gui::DrawSprite(this->PreviewSheet, this->Idx - 1, 25, 50);
					Gui::DrawSprite(this->PreviewSheet, this->Idx, 140, 50);
				}
			}

			GFX::DrawCornerEdge(true, this->Positions[0].X, this->Positions[0].Y, this->Positions[0].H, this->CanGoNext());
			GFX::DrawCornerEdge(false, this->Positions[1].X, this->Positions[1].Y, this->Positions[1].H, this->CanGoNext());
			if (this->PreviewSheet) Gui::DrawStringCentered(0, 200, 0.6f, TEXT_COLOR, Lang::Get("AMOUNT_OF_CHARACTERS") + std::to_string(C2D_SpriteSheetCount(this->PreviewSheet)) , 395);
			Pointer::Draw();

		} else {
			Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("SET_PREVIEW_ERROR"), 395);

			Gui::DrawStringCentered(0, 100, 0.4f, TEXT_COLOR, Lang::Get("SET_PREVIEW_ERROR_2"), 395);
		}

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.

		if (this->SetGood) {
			GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, this->CanGoNext());
			GFX::DrawCornerEdge(false, this->BottomPos[1].X, this->BottomPos[1].Y, this->BottomPos[1].H, this->CanGoNext());

			/* Draw Cancel and Confirm buttons. */
			Gui::Draw_Rect(this->BottomPos[2].X, this->BottomPos[2].Y, this->BottomPos[2].W, this->BottomPos[2].H, KBD_KEYPRESSED);
			Gui::DrawStringCentered(-60, this->BottomPos[2].Y + 15, 0.6f, TEXT_COLOR, Lang::Get("CANCEL"));
			Gui::Draw_Rect(this->BottomPos[3].X, this->BottomPos[3].Y, this->BottomPos[3].W, this->BottomPos[3].H, KBD_KEYPRESSED);
			Gui::DrawStringCentered(60, this->BottomPos[3].Y + 15, 0.6f, TEXT_COLOR, Lang::Get("CONFIRM"));
		}

		C3D_FrameEnd(0);

		hidScanInput();
		touchPosition T;
		hidTouchRead(&T);
		const uint32_t Down = hidKeysDown();
		const uint32_t Held = hidKeysHeld();

		if (!this->SetGood) {
			if (Down) return false; // Any key to continue.

		} else {
			Pointer::ScrollHandling(Held);

			if (Down & KEY_L) this->PrevChar();
			if (Down & KEY_R) this->NextChar();
			if (Down & KEY_X || Down & KEY_START) this->Confirm(); // X | START -> Using.
			if (Down & KEY_Y || Down & KEY_B) this->Cancel(); // Y | B -> Cancel.

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
	};

	if (this->PreviewSheet) C2D_SpriteSheetFree(this->PreviewSheet); // We don't need the Preview anymore, so free the Spritesheet.
	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);
	return this->Res;
};