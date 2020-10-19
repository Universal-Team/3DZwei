/*
*   This file is part of 3DZwei-Core
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

#ifndef _3DZWEI_CORE_GAME_HPP
#define _3DZWEI_CORE_GAME_HPP

#include "ai.hpp"
#include "coreHelper.hpp"

class Game {
public:
	Game(int pairs = 10, bool useAI = true, bool rememberMoreAI = false);
	void generateCards(int pairs);
	bool isCollected(int index) const;
	void setCollected(int index, bool collected);
	bool checkIfMatches() const;
	bool setCardPair();
	bool returnIfShown(int index) const;
	void setShown(int index, bool show);
	bool play(int index);
	int getCard(int index) const;
	Players getCurrentPlayer() const;
	void setCurrentPlayer(Players player);
	void nextPlayer();
	GameWinner checkOver() const;
	void restart();
	int getPairs(Players player) const;
	CardSelectMode getCardSelect() const;
	void setCardSelect(CardSelectMode cardSelect);
	int getWins(Players player) const;
	void setWins(Players player, int wins);
	int getPairs() const;
	void setPairs(int pairs);
	int returnProperPair() const;

	/* AI stuff. */
	int doRandomTurn();
	int doPrediction();
	int doPredictLonger(int amountToRemember = 2);
	int doAITurn(bool predict = false, int amount = -1);
private:
	std::unique_ptr<AI> ai; // Our AI.
	std::vector<CardStruct> field; // The game's field.
	int player1 = 0, player2 = 0;
	bool useAI, rememberMoreAI = false;
	Players currentPlayer = Players::Player1;
	int card1 = -1, card2 = -1, p1Wins = 0, p2Wins = 0, pairs = 10;
	CardSelectMode cardSelect = CardSelectMode::DrawFirst;
};

#endif