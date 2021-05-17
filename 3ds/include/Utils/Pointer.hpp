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

#ifndef _3DZWEI_POINTER_HPP
#define _3DZWEI_POINTER_HPP

#include <3ds.h> // touchPosition.
#include <functional> // std::functional.

struct FuncCallback {
	float X;
	float Y;
	int W;
	int H;
	std::function<void()> Func = []() { }; // Optional.
};

namespace Pointer {
	/* Pointer related. */
	extern float X, Y;
	extern bool OnTop;

	bool Clicked(const FuncCallback CBack, const bool CallFunc = true);
	void ScrollHandling(const uint32_t Held, const bool InGame = false);

	void Draw(const float Scale = 1);
	void SetPos(const FuncCallback CBack);
	void SetPos(const float X, const float Y);
};

/* This isn't really pointer related, but also kinda FuncCallback related which is declared at this file. */
bool Touched(const FuncCallback CBack, touchPosition T, const bool CallFunc = true);

#endif