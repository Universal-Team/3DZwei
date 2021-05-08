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
	if (this->Mode > 0) this->Mode--;
	else this->Mode = 2;
};

void AISelector::NextMode() {
	if (this->Mode < 2) this->Mode++;
	else this->Mode = 0;
};

void AISelector::OK() { this->Done = true; };

void AISelector::Cancel() {
	this->Cancelled = true;
	this->Done = true;
};

StackMem::AIMethod AISelector::Action() {
	Pointer::OnTop = true;
	Pointer::SetPos(0, 0);

	while(!this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("AI_METHOD") + Lang::Get(this->Modes[this->Mode]), 395);

		/* Display Box + Top Field. */
		GFX::DrawMsgBox(this->Positions[0].X, this->Positions[0].Y, this->Positions[2].X - 15, this->Positions[0].H);
		Gui::Draw_Rect(this->Positions[1].X, this->Positions[1].Y, this->Positions[1].W, this->Positions[1].H, CORNER_COLOR);

		/* Arrows for the sides. */
		Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, this->Positions[0].X + 5, this->Positions[0].Y + ((this->Positions[0].H / 2) - (39 / 2)));
		Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, this->Positions[2].X, this->Positions[2].Y + ((this->Positions[2].H / 2) - (39 / 2)), -1.0f, 1.0f);

		/* Info about the Difficulty / Method. */
		Gui::DrawStringCentered(0, 70, 0.4f, TEXT_COLOR, Lang::Get(this->ModeDesc[this->Mode]), 260, 110, nullptr, C2D_WordWrap);
		Gui::DrawStringCentered(0, this->Positions[1].Y + 2, 0.5f, TEXT_COLOR, Lang::Get("SELECT"));
		Pointer::Draw();

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.
		GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, true);
		GFX::DrawCornerEdge(false, this->BottomPos[2].X, this->BottomPos[2].Y, this->BottomPos[2].H, true);

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

		if (Down & KEY_START) this->OK();
	}

	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);
	if (!this->Cancelled) this->Res = (StackMem::AIMethod)this->Mode; // Only set if NOT cancelled.
	return this->Res;
};