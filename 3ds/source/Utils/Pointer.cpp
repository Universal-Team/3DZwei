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
#include "Pointer.hpp"

/* Define the actual variables. */
float Pointer::X = 0, Pointer::Y = 0;
bool Pointer::OnTop = false;

/* Sizes of the pointer + distances. */
#define PTR_X_SIZE 19
#define PTR_Y_SIZE 16
#define X_DIST 3
#define Y_DIST 4

#define PTR_SPEED _3DZwei::CFG->PointerSpeed(); // Pointer Speed.

/*
	Handles the Pointer Scrolling.

	const uint32_t Held: The key held input variable, also known as hidKeysHeld() on 3DS.
*/
void Pointer::ScrollHandling(const uint32_t Held) {
	if (Held & KEY_LEFT) {
		if (Pointer::X >= 0) {
			Pointer::X -= PTR_SPEED;

			if (Pointer::X < 0) Pointer::X = 0; // Make it not go below the limit.
		}
	}

	if (Held & KEY_RIGHT) {
		if (Pointer::X <= ((Pointer::OnTop ? 400 : 320) - PTR_X_SIZE)) {
			Pointer::X += PTR_SPEED;

			if (Pointer::X > (Pointer::OnTop ? 400 : 320) - PTR_X_SIZE) Pointer::X = (Pointer::OnTop ? 400 : 320) - PTR_X_SIZE; // Make it not go over the limit.
		}
	}

	if (Held & KEY_UP) {
		if (Pointer::Y >= -Y_DIST) {
			Pointer::Y -= PTR_SPEED;

			if (Pointer::Y < -Y_DIST) Pointer::Y = -Y_DIST; // Make it not go below the limit.
		}
	}

	if (Held & KEY_DOWN) {
		if (Pointer::Y <= (240 - PTR_Y_SIZE)) {
			Pointer::Y += PTR_SPEED;

			if (Pointer::Y > (240 - PTR_Y_SIZE)) Pointer::Y = (240 - PTR_Y_SIZE); // Make it not go over the limit.
		}
	}
};

/*
	Returns, of a specific Callback Position was clicked.. and if so, execute optionally it's function.

	const FuncCallback CBack: The Callback which to check.
	const bool CallFunc: If the function should be called if clicked or not.
*/
bool Pointer::Clicked(const FuncCallback CBack, const bool CallFunc) {
	if (((Pointer::X + X_DIST) >= CBack.X && (Pointer::X + X_DIST) <= CBack.X + CBack.W)
		&& ((Pointer::Y + Y_DIST) >= CBack.Y && (Pointer::Y + Y_DIST) <= CBack.Y + CBack.H)) {
			if (CallFunc) CBack.Func(); // Call the function, if specified.
			return true;
	}

	return false;
};

/*
	Returns, of a specific Callback Position was touched.. and if so, execute optionally it's function.

	const FuncCallback CBack: The Callback which to check.
	touchPosition T: The touchPosition variable.
	const bool CallFunc: If the function should be called if clicked or not.
*/
bool Touched(const FuncCallback CBack, touchPosition T, const bool CallFunc) {
	if (T.px >= CBack.X && T.px <= (CBack.X + CBack.W) && T.py >= CBack.Y && T.py <= (CBack.Y + CBack.H)) {
		if (CallFunc) CBack.Func();
		return true;
	}

	return false;
};


/*
	Draw the Pointer, also with Position.

	const float Scale: Optional Pointer Scale.
*/
void Pointer::Draw(const float Scale) { Gui::DrawSprite(GFX::Sprites, sprites_pointer_idx, Pointer::X, Pointer::Y); };

/* Set Pointer Position from two float's, or the Pointer Struct. */
void Pointer::SetPos(const FuncCallback CBack) { Pointer::X = CBack.X, Pointer::Y = CBack.Y; };
void Pointer::SetPos(const float X, const float Y) { Pointer::X = X, Pointer::Y = Y; };