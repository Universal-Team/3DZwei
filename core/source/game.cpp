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

#include "coreHelper.hpp"
#include "game.hpp"
#include <algorithm>
#include <string>
#include <vector>

// Constructor.
Game::Game(int pairs, bool useAI) {
	this->useAI = useAI;
	if (this->useAI) ai = std::make_unique<AI>();
	CoreHelper::generateSeed(); // Seed one time.
	this->pairs = pairs;
	this->generateCards(this->pairs);
	this->card1 = -1;
	this->card2 = -1;
	this->cardSelect = 0;
}

// Generate new field.
void Game::generateCards(int pairs) {
	std::vector<int> tempCards;

	this->field.clear();

	// First card for the pair!
	for (int i = 0; i < pairs; i++) {
		tempCards.push_back(i);
	}
	// Second card for the pair!
	for (int i = 0; i < pairs; i++) {
		tempCards.push_back(i);
	}

	std::shuffle(tempCards.begin(), tempCards.end(), randomGen);

	for (int i = 0; i < (int)tempCards.size(); i++) {
		this->field.push_back({false, tempCards[i], false});
	}
}

// Get and Set used state.
bool Game::returnIfUsed(int index) {
	if (index > (this->pairs * 2)-1) return true; // Out of scope. No idea what else to return in that case.
	return this->field[index].Used;
}
void Game::setUsed(int index, bool isUsed) {
	if (index > (this->pairs * 2)-1) return; // Out of scope.
	this->field[index].Used = isUsed;
}

// Return if both cards matches.
bool Game::checkIfMatches() {
	if (this->card1 < 20 && this->card2 < 20 && this->card1 != -1 && this->card2 != -1) {
		if (this->field[this->card1].CardType == this->field[this->card2].CardType)	return true;
		else return false;
	} else {
		return false;
	}
}

// Set cardpair to a specific player.
bool Game::setCardPair() {
	if (this->card1 != -1 && this->card2 != -1) {
		if (this->checkIfMatches()) {
			if (this->currentPlayer == 0) {
				this->player1.push_back(this->card1);
				this->player1.push_back(this->card2);
			} else {
				this->player2.push_back(this->card1);
				this->player2.push_back(this->card2);
			}

			// Set that we used it.
			this->setUsed(this->card1, true);
			this->setUsed(this->card2, true);
			return true;
		} else {
			if (this->useAI)	ai->setLastCards(this->card1, this->card2);
			this->setShown(this->card1, false); this->setShown(this->card2, false);
			this->setUsed(this->card1, false); this->setUsed(this->card2, false);
			// Heh, nope.
			return false;
		}
	} else {
		// Nope, index == -1.
		return false;
	}
}

// Get and Set shown state.
bool Game::returnIfShown(int index) {
	if (index > (this->pairs * 2)-1)	return false; // Out of scope.
	return this->field[index].Shown;
}
void Game::setShown(int index, bool show) {
	if (index > (this->pairs * 2)-1)	return; // Out of scope.
	this->field[index].Shown = show;
}

// Play if you can.
bool Game::play(int index) {
	if (this->returnIfUsed(index) != true) {
		if (this->cardSelect == 0) {
			this->setShown(index, true);
			this->setUsed(index, true);
			this->card1 = index;
			this->cardSelect = 1;
			return true;
		} else if (this->cardSelect == 1) {
			this->setShown(index, true);
			this->setUsed(index, true);
			this->card2 = index;
			this->cardSelect = 2;
			return true;
		}
	} else {
		return false;
	}

	return false;
}

// Get a cardtype from an index.
int Game::getCard(int index) {
	if (index > (this->pairs * 2)-1)	return -1; // Out of scope.
	return this->field[index].CardType;
}

// Get and Set the current Player.
int Game::getCurrentPlayer() { return this->currentPlayer; }
void Game::setCurrentPlayer(int player) { this->currentPlayer = player; }
void Game::nextPlayer() {
	if (this->currentPlayer == 0)	this->currentPlayer = 1;
	else							this->currentPlayer = 0;
}

// Check if all cards are used and return the winner.
int Game::checkOver() {
	if ((int)this->player1.size() + (int)this->player2.size() == (this->pairs * 2)) {
		if (this->player1.size() > this->player2.size())	return 1; // Player 1 wins!
		else if (this->player2.size() > this->player1.size())	return 2; // Player 2 wins!
		else if (this->player1.size() == this->player2.size())	return 3; // No one wins!
	} else {
		return -1; // Nah, not all used.
	}

	return -1; // To not cause compile conflicts.
}

// Restart the game.
void Game::restart() {
	this->player1.clear();
	this->player2.clear();
	if (this->useAI)	ai->clearCards();
	this->card1 = -1;
	this->card2 = -1;
	this->cardSelect = 0;
	this->generateCards(this->pairs);
}

// Get the amount of pairs from the players.
int Game::getPairs(int player) {
	if (player == 0) {
		return this->player1.size() / 2;
	} else {
		return this->player2.size() / 2;
	}
}

// Get and Set the current card selection.
int Game::getCardSelect() {
	return this->cardSelect;
}
void Game::setCardSelect(int cardSelect) {
	this->cardSelect = cardSelect;
}

// Get and Set wins.
int Game::getWins(int player) {
	if (player == 0) {
		return this->p1Wins;
	} else {
		return this->p2Wins;
	}
}
void Game::setWins(int player, int wins) {
	if (player == 0) {
		this->p1Wins = wins;
	} else {
		this->p2Wins = wins;
	}
}

// Get and Set pairs.
int Game::getPairs() {
	return this->pairs;
}
void Game::setPairs(int pairs) {
	this->pairs = pairs; // Because we change it there, we also need to reload the field.
	this->restart();
}

// Broken. Needs fixes.
int Game::doPredict(int amountToRemember) {
	if (this->useAI) {
		if (amountToRemember > ai->getSize()-1)	return this->doRandom(); // Heh, nope. Size is too large.

		if (amountToRemember != -1) {
			for (int i = 0; i < ai->getSize()-amountToRemember; i++) {
				if (this->getCard(ai->getFirst(i)) == this->card1) {
					if (!this->returnIfUsed(ai->getFirst(i)))	return this->getCard(ai->getFirst(i));
					else return this->doRandom();
				}
				if (this->getCard(ai->getSecond(i)) == this->card1) {
					if (!this->returnIfUsed(ai->getSecond(i)))	return this->getCard(ai->getSecond(i));
					else return this->doRandom();
				}
			}
		} else {
			for (int i = 0; i < ai->getSize(); i++) {
				if (this->getCard(ai->getFirst(i)) == this->card1) {
					if (!this->returnIfUsed(ai->getFirst(i)))	return this->getCard(ai->getFirst(i));
					else return this->doRandom();
				}
				if (this->getCard(ai->getSecond(i)) == this->card1) {
					if (!this->returnIfUsed(ai->getSecond(i)))	return this->getCard(ai->getSecond(i));
					else return this->doRandom();
				}
			}
		}
		return this->doRandom();
	} else {
		return this->doRandom();
	}
}

int Game::doRandom() {
	if (this->cardSelect == 0 || this->cardSelect == 1) {
		std::vector<int> possiblePlays;
		for (int i = 0; i < (this->getPairs() * 2); i++) {
			if (!this->returnIfUsed(i))	possiblePlays.push_back({i});
		}

		if (possiblePlays.size() > 0) {
			int index = ((randomGen()) % possiblePlays.size()-1) + 0;
			return possiblePlays[index];
		} else {
			return -1;
		}
	}
	return -1;
}

int Game::doComTurn(bool predict, int amountToRemember) {
	if (predict)	return this->doPredict(amountToRemember);
	else			return this->doRandom();
}