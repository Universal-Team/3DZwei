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

#ifndef _3DZWEI_CHARACTERSET_SELECTOR_HPP
#define _3DZWEI_CHARACTERSET_SELECTOR_HPP

#include "Pointer.hpp"
#include <citro2d.h>
#include <string>
#include <vector>

class CharacterSetSelector {
public:
	CharacterSetSelector();
	std::string Action();
private:
	bool Done = false, FullDone = false, Res = false, SetGood = false, FadeIn = true,
		IsSelecting = true, ModeSwitch = false, InitialSwipe = true;
	int16_t FAlpha = 255;

	/* Selector based. */
	std::vector<std::string> CharSets; // List all CharacterSets there.
	int SelectedSet = 0, SetListPos = 0, CurSetPos = 0;

	/* Preview based. */
	C2D_SpriteSheet PreviewSheet = nullptr;
	bool DoSwipe = false, SwipeDir = false, CharSwipeIn = false, CharSwipeOut = false;;
	int CharPage = 0, PrevPos = -400, CurPos = -400, NextPos = 400;

	/* Animation related. */
	float Cubic = 0.0f;

	/* Set. */
	void DrawSetList(const int AddOffs);
	void PrevSetPage();
	void LastSet();
	void NextSet();
	void NextSetPage();
	void HandleSet(const uint32_t Down, const uint32_t Held, const touchPosition &T);

	/* Preview. */
	void PrevChar();
	void NextChar();
	bool CanGoNext() const;
	void DrawCharacter(const int Page, const int AddOffs);
	void DrawCharBottom(const int AddOffs);
	void HandleChar(const uint32_t Down, const uint32_t Held, const touchPosition &T);

	/* Both. */
	void PreviewSelection(const size_t Idx, const bool SetSelection = false, const bool First = false);
	void OldCharsetOut();
	void Cancel();
	void Confirm();
	void Handler();
	void Draw();

	const std::vector<FuncCallback> Positions = {
		{ 0, 25, 20, 215, [this]() { this->PrevChar(); } },
		{ 380, 25, 20, 215, [this]() { this->NextChar(); } }
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
		{ 0, 15, 25, 215, [this]() { this->PrevChar(); } },
		{ 295, 15, 25, 215, [this]() { this->NextChar(); } },

		{ 45, 94, 110, 48, [this]() { this->Cancel(); } },
		{ 165, 94, 110, 48, [this]() { this->Confirm(); } }
	};
};

#endif