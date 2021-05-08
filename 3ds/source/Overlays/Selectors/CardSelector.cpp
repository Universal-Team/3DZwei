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
	for (size_t Idx = 0; Idx < Utils::GetCardSheetSize(); Idx++) this->Cards.push_back(std::make_pair(false, Idx));

	/* Enable all current enabled cards. */
	if (!Utils::Cards.empty()) {
		for (size_t Idx = 0; Idx < Utils::Cards.size(); Idx++) this->Cards[Utils::Cards[Idx]].first = true;
	}
};

/* Toggle the specified indexes card. */
void CardSelector::ToggleCard(const uint8_t Idx) {
	if (((this->Page * 9) + Idx) < this->Cards.size()) {
		this->Cards[(this->Page * 9) + Idx].first = !this->Cards[(this->Page * 9) + Idx].first;
		if (!this->Changed) this->Changed = true;
	}
};

/* Go to the previous page. */
void CardSelector::PrevPage() {
	if (this->Page > 0) this->Page--;
};

/* Go to the next page. */
void CardSelector::NextPage() {
	if (this->CanGoNext()) this->Page++;
};

/* Give the OK state. */
void CardSelector::OK() { this->Done = true; };

/* Return, if a next page is available. */
bool CardSelector::CanGoNext() const {
	return (((this->Page * 9) + 9) < this->Cards.size());
};

void CardSelector::Action() {
	Pointer::OnTop = true;
	Pointer::SetPos(0, 0);

	while(!this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("CARD_SELECTOR_TITLE"), 395);

		/* Draw the Cards + Checkboxes. */
		for (size_t Idx = (this->Page * 9), Idx2 = 0; Idx < (this->Page * 9) + 9 && Idx < this->Cards.size(); Idx++, Idx2++) {
			Gui::DrawSprite(GFX::Cards, Idx, this->Positions[Idx2 + 2].X, this->Positions[Idx2 + 2].Y);
			GFX::DrawCheckbox(this->Positions[Idx2 + 11].X, this->Positions[Idx2 + 11].Y, this->Cards[Idx].first);
		}

		GFX::DrawCornerEdge(true, this->Positions[0].X, this->Positions[0].Y, this->Positions[0].H, this->Page > 0);
		GFX::DrawCornerEdge(false, this->Positions[1].X, this->Positions[1].Y, this->Positions[1].H, this->CanGoNext());
		Pointer::Draw();

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.

		GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, this->Page > 0);
		GFX::DrawCornerEdge(false, this->BottomPos[1].X, this->BottomPos[1].Y, this->BottomPos[1].H, this->CanGoNext());

		/* Draw the Checkboxes. */
		for (size_t Idx = (this->Page * 9), Idx2 = 0; Idx < (this->Page * 9) + 9 && Idx < this->Cards.size(); Idx++, Idx2++) {
			GFX::DrawCheckbox(this->BottomPos[Idx2 + 2].X, this->BottomPos[Idx2 + 2].Y, this->Cards[Idx].first);
		}

		Gui::Draw_Rect(this->BottomPos[11].X, this->BottomPos[11].Y, this->BottomPos[11].W, this->BottomPos[11].H, KBD_KEYPRESSED);
		Gui::DrawStringCentered(0, this->BottomPos[11].Y + 3, 0.6f, TEXT_COLOR, Lang::Get("OK"));

		C3D_FrameEnd(0);

		hidScanInput();
		touchPosition T;
		hidTouchRead(&T);
		const uint32_t Down = hidKeysDown();
		const uint32_t Held = hidKeysHeld();
		Pointer::ScrollHandling(Held);

		if (Down & KEY_L) this->PrevPage();
		if (Down & KEY_R) this->NextPage();

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

		if (Down & KEY_START) this->OK();
	};

	/* Set the cards now, that we finished. */
	if (this->Changed) {
		Utils::Cards.clear();

		for (size_t Idx = 0; Idx < this->Cards.size(); Idx++) {
			/* If card enabled -> Push. */
			if (this->Cards[Idx].first) Utils::Cards.push_back(this->Cards[Idx].second);
		}

		_3DZwei::CFG->ActivatedCards(Utils::Cards); // Set the activated cards to config.
	}

	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);
};