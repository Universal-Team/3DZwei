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

#ifndef _3DZWEI_GAME_SETTINGS_HPP
#define _3DZWEI_GAME_SETTINGS_HPP

#include "Pointer.hpp"
#include "StackMem.hpp"
#include <string>
#include <vector>

class GameSettings {
public:
	enum class GameModes : uint8_t { NormalPlay = 0, TryPlay = 1 };
	enum class RoundStarter : uint8_t { Player1 = 0, Player2 = 1, Random = 2 };

	/* Game Parameters. */
	struct GameParams {
		bool AIUsed = false; // If an AI is being used at all.
		bool CardDelayUsed = false; // If the card delay should be used.. or if it's over a button press.
		StackMem::AIMethod Method = StackMem::AIMethod::Random; // AI Method / Difficulty.
		uint8_t CardDelay = 0; // The card delay before the card gets hidden again after a turn.
		uint8_t RoundsToWin = 3; // Rounds to win the game.
		uint8_t Characters[2] = { 0 }; // Avatar Indexes.
		std::string Names[2] = { "Player1", "Player2" }; // Player Names.
		GameSettings::GameModes GameMode = GameSettings::GameModes::NormalPlay; // Game Mode.
		uint8_t Wins[2] = { 0 }; // Wins of both players.
		size_t PlayerPairs[2] = { 0 }; // Player Pairs for both players.
		size_t Tries = 0; // Amount of tries.
		RoundStarter Starter = RoundStarter::Player1; // The Round Starter of the game.
		bool CancelGame = false; // If Game should be cancelled or not.
	};

	GameSettings() { };
	GameParams Action();
private:
	enum class Tabs : uint8_t { General = 0, Player = 1 };

	GameParams Params;
	bool Done = false;
	Tabs Tab = Tabs::General;

	/* Both Tabs operations. */
	void OK();
	void Cancel();
	void GeneralTab() { this->Tab = Tabs::General; };
	void PlayerTab() { this->Tab = Tabs::Player; };
	void TabLogic();

	/* General Tab operations. */
	void ToggleAI();
	void ToggleDelay();
	void ToggleGameMode();
	void SelectAIMethod();
	void SelectCards();
	void SetCardDelay();
	void SetWinRounds();

	/* Player Tab operations. */
	void SetName(const bool AI);
	void SelectPicture(const bool AI);
	void ToggleStarter();

	/* General Tab. */
	const std::vector<FuncCallback> GeneralPos = {
		{ 0, 0, 160, 20, [this]() { this->GeneralTab(); } },
		{ 160, 0, 160, 20, [this]() { this->PlayerTab(); } },

		{ 150, 35, 24, 24, [this]() { this->ToggleGameMode(); } },

		{ 150, 75, 24, 24, [this]() { this->ToggleDelay(); } },
		{ 200, 75, 24, 24, [this]() { this->SetCardDelay(); } },

		{ 150, 115, 24, 24, [this]() { this->SelectCards(); } },

		{ 150, 155, 24, 24, [this]() { this->ToggleAI(); } },
		{ 200, 155, 24, 24, [this]() { this->SelectAIMethod(); } },

		{ 150, 195, 24, 24, [this]() { this->SetWinRounds(); } },

		{ 0, 223, 17, 17, [this]() { this->Cancel(); } },
		{ 303, 223, 17, 17, [this]() { this->OK(); } }
	};

	/* Player Tab. */
	const std::vector<FuncCallback> PlayerPos = {
		{ 0, 0, 160, 20, [this]() { this->GeneralTab(); } },
		{ 160, 0, 160, 20, [this]() { this->PlayerTab(); } },

		{ 30, 30, 120, 120, [this]() { this->SelectPicture(false); } },
		{ 170, 30, 120, 120, [this]() { this->SelectPicture(true); } },

		{ 37, 159, 106, 20, [this]() { this->SetName(false); } },
		{ 177, 159, 106, 20, [this]() { this->SetName(true); } },

		{ 150, 195, 24, 24, [this]() { this->ToggleStarter(); } },

		{ 0, 223, 17, 17, [this]() { this->Cancel(); } },
		{ 303, 223, 17, 17, [this]() { this->OK(); } }
	};
};

#endif