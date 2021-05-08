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
#include "Common.hpp"
#include "Utils.hpp"

CardSetPreview::CardSetPreview(const std::string &Set) { this->Set = Set; };

/* Toggle the specified indexes card. */
void CardSetPreview::ToggleCard(const uint8_t Idx) {
	if (((this->Page * 9) + Idx) < this->CardsShown.size()) {
		this->CardsShown[(this->Page * 9) + Idx] = !this->CardsShown[(this->Page * 9) + Idx];
	}
};

/* Go to the previous page. */
void CardSetPreview::PrevPage() {
	if (this->Page > 0) this->Page--;
};

/* Go to the next page. */
void CardSetPreview::NextPage() {
	if (this->CanGoNext()) this->Page++;
};

/* Return, if a next page is available. */
bool CardSetPreview::CanGoNext() const {
	return (((this->Page * 9) + 9) < this->CardsShown.size());
};

void CardSetPreview::Cancel() { this->Res = false, this->Done = true; };
void CardSetPreview::Confirm() { this->Res = true, this->Done = true; };


bool CardSetPreview::Action() {
	if (this->Set == "3DZwei-RomFS") return true; // Already good due to RomFS always existing.

	/* Check, if a cardset is good. */
	if (this->Set != "" && this->Set != "3DZwei-RomFS" && Utils::CheckSetContent(Set, false)) {
		this->PreviewSheet = C2D_SpriteSheetLoad((std::string("sdmc:/3ds/3DZwei/sets/cards/") + this->Set).c_str());
		this->SetGood = this->PreviewSheet;

		Pointer::OnTop = true;
		Pointer::SetPos(0, 0);

		if (this->PreviewSheet) {
			if (C2D_SpriteSheetCount(this->PreviewSheet) > 0) {
				for (size_t Idx = 0; Idx < C2D_SpriteSheetCount(this->PreviewSheet) - 1; Idx++) this->CardsShown.push_back(true);
			}
		}
	}

	while(!this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		if (this->SetGood) {
			Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("SET_PREVIEW_TITLE"), 395);

			/* Draw the Cards + Checkboxes. */
			for (size_t Idx = (this->Page * 9), Idx2 = 0; Idx < (this->Page * 9) + 9 && Idx < this->CardsShown.size(); Idx++, Idx2++) {
				if (this->CardsShown[Idx]) {
					Gui::DrawSprite(this->PreviewSheet, Idx, this->Positions[Idx2 + 2].X, this->Positions[Idx2 + 2].Y);

				} else {
					Gui::DrawSprite(this->PreviewSheet, C2D_SpriteSheetCount(this->PreviewSheet) - 1, this->Positions[Idx2 + 2].X, this->Positions[Idx2 + 2].Y);
				}
			}

			GFX::DrawCornerEdge(true, this->Positions[0].X, this->Positions[0].Y, this->Positions[0].H, this->Page > 0);
			GFX::DrawCornerEdge(false, this->Positions[1].X, this->Positions[1].Y, this->Positions[1].H, this->CanGoNext());
			Pointer::Draw();

		} else {
			Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("SET_PREVIEW_ERROR"), 395);

			Gui::DrawStringCentered(0, 100, 0.4f, TEXT_COLOR, Lang::Get("SET_PREVIEW_ERROR_2"), 395);
		}

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.

		if (this->SetGood) {
			GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, this->Page > 0);
			GFX::DrawCornerEdge(false, this->BottomPos[1].X, this->BottomPos[1].Y, this->BottomPos[1].H, this->CanGoNext());
			Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("AMOUNT_OF_CARDS") + std::to_string(this->CardsShown.size()) , 310);

			/* Draw Cancel and Confirm buttons. */
			Gui::Draw_Rect(this->BottomPos[11].X, this->BottomPos[11].Y, this->BottomPos[11].W, this->BottomPos[11].H, KBD_KEYPRESSED);
			Gui::DrawStringCentered(-60, this->BottomPos[11].Y + 3, 0.6f, TEXT_COLOR, Lang::Get("CANCEL"));
			Gui::Draw_Rect(this->BottomPos[12].X, this->BottomPos[12].Y, this->BottomPos[12].W, this->BottomPos[12].H, KBD_KEYPRESSED);
			Gui::DrawStringCentered(60, this->BottomPos[12].Y + 3, 0.6f, TEXT_COLOR, Lang::Get("CONFIRM"));


			/* Draw the Checkboxes for toggling the back cover. */
			for (size_t Idx = (this->Page * 9), Idx2 = 0; Idx < (this->Page * 9) + 9 && Idx < this->CardsShown.size(); Idx++, Idx2++) {
				GFX::DrawCheckbox(this->BottomPos[Idx2 + 2].X, this->BottomPos[Idx2 + 2].Y, this->CardsShown[Idx]);
			}
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

			if (Down & KEY_L) this->PrevPage();
			if (Down & KEY_R) this->NextPage();
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