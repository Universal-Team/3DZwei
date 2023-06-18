/*
*   This file is part of 3DZwei
*   Copyright (C) 2020-2023 Universal-Team
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

#ifndef _3DZWEI_CARDSET_SELECTOR_HPP
#define _3DZWEI_CARDSET_SELECTOR_HPP

#include "Pointer.hpp"
#include <citro2d.h>
#include <string>
#include <vector>

class CardSetSelector {
public:
	CardSetSelector();
	std::string Action();
private:
	bool Done = false, FullDone = false, Res = false, SetGood = false, FadeIn = true,
		IsSelecting = true, ModeSwitch = false, InitialSwipe = true;
	int16_t FAlpha = 255;

	/* Selector based. */
	std::vector<std::string> CardSets; // List all cardsets there.
	int SelectedSet = 0, SetListPos = 0, CurSetPos = 0;

	/* Preview based. */
	C2D_SpriteSheet PreviewSheet = nullptr;
	bool FlipCard = false, FirstFlipDone = false, CardSwipeDir = false, CardSwipe = false,
		CardSwipeIn = false, CardSwipeOut = false;
	int CardPage = 0, PrevCardPos = -400, CurCardPos = -400, NextCardPos = 400, ToFlip = -1;

	/* Animation related. */
	float Cubic = 0.0f;
	std::vector<float> CardScale;
	std::vector<bool> CardFlipped;

	/* Set. */
	void DrawSetList(const int AddOffs);
	void PrevSetPage();
	void LastSet();
	void NextSet();
	void NextSetPage();
	void HandleSet(const uint32_t Down, const uint32_t Held, const uint32_t Repeat, const touchPosition &T);

	/* Preview. */
	void PrevCardPage();
	void NextCardPage();
	bool CardCanGoNext() const;
	void ToggleCard(const uint8_t Idx);
	void DrawCardPage(const size_t Pg, const int AddOffs);
	void DrawCardBottom(const int AddOffs);
	void HandleCard(const uint32_t Down, const uint32_t Held, const uint32_t Repeat, const touchPosition &T);

	/* Both. */
	void PreviewSelection(const size_t Idx, const bool SetSelection = false, const bool First = false);
	void OldCardsetOut();
	void Cancel();
	void Confirm();
	void Handler();
	void Draw();

	const std::vector<FuncCallback> Positions = {
		{ 0, 25, 25, 215, [this]() { this->PrevCardPage(); } },
		{ 375, 25, 25, 215, [this]() { this->NextCardPage(); } },

		{ 60, 35, 55, 55, [this]() { this->ToggleCard(0); } },
		{ 160, 35, 55, 55, [this]() { this->ToggleCard(1); } },
		{ 260, 35, 55, 55, [this]() { this->ToggleCard(2); } },

		{ 60, 105, 55, 55, [this]() { this->ToggleCard(3); } },
		{ 160, 105, 55, 55, [this]() { this->ToggleCard(4); } },
		{ 260, 105, 55, 55, [this]() { this->ToggleCard(5); } },

		{ 60, 175, 55, 55, [this]() { this->ToggleCard(6); } },
		{ 160, 175, 55, 55, [this]() { this->ToggleCard(7); } },
		{ 260, 175, 55, 55, [this]() { this->ToggleCard(8); } }
	};

	const std::vector<FuncCallback> SetPos = {
		{ 0, 15, 25, 215, [this]() { this->PrevSetPage(); } },

		{ 50, 30, 220, 32, [this]() { this->PreviewSelection(this->SetListPos + 0, true); } },
		{ 50, 67, 220, 32, [this]() { this->PreviewSelection(this->SetListPos + 1, true); } },
		{ 50, 104, 220, 32, [this]() { this->PreviewSelection(this->SetListPos + 2, true); } },
		{ 50, 141, 220, 32, [this]() { this->PreviewSelection(this->SetListPos + 3, true); } },
		{ 50, 178, 220, 32, [this]() { this->PreviewSelection(this->SetListPos + 4, true); } },

		{ 295, 15, 25, 215, [this]() { this->NextSetPage(); } }
	};


	const std::vector<FuncCallback> BottomPos = {
		{ 0, 15, 25, 215, [this]() { this->PrevCardPage(); } },
		{ 295, 15, 25, 215, [this]() { this->NextCardPage(); } },

		{ 88, 50, 24, 24, [this]() { this->ToggleCard(0); } },
		{ 148, 50, 24, 24, [this]() { this->ToggleCard(1); } },
		{ 208, 50, 24, 24, [this]() { this->ToggleCard(2); } },

		{ 88, 110, 24, 24, [this]() { this->ToggleCard(3); } },
		{ 148, 110, 24, 24, [this]() { this->ToggleCard(4); } },
		{ 208, 110, 24, 24, [this]() { this->ToggleCard(5); } },

		{ 88, 170, 24, 24, [this]() { this->ToggleCard(6); } },
		{ 148, 170, 24, 24, [this]() { this->ToggleCard(7); } },
		{ 208, 170, 24, 24, [this]() { this->ToggleCard(8); } },

		{ 45, 210, 110, 25, [this]() { this->Cancel(); } },
		{ 165, 210, 110, 25, [this]() { this->Confirm(); } }
	};
};

#endif