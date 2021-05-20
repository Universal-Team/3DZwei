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

#ifndef _3DZWEI_CHARACTER_SELECTOR_HPP
#define _3DZWEI_CHARACTER_SELECTOR_HPP

#include "Pointer.hpp"
#include <string>
#include <vector>

class CharacterSelector {
public:
	CharacterSelector(const int OldSelection) : Res(OldSelection) { };
	int Action();
private:
	size_t Page = 0;
	int Res = 0, PrevPos = -320, CurPos = -320, NextPos = 320;
	bool Done = false, FullDone = false, FadeIn = true, SwipeDir = true, DoSwipe = false, InitialSwipe = true;
	float Cubic = 0.0f;
	int16_t FAlpha = 255;

	void DrawPage(const size_t Page, const int AddOffs);
	void SelectCharacter(const uint8_t Idx);
	void PrevPage();
	void NextPage();
	bool CanGoNext() const;
	void Cancel();
	void Handler();

	const std::vector<FuncCallback> BottomPos = {
		{ 0, 15, 25, 215, [this]() { this->PrevPage(); } },
		{ 295, 15, 25, 215, [this]() { this->NextPage(); } }
	};

	const std::vector<FuncCallback> Characters = {
		{ 40, 9, 48, 48, [this]() { this->SelectCharacter(0); } },
		{ 103, 9, 48, 48, [this]() { this->SelectCharacter(1); } },
		{ 166, 9, 48, 48, [this]() { this->SelectCharacter(2); } },
		{ 229, 9, 48, 48, [this]() { this->SelectCharacter(3); } },

		{ 40, 66, 48, 48, [this]() { this->SelectCharacter(4); } },
		{ 103, 66, 48, 48, [this]() { this->SelectCharacter(5); } },
		{ 166, 66, 48, 48, [this]() { this->SelectCharacter(6); } },
		{ 229, 66, 48, 48, [this]() { this->SelectCharacter(7); } },

		{ 40, 123, 48, 48, [this]() { this->SelectCharacter(8); } },
		{ 103, 123, 48, 48, [this]() { this->SelectCharacter(9); } },
		{ 166, 123, 48, 48, [this]() { this->SelectCharacter(10); } },
		{ 229, 123, 48, 48, [this]() { this->SelectCharacter(11); } },

		{ 40, 180, 48, 48, [this]() { this->SelectCharacter(12); } },
		{ 103, 180, 48, 48, [this]() { this->SelectCharacter(13); } },
		{ 166, 180, 48, 48, [this]() { this->SelectCharacter(14); } },
		{ 229, 180, 48, 48, [this]() { this->SelectCharacter(15); } }
	};
};

#endif