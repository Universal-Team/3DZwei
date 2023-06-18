/*
*   This file is part of 3DZwei
*   Copyright (C) 2020-2023 Universal-Team
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

#ifndef _3DZWEI_GAME_HELPER_HPP
#define _3DZWEI_GAME_HELPER_HPP

#include "GameSettings.hpp" // Game Params.
#include "StackMem.hpp" // Game class.
#include "Pointer.hpp"
#include <vector> // Positions.

class GameHelper {
public:
	enum class LogicState : uint8_t { Nothing = 0, P1Won = 1, P2Won = 2, Tie = 3 };

	GameHelper(const GameSettings::GameParams Params = { }); // Constructor.
	void StartGame(const bool AlreadyInitialized = true, const GameSettings::GameParams Params = { }, const bool P2Won = true);

	void DrawTryPlay(void) const;
	void DrawNormalPlay(void) const;
	void DrawTop(void) const;
	void DrawField(const bool ShowPointer = true) const;

	LogicState Logic(const uint32_t Down, const uint32_t Held, const uint32_t Repeat, const touchPosition T);
	GameSettings::GameParams &ReturnParams() { return this->Params; }
private:
	std::unique_ptr<StackMem> Game = nullptr;
	GameSettings::GameParams Params = { };
	size_t Page = 0, Selection = 0;
	bool RefreshFrame = true; // Used for the delay mode to properly display the card.

	/* Turn Card variables. The clicked state AND card scale (0.0f up to 1.0f). */
	bool CardClicked[2] = { false };
	float ClickedScale[2] = { 1.0f };

	/* Page related. */
	bool CanGoForward(const size_t CurPage) const;
	bool PrevPage();
	bool NextPage();

	/* Utility related. */
	void CheckCard(const uint8_t Idx);
	std::vector<uint8_t> GetAnimationIdx(const uint8_t Round) const;
	uint8_t GetIndexCount() const;

	/* Turn based related. */
	LogicState TurnChecks();
	LogicState AILogic(const uint32_t Down);
	LogicState PlayerLogic(const uint32_t Down, const uint32_t Held, const uint32_t Repeat, const touchPosition T);

	/* Include all Animations here. */
	void StartGameAnimationFalling();
	void StartGameAnimationGrowing();
	void StartGameAnimation();
	void PageAnimation(const bool Forward);
	void AIPageAnimation(const int Page);
	void PickAnimation(const size_t Idx);
	void HideAnimation();
	void ShrinkAnimation();
	void EndGameAnimation();

	/* Card Positions. */
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