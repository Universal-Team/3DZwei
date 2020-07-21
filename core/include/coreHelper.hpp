/*
*   This file is part of 3DZwei-Core
*   Copyright (C) 2020 SuperSaiyajinStackZ
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

#ifndef _3DZWEI_CORE_HELPER_HPP
#define _3DZWEI_CORE_HELPER_HPP

#include <memory>
#include <random>

inline std::mt19937 randomGen; // Random generation value.

// The CardSelect Mode.
enum class CardSelectMode {
	DrawFirst,
	DrawSecond,
	DoCheck
};

// The winner of the match.
enum class GameWinner {
	Player1,
	Player2,
	None,
	NotOver
};

// The current Player.
enum class Players {
	Player1,
	Player2
};

// For the gamefield.
struct CardStruct {
	bool Used;
	int CardType;
	bool Shown;
};

namespace CoreHelper {
	void generateSeed();
}

#endif