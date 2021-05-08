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
#include "CreditsOverlay.hpp"

/* Go to the previous Page. */
void CreditsOverlay::PrevPage() {
	switch(this->Page) {
		case Pages::Contributors:
			this->Page = Pages::Translators;
			break;

		case Pages::SMemCore:
			this->Page = Pages::Contributors;
			break;

		case Pages::UnivCore:
			this->Page = Pages::SMemCore;
			break;

		case Pages::Translators:
			this->Page = Pages::UnivCore;
			break;
	};
};

/* Go to the next Page. */
void CreditsOverlay::NextPage() {
	switch(this->Page) {
		case Pages::Contributors:
			this->Page = Pages::SMemCore;
			break;

		case Pages::SMemCore:
			this->Page = Pages::UnivCore;
			break;

		case Pages::UnivCore:
			this->Page = Pages::Translators;
			break;

		case Pages::Translators:
			this->Page = Pages::Contributors;
			break;
	};
};

/* Give an OK. */
void CreditsOverlay::OK() { this->Done = true; };

void CreditsOverlay::Action() {
	Pointer::OnTop = true;
	Pointer::SetPos(0, 0);

	while(!this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("CREDITS_TITLE"));

		/* Display Box + Top Field. */
		GFX::DrawMsgBox(this->Positions[0].X, this->Positions[0].Y, this->Positions[1].X - 15, this->Positions[0].H);

		/* Arrows for the sides. */
		Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, this->Positions[0].X + 5, this->Positions[0].Y + ((this->Positions[0].H / 2) - (39 / 2)));
		Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, this->Positions[1].X, this->Positions[1].Y + ((this->Positions[1].H / 2) - (39 / 2)), -1.0f, 1.0f);

		/* Info. */
		switch(this->Page) {
			case Pages::Contributors: // Contributors Page.
				Gui::DrawStringCentered(0, 70, 0.55f, TEXT_COLOR, Lang::Get("CREDITS_CONTRIBUTORS"), 100);

				Gui::DrawStringCentered(0, 100, 0.5f, TEXT_COLOR, "devkitPro");
				Gui::DrawStringCentered(0, 115, 0.4, TEXT_COLOR, "devkitARM, libctru, citro2d, citro3d.");

				Gui::DrawStringCentered(0, 130, 0.5f, TEXT_COLOR, "SuperSaiyajinStackZ");
				Gui::DrawStringCentered(0, 145, 0.4f, TEXT_COLOR, "StackMem-Core + 3DZwei.");

				Gui::DrawStringCentered(0, 160, 0.5f, TEXT_COLOR, "Universal-Team");
				Gui::DrawStringCentered(0, 175, 0.4f, TEXT_COLOR, "Universal-Core.");
				break;

			case Pages::SMemCore: // StackMem-Core Page.
				Gui::DrawSprite(GFX::Sprites, sprites_stackmemcore_idx, this->Positions[0].X + 3, this->Positions[0].Y + 0);
				break;

			case Pages::UnivCore: // Universal-Core Page.
				Gui::DrawSprite(GFX::Sprites, sprites_universal_core_idx, this->Positions[0].X + 49, this->Positions[0].Y + 8);
				break;

			case Pages::Translators: // Translators Page.
				Gui::DrawStringCentered(0, 70, 0.55f, TEXT_COLOR, Lang::Get("CREDITS_TRANSLATORS"), 100);

				Gui::DrawString(this->Positions[0].X + 30, 100, 0.45f, TEXT_COLOR, "SuperSaiyajinStackZ: Deutsch, English.");
				break;
		}


		Pointer::Draw();

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.
		GFX::DrawCornerEdge(true, this->BottomPos[0].X, this->BottomPos[0].Y, this->BottomPos[0].H, true);
		GFX::DrawCornerEdge(false, this->BottomPos[2].X, this->BottomPos[2].Y, this->BottomPos[2].H, true);

		Gui::Draw_Rect(95, 105, 130, 30, KBD_KEYUNPRESSED);
		Gui::Draw_Rect(this->BottomPos[1].X, this->BottomPos[1].Y, this->BottomPos[1].W, this->BottomPos[1].H, CORNER_COLOR);
		Gui::DrawStringCentered(0, this->BottomPos[1].Y + 3, 0.5f, TEXT_COLOR, Lang::Get("OK"));
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

		if (Down & KEY_START || Down & KEY_B) this->OK(); // Exit with START or B as well.
	}

	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);
};