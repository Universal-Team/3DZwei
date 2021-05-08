/*
*   This file is part of StackMem
*   Copyright (C) 2021 SuperSaiyajinStackZ
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

#ifndef _STACKMEM_HPP
#define _STACKMEM_HPP

#include <memory> // std::unique_ptr.
#include <random> // std::mt19937.
#include <vector> // std::vector.


/*
	StackMem's' main class implementation.
	Written by SuperSaiyajinStackZ.
*/
class StackMem {
public:
	/* All used enum classes, which also need to be accessible for outside this class because of checks, here. */
	enum class AIDifficulty : uint8_t { Random = 0, Hard = 1, Extreme = 2 }; // AI Difficulties.
	enum class TurnState : uint8_t { DrawFirst = 0, DrawSecond = 1, DoCheck = 2 }; // The Current Turn State.
	enum class GameState : uint8_t { NotOver = 0, Tie = 1, Player1 = 2, Player2 = 3 }; // The Game State.
	enum class Players : uint8_t { Player1 = 0, Player2 = 1 }; // The Current Player.


	StackMem(const size_t Pairs = 10, const bool AIUsed = true, const AIDifficulty Difficulty = AIDifficulty::Random);

	/* The actual Game initialization thing. Also called at constructor. */
	void InitializeGame(const size_t Pairs, const bool AIUsed = true, const AIDifficulty Difficulty = AIDifficulty::Random, const bool DoSeed = false);

	/* Card Collected things. */
	bool IsCardCollected(const size_t Idx) const;
	void SetCardCollected(const size_t Idx, const bool Collected);

	/* Card Shown things. */
	bool IsCardShown(const size_t Idx) const;
	void SetCardShown(const size_t Idx, const bool Shown);

	/* CardType. */
	int GetCardType(const size_t Idx) const;

	/* Some Utility related things. */
	bool DoCheck();
	bool DoPlay(const size_t Idx = 0);
	GameState CheckGameState() const;
	size_t GetPairs() const { return this->Pairs; }; // Get the amount of Pairs used in the Current Game.
	size_t GetPlayerPairs(const Players P) const;
	int GetProperPair() const;
	bool AIEnabled() const { return this->AIUsed; }; // Get if the AI is enabled / used whatever.

	/* Turn State related things. */
	TurnState GetState() const { return this->State; }; // Gets the Current State.
	void SetState(const TurnState State) { this->State = State; }; // Sets the Current State.

	/* Player related things. */
	Players GetCurrentPlayer() const { return this->CurrentPlayer; }; // Gets the Current Player.
	void SetCurrentPlayer(const Players P) { this->CurrentPlayer = P; }; // Sets the Current Player.
	void NextPlayer();
	void SelectRandomPlayer();

	/* AI Play! */
	int AIPlay();
	AIDifficulty GetDifficulty() const;
private:
	std::mt19937 RandomEngine;

	/*
		CardStruct for the Game.
		int CardType: The "type" of the card.. also known as the index of the pair.
		bool Shown: If the card is currently shown (true) or not (false).
		bool Collected: If the card got collected already (true) or not (false).
	*/
	struct CardStruct { int CardType = 0; bool Shown = false; bool Collected = false; };

	/*
		StackMem's AI class implementation.
		Written by SuperSaiyajinStackZ.
	*/
	class AI {
	public:
		AI(const AIDifficulty Difficulty = AIDifficulty::Random);

		/* Clear and Set. */
		void ClearAICards();
		void SetLastCards(const int Idx1, const int Idx2);

		/* Return first and second cards. */
		int GetFirst(const size_t Idx = 0) const;
		int GetSecond(const size_t Idx = 0) const;

		/* Some other returns and sets. */
		size_t GetSize() const { return this->Mind.size(); };
		AIDifficulty GetDifficulty() const { return this->Difficulty; };
		void SetDifficulty(const AIDifficulty Difficulty) { this->Difficulty = Difficulty; };
	private:
		std::vector<std::pair<int, int>> Mind = { }; // The mind for the Hard + Extreme AI Mode.
		AIDifficulty Difficulty = AIDifficulty::Random; // The Difficulty of the AI.
	};


	bool AIUsed = false; // If an AI is used.
	size_t Pairs = 0, PlayerPairs[2] = { 0 }; // The amount of Pairs of the current game + pairs from the players.
	int PlayCards[2] = { -1 }; // The play cards containing the indexes of the current turn.
	std::unique_ptr<AI> _AI = nullptr; // The AI used in the current Game.
	std::vector<CardStruct> Gamefield = { }; // The Game's field.
	Players CurrentPlayer = Players::Player1; // The Current Player.
	TurnState State = TurnState::DrawFirst; // The current Turn State.

	/* Keep those methods private, because you shouldn't mess with it. */
	bool CheckMatch() const;
	void GenerateField(const size_t Pairs);
	int AIRandomPlay();
	int AIHardPlay();
	int AIExtremePlay();
};

#endif