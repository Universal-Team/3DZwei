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
#include "RulesOverlay.hpp"

/* STATE 1: Handle Going to first card. */
void RulesOverlay::State1() {
	if (this->X > this->Cards[0].X + 20) {
		this->X--;

		if (this->X != this->Cards[0].X + 20) return;
	}

	if (this->Y > this->Cards[0].Y + 20) {
		this->Y--;

		if (this->Y != this->Cards[0].Y + 20) return;
	}

	this->State = States::ClickOnFirst;
};

/* STATE 2: Handle click on the card + Swipe the StackMem Logo. */
void RulesOverlay::State2() {
	this->CardClicks[0] = true;
	this->State = States::GotoSecond;
};

/* STATE 3: Handle Going to second card. */
void RulesOverlay::State3() {
	if (this->X < this->Cards[19].X + 20) {
		this->X += 2;

		if (this->X != this->Cards[19].X + 20) return;
	}

	if (this->Y < this->Cards[19].Y + 20) {
		this->Y += 2;

		if (this->Y != this->Cards[19].Y + 20) return;
	}

	this->State = States::ClickOnSecond;
};

/* STATE 4: Show the second card and go into the Wait State. */
void RulesOverlay::State4() {
	this->CardClicks[1] = true;
	this->State = States::WaitDelay;
};

/* STATE 5: Wait for the delay. */
void RulesOverlay::State5() {
	if (this->Delay > 0) {
		this->Delay--;

		if (this->Delay == 0) {
			this->Delay = 100;

			/* Hide Cards, reset positions + go to first State. */
			this->CardClicks[0] = false, this->CardClicks[1] = false;
			this->X = this->Cards[7].X + 20, this->Y = this->Cards[7].Y + 20;
			this->State = States::GotoFirst;
		}
	}
};

void RulesOverlay::StateHandler() {
	switch(this->State) {
		case States::GotoFirst:
			this->State1();
			break;

		case States::ClickOnFirst:
			this->State2();
			break;

		case States::GotoSecond:
			this->State3();
			break;

		case States::ClickOnSecond:
			this->State4();
			break;

		case States::WaitDelay:
			this->State5();
			break;
	}
};

const std::string RULES = "Select 2 cards each turn, if those show the same image, you've got a pair, else it's your opponents turn.\n\nThe Player with the most pairs wins, but NOTE, that ties are also possible if you are using an even pair number.";

void RulesOverlay::Action() {
	while(!this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("RULES_TITLE"), 395);
		Gui::DrawStringCentered(0, 40, 0.55f, TEXT_COLOR, Lang::Get("RULES_DESC"), 380, 160, nullptr, C2D_WordWrap);

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.

		/* First real card. */
		if (this->CardClicks[0]) Gui::DrawSprite(GFX::Sprites, sprites_voltcard_idx, this->Cards[0].X, this->Cards[0].Y);
		else Gui::DrawSprite(GFX::Sprites, sprites_icon_idx, this->Cards[0].X, this->Cards[0].Y);

		/* Hidden cards. */
		for (uint8_t Idx = 1; Idx < 19; Idx++) {
			Gui::DrawSprite(GFX::Sprites, sprites_icon_idx, this->Cards[Idx].X, this->Cards[Idx].Y);
		}

		/* Second real card. */
		if (this->CardClicks[1]) Gui::DrawSprite(GFX::Sprites, sprites_voltcard_idx, this->Cards[19].X, this->Cards[19].Y);
		else Gui::DrawSprite(GFX::Sprites, sprites_icon_idx, this->Cards[19].X, this->Cards[19].Y);

		Gui::DrawSprite(GFX::Sprites, sprites_grid_idx, 19.5, 7.5);
		Gui::DrawSprite(GFX::Sprites, sprites_pointer_idx, this->X, this->Y);

		C3D_FrameEnd(0);

		hidScanInput();
		const uint32_t Down = hidKeysDown();
		if (Down) this->Done = true; // Any key -> Skip.

		this->StateHandler();
	};
};