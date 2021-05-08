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

#ifndef _3DZWEI_GAME_SCREEN_HPP
#define _3DZWEI_GAME_SCREEN_HPP

#include "GameSettings.hpp"
#include "Pointer.hpp"
#include "screen.hpp"
#include "StackMem.hpp"
#include <vector>

class GameScreen : public Screen {
public:
	GameScreen(const GameSettings::GameParams Params = { });
	void Draw(void) const override;
	void Logic(uint32_t Down, uint32_t Held, touchPosition T) override;
private:
	/* Some Variables. */
	std::unique_ptr<StackMem> Game = nullptr;
	GameSettings::GameParams Params; // Game Parameters!
	size_t Page = 0, P1Wins = 0, P2Wins = 0, Tries = 0; // Current Page, Player 1 Wins, Player 2 Wins and the amount of Tries.
	bool RefreshFrame = true; // Used for the delay mode to properly display the card.

	/* Functions, which are used in both play modes. */
	bool CanGoNext() const;
	void PrevPage();
	void NextPage();
	void CheckCard(const uint8_t Idx);

	/* Normal Play Mode. */
	void DrawNormalPlay(void) const;
	void PlayerLogic(const uint32_t Down, const uint32_t Held, const touchPosition T);
	void AILogic(const uint32_t Down);
	void NormalPlayLogic(const uint32_t Down, const uint32_t Held, const touchPosition T);

	/* Try Play Mode. */
	void DrawTryPlay(void) const;
	void TryPlayLogic(const uint32_t Down, const uint32_t Held, const touchPosition T);

	const std::vector<FuncCallback> CPos = {
		/* Row 1. */
		{ 20.5, 8.5, 55, 55, [this]() { this->CheckCard(0); } },
		{ 20.5 + (1 * 56), 8.5, 55, 55, [this]() { this->CheckCard(1); } },
		{ 20.5 + (2 * 56), 8.5, 55, 55, [this]() { this->CheckCard(2); } },
		{ 20.5 + (3 * 56), 8.5, 55, 55, [this]() { this->CheckCard(3); } },
		{ 20.5 + (4 * 56), 8.5, 55, 55, [this]() { this->CheckCard(4); } },

		/* Row 2. */
		{ 20.5, 8.5 + (1 * 56), 55, 55, [this]() { this->CheckCard(5); } },
		{ 20.5 + (1 * 56), 8.5 + (1 * 56), 55, 55, [this]() { this->CheckCard(6); } },
		{ 20.5 + (2 * 56), 8.5 + (1 * 56), 55, 55, [this]() { this->CheckCard(7); } },
		{ 20.5 + (3 * 56), 8.5 + (1 * 56), 55, 55, [this]() { this->CheckCard(8); } },
		{ 20.5 + (4 * 56), 8.5 + (1 * 56), 55, 55, [this]() { this->CheckCard(9); } },

		/* Row 3. */
		{ 20.5, 8.5 + (2 * 56), 55, 55, [this]() { this->CheckCard(10); } },
		{ 20.5 + (1 * 56), 8.5 + (2 * 56), 55, 55, [this]() { this->CheckCard(11); } },
		{ 20.5 + (2 * 56), 8.5 + (2 * 56), 55, 55, [this]() { this->CheckCard(12); } },
		{ 20.5 + (3 * 56), 8.5 + (2 * 56), 55, 55, [this]() { this->CheckCard(13); } },
		{ 20.5 + (4 * 56), 8.5 + (2 * 56), 55, 55, [this]() { this->CheckCard(14); } },

		/* Row 4. */
		{ 20.5, 8.5 + (3 * 56), 55, 55, [this]() { this->CheckCard(15); } },
		{ 20.5 + (1 * 56), 8.5 + (3 * 56), 55, 55, [this]() { this->CheckCard(16); } },
		{ 20.5 + (2 * 56), 8.5 + (3 * 56), 55, 55, [this]() { this->CheckCard(17); } },
		{ 20.5 + (3 * 56), 8.5 + (3 * 56), 55, 55, [this]() { this->CheckCard(18); } },
		{ 20.5 + (4 * 56), 8.5 + (3 * 56), 55, 55, [this]() { this->CheckCard(19); } },

		/* Pages switches. */
		{ 0, 0, 15, 240, [this]() { this->PrevPage(); } },
		{ 300, 0, 20, 240, [this]() { this->NextPage(); } }
	};
};

#endif