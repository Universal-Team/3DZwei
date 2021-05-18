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

#ifndef _3DZWEI_SPLASH_OVERLAY_HPP
#define _3DZWEI_SPLASH_OVERLAY_HPP

#include "Pointer.hpp"
#include <vector>

class SplashOverlay {
public:
	SplashOverlay() {
		this->CardScale[0] = 1.0f, this->CardScale[1] = 1.0f;
		this->CardFlipped[0] = false, this->CardFlipped[1] = false;
	};
	void Action();
private:
	enum class States : uint8_t {
		GotoFirst = 0,
		ClickOnFirst = 1,
		GotoSecond = 2,
		ClickOnSecond = 3,
		FadeOut = 4
	};

	int16_t FAlpha = 255;
	int16_t X = 200, Y = 120; // Pointer Pos.
	int16_t CoreMoved = 0; // The Moved Core Logo.
	States State = States::GotoFirst; // The Current State.
	bool Done = false;

	/* Card Flip related. */
	bool CardFlipped[2] = { false }, FlipDone[2] = { false };
	float CardScale[2] = { 1.0f }, Cubic = 0.0f;

	/* Splash States. */
	void State1();
	void State2();
	void State3();
	void State4();
	void State5();
	void StateHandler();

	const std::vector<FuncCallback> Cards = {
		{ 72, 35, 55, 55 },
		{ 172, 35, 55, 55 },
		{ 272, 35, 55, 55 },

		{ 72, 105, 55, 55 },
		{ 172, 105, 55, 55 },
		{ 272, 105, 55, 55 },

		{ 72, 175, 55, 55 },
		{ 172, 175, 55, 55 },
		{ 272, 175, 55, 55 }
	};
};

#endif