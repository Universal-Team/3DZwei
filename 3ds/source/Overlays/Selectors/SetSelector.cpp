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

#include "Browser.hpp"
#include "Common.hpp"
#include "SetSelector.hpp"
#include "Utils.hpp"

#define SETS_PER_SCREEN 5

/* Fetch the Sets. */
SetSelector::SetSelector(const bool LoadCharacters) {
	const std::vector<std::string> Tmp = { "t3x" };
	this->LoadCharacters = LoadCharacters;

	if (LoadCharacters) { // Character Mode.
		std::unique_ptr<Browser> B = std::make_unique<Browser>("sdmc:/3ds/3DZwei/sets/characters/", Tmp);
		this->Sets = B->GetList();

	} else { // Card Mode.
		std::unique_ptr<Browser> B = std::make_unique<Browser>("sdmc:/3ds/3DZwei/sets/cards/", Tmp);
		this->Sets = B->GetList();
	}

	this->Sets.push_back("3DZwei-RomFS"); // Push back the 3DZwei-RomFS default set. :p
};

/* Go to previous Page. */
void SetSelector::PrevPage() {
	if (this->Sets.size() > 5) {
		if (this->Selected >= SETS_PER_SCREEN) this->Selected -= SETS_PER_SCREEN;
		else this->Selected = 0;
	}
};

/* Go to next Page. */
void SetSelector::NextPage() {
	if (this->Sets.size() > 5) {
		if ((this->Selected + SETS_PER_SCREEN) < (this->Sets.size())) this->Selected += SETS_PER_SCREEN;
		else this->Selected = this->Sets.size() - 1;
	}
};

/* The main action to select. */
void SetSelector::SelectSet(const size_t Idx) {
	if ((this->ScreenPos + Idx) < this->Sets.size()) {
		if (Utils::CheckSetContent(this->Sets[this->ScreenPos + Idx], this->LoadCharacters)) { // Ensure set is good.
			this->Res = this->Sets[this->ScreenPos + Idx];
			this->Done = true;
		}
	}
};

void SetSelector::Cancel() const { this->Res = "", this->Done = true; };

std::string SetSelector::Action() const {
	Pointer::OnTop = true;
	Pointer::SetPos(0, 0);

	while(!this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("SET_SELECTION_TITLE"), 395);
		GFX::DrawCornerEdge(true, this->Positions[0].X, this->Positions[0].Y, this->Positions[0].H, this->Selected > 0);
		GFX::DrawCornerEdge(false, this->Positions[6].X, this->Positions[6].Y, this->Positions[6].H, this->Sets.size() > 5 && this->Selected < this->Sets.size() - 1);
		Gui::Draw_Rect(0, 215, 400, 25, BAR_COLOR);

		/* Display Cardsets. */
		if (this->Sets.size() > 0) {
			for(size_t Idx = 0; Idx < SETS_PER_SCREEN && Idx < this->Sets.size(); Idx++) {
				Gui::Draw_Rect(this->Positions[1 + Idx].X, this->Positions[1 + Idx].Y, this->Positions[1 + Idx].W, this->Positions[1 + Idx].H, (Idx % 2 == 0 ? KBD_KEYPRESSED : KD_KEYUNPRESSED));
				Gui::DrawStringCentered(0, this->Positions[1 + Idx].Y + 8, 0.5f, TEXT_COLOR, this->Sets[this->ScreenPos + Idx], 290);
			}
		}

		Pointer::Draw();

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.

		/* Side Edge boxes. */
		GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, this->Selected > 0);
		GFX::DrawCornerEdge(false, this->BottomPos[6].X, 25, this->BottomPos[6].H, this->Sets.size() > 5 && this->Selected < this->Sets.size() - 1);

		/* Display Cardsets on the bottom. */
		if (this->Sets.size() > 0) {
			for(size_t Idx = 0; Idx < SETS_PER_SCREEN && Idx < this->Sets.size(); Idx++) {
				Gui::Draw_Rect(this->BottomPos[1 + Idx].X, this->BottomPos[1 + Idx].Y, this->BottomPos[1 + Idx].W, this->BottomPos[1 + Idx].H, (Idx % 2 == 0 ? KBD_KEYPRESSED : KD_KEYUNPRESSED));
			}
		}

		Gui::Draw_Rect(this->BottomPos[7].X, this->BottomPos[7].Y, this->BottomPos[7].W, this->BottomPos[7].H, KBD_KEYPRESSED); // Back.
		Gui::DrawSprite(GFX::Sprites, sprites_back_btn_idx, this->BottomPos[7].X, this->BottomPos[7].Y);

		C3D_FrameEnd(0);

		/* ScreenPos handle. */
		if (this->Selected < this->ScreenPos) this->ScreenPos = this->Selected;
		else if (this->Selected > this->ScreenPos + SETS_PER_SCREEN - 1) this->ScreenPos = this->Selected - SETS_PER_SCREEN + 1;


		hidScanInput();
		touchPosition T;
		const uint32_t Down = hidKeysDown();
		const uint32_t Held = hidKeysHeld();
		hidTouchRead(&T);
		Pointer::ScrollHandling(Held);

		if (Down & KEY_B) this->Cancel();

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