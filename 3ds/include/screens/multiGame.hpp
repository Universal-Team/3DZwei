/*
*   This file is part of 3DZwei
*   Copyright (C) 2020 Universal-Team
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

#ifndef _3DZWEI_MULTI_GAME_HPP
#define _3DZWEI_MULTI_GAME_HPP

#include "common.hpp"
#include "game.hpp"
#include "structs.hpp"

#include <vector>

class MultiGame : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	MultiGame();
private:
	std::unique_ptr<Game> currentGame;
	bool useDelay = false, useAI = false, betterPredict = false;
	int selectedCard = 0, delay, avatar1, avatar2, wins, page = 0, pairAmount = 0;
	// Logics.
	void playerLogic(u32 hDown, u32 hHeld, touchPosition touch);
	void AILogic(u32 hDown);

	const std::vector<CardStr> cardPos = {
		// Row 1.
		{20.5, 8.5},
		{20.5 + (1 * 56), 8.5},
		{20.5 + (2 * 56), 8.5},
		{20.5 + (3 * 56), 8.5},
		{20.5 + (4 * 56), 8.5},
		// Row 2.
		{20.5, 8.5 + (1 * 56)},
		{20.5 + (1 * 56), 8.5 + (1 * 56)},
		{20.5 + (2 * 56), 8.5 + (1 * 56)},
		{20.5 + (3 * 56), 8.5 + (1 * 56)},
		{20.5 + (4 * 56), 8.5 + (1 * 56)},
		// Row 3.
		{20.5, 8.5 + (2 * 56)},
		{20.5 + (1 * 56), 8.5 + (2 * 56)},
		{20.5 + (2 * 56), 8.5 + (2 * 56)},
		{20.5 + (3 * 56), 8.5 + (2 * 56)},
		{20.5 + (4 * 56), 8.5 + (2 * 56)},
		// Row 4.
		{20.5, 8.5 + (3 * 56)},
		{20.5 + (1 * 56), 8.5 + (3 * 56)},
		{20.5 + (2 * 56), 8.5 + (3 * 56)},
		{20.5 + (3 * 56), 8.5 + (3 * 56)},
		{20.5 + (4 * 56), 8.5 + (3 * 56)}
	};
};

#endif