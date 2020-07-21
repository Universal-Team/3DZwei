/*
*   This file is part of 3DZwei
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

#ifndef _3DZWEI_GAME_SCREEN_HPP
#define _3DZWEI_GAME_SCREEN_HPP

#include "common.hpp"
#include "game.hpp"
#include "structs.hpp"

#include <vector>

class GameScreen : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	GameScreen(bool useDelay = true, bool useAI = false, bool doBetterPredict = false);
private:
	std::unique_ptr<Game> currentGame;
	bool useDelay = false, useAI = false, betterPredict = false;
	int selectedCard = 0, delay, avatar1, avatar2, wins, page = 0, pairAmount = 0;
	// Logics.
	void playerLogic(u32 hDown, u32 hHeld, touchPosition touch);
	void AILogic(u32 hDown);

	const std::vector<Structs::ButtonPos> cardPos = {
		// Row 1.
		{10, 2, 55, 55}, // 1.
		{71, 2, 55, 55}, // 2.
		{132, 2, 55, 55}, // 3.
		{193, 2, 55, 55}, // 4.
		{254, 2, 55, 55}, // 5.
		// Row 2.
		{10, 61, 55, 55}, // 1.
		{71, 61, 55, 55}, // 2.
		{132, 61, 55, 55}, // 3.
		{193, 61, 55, 55}, // 4.
		{254, 61, 55, 55}, // 5.
		// Row 3.
		{10, 122, 55, 55}, // 1.
		{71, 122, 55, 55}, // 2.
		{132, 122, 55, 55}, // 3.
		{193, 122, 55, 55}, // 4.
		{254, 122, 55, 55}, // 5.
		// Row 4.
		{10, 183, 55, 55}, // 1.
		{71, 183, 55, 55}, // 2.
		{132, 183, 55, 55}, // 3.
		{193, 183, 55, 55}, // 4.
		{254, 183, 55, 55} // 5.
	};
};

#endif