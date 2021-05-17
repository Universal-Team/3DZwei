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
	CharacterSelector(const int8_t OldSelection) : Res(OldSelection) { };
	int Action();
private:
	int Res = 0, PrevPos = -400, CurPos = -400, NextPos = 400;
	bool Done = false, FullDone = false, FadeIn = true, SwipeDir = true, DoSwipe = false, InitialSwipe = true;
	float Cubic = 0.0f;
	uint8_t FAlpha = 255;

	void PrevChar();
	void NextChar();
	void Select();
	void Cancel();
	void CharLogic();

	const std::vector<FuncCallback> Positions = {
		{ 0, 25, 25, 215, [this]() { this->PrevChar(); } },
		{ 375, 25, 25, 215, [this]() { this->NextChar(); } },
		{ 147, 200, 106, 20, [this]() { this->Select(); } }
	};

	const std::vector<FuncCallback> BottomPos = {
		{ 70, 75, 25, 90, [this]() { this->PrevChar(); } },
		{ 123, 109, 74, 22, [this]() { this->Select(); } },
		{ 225, 75, 25, 90, [this]() { this->NextChar(); } },
		{ 0, 223, 17, 17, [this]() { this->Cancel(); } }
	};
};

#endif