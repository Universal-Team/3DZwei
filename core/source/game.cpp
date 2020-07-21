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
Game::Game(int pairs, bool useAI, bool rememberMoreAI) {
	this->rememberMoreAI = rememberMoreAI;
	this->useAI = useAI;
	if (this->useAI) ai = std::make_unique<AI>(this->rememberMoreAI);
	CoreHelper::generateSeed(); // Seed one time.
	this->pairs = pairs;
	this->generateCards(this->pairs);
	this->card1 = -1;
	this->card2 = -1;
	this->p1Wins = 0;
	this->p2Wins = 0;
	this->cardSelect = CardSelectMode::DrawFirst;
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

// Return, if card is used.
bool Game::returnIfUsed(int index) {
	if (index > (this->pairs * 2)-1) return true; // Out of scope. No idea what else to return in that case.
	return this->field[index].Used;
}

// Set, if card is used.
void Game::setUsed(int index, bool isUsed) {
	if (index > (this->pairs * 2)-1) return; // Out of scope.
	this->field[index].Used = isUsed;
}

// Return if both cards matches.
bool Game::checkIfMatches() {
	if (this->card1 < (this->pairs * 2) && this->card2 < (this->pairs * 2) && this->card1 != -1 && this->card2 != -1) {
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
			// Card matches, so set a win.
			switch(this->currentPlayer) {
				case Players::Player1:
					this->player1++;
					break;
				case Players::Player2:
					this->player2++;
					break;	
			}
			
			// Set that we used it.
			this->setUsed(this->card1, true);
			this->setUsed(this->card2, true);
			return true;
		} else {
			if (this->useAI) ai->setLastCards(this->card1, this->card2);
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

// Return, if the specific card is shown.
bool Game::returnIfShown(int index) {
	if (index > (this->pairs * 2)-1) return false; // Out of scope.
	return this->field[index].Shown;
}

// Set a card's status to shown.
void Game::setShown(int index, bool show) {
	if (index > (this->pairs * 2)-1) return; // Out of scope.
	this->field[index].Shown = show;
}

// Play if you can.
bool Game::play(int index) {
	if (this->returnIfUsed(index) != true) {
		if (this->cardSelect == CardSelectMode::DrawFirst) {
			this->setShown(index, true);
			this->setUsed(index, true);
			this->card1 = index;
			this->cardSelect = CardSelectMode::DrawSecond;
			return true;
		} else if (this->cardSelect == CardSelectMode::DrawSecond) {
			this->setShown(index, true);
			this->setUsed(index, true);
			this->card2 = index;
			this->cardSelect = CardSelectMode::DoCheck;
			return true;
		}
	} else {
		return false;
	}

	return false;
}

// Get a cardtype from an index.
int Game::getCard(int index) {
	if (index > (this->pairs * 2)-1) return -1; // Out of scope.
	return this->field[index].CardType;
}

// Get Current Player.
Players Game::getCurrentPlayer() { return this->currentPlayer; }

// Set Current Player.
void Game::setCurrentPlayer(Players player) { this->currentPlayer = player; }

// Get next player.
void Game::nextPlayer() {
	switch(this->currentPlayer) {
		case Players::Player1:
			this->currentPlayer = Players::Player2;
			break;
		case Players::Player2:
			this->currentPlayer = Players::Player1;
			break;
	}
}

// Check if all cards are used and return the winner.
GameWinner Game::checkOver() {
	// The Checkover is only valid, if the Player 1 & 2 cards matches the full size.
	if (this->player1 + this->player2 == (this->pairs)) {
		if (this->player1 > this->player2) {
			return GameWinner::Player1; // Player 1 wins!
		} else if (this->player2 > this->player1) {
			return GameWinner::Player2; // Player 2 wins!
		} else if (this->player1 == this->player2) {
			return GameWinner::None; // No one wins!
		}
	} else {
		return GameWinner::NotOver; // Nah, not all used.
	}

	return GameWinner::NotOver; // To not cause compile conflicts.
}

// Restart the game.
void Game::restart() {
	this->player1 = 0;
	this->player2 = 0;
	if (this->useAI) ai->clearCards();
	this->card1 = -1;
	this->card2 = -1;
	this->cardSelect = CardSelectMode::DrawFirst;
	this->currentPlayer = Players::Player1;
	this->generateCards(this->pairs);
}

// Get the amount of pairs from the players.
int Game::getPairs(Players player) {
	switch(player) {
		case Players::Player1:
			return this->player1;
		case Players::Player2:
			return this->player2;
	}

	return 0;
}

// Get and Set the current card selection.
CardSelectMode Game::getCardSelect() {
	return this->cardSelect;
}

// Set the CardSelect mode.
void Game::setCardSelect(CardSelectMode cardSelect) {
	this->cardSelect = cardSelect;
}

// Get amount of wins.
int Game::getWins(Players player) {
	if (player == Players::Player1) {
		return this->p1Wins;
	} else {
		return this->p2Wins;
	}
}

// Set amount of wins.
void Game::setWins(Players player, int wins) {
	if (player == Players::Player1) {
		this->p1Wins = wins;
	} else {
		this->p2Wins = wins;
	}
}

// Get amount of pairs.
int Game::getPairs() {
	return this->pairs;
}

// Set amount of pairs. Requires to restart the game.
void Game::setPairs(int pairs) {
	this->pairs = pairs; // Because we change it there, we also need to reload the field.
	this->restart();
}

// Do a random turn and return a random index from available cards.
int Game::doRandomTurn() {
	std::vector<int> availableIndexes;
	for (int i = 0; i < this->getPairs() * 2; i++) {
		// Return available positions.
		if (!this->returnIfShown(i)) {
			availableIndexes.push_back({i}); // Push back indexes.
		}
	}

	// Return a random index from the available indexes here.
	return availableIndexes[((randomGen()) % availableIndexes.size()) + 0];
}

int Game::doPredictLonger(int amountToRemember) {
	// Make sure we use the AI and our AI is not an nullptr.
	if (this->useAI || this->ai != nullptr) {
		// Check, if amountToRemember size is not larger than our AI size.
		if (amountToRemember > this->ai->getSize()-1) return this->doRandomTurn(); // Heh, nope. Size is too large.

		// If amountToRemember is not -1.
		if (amountToRemember != -1) {
			for (int i = 0; i < this->ai->getSize()-amountToRemember; i++) {
				// Check for the first card.
				if (this->getCard(this->ai->getFirst(i)) == this->getCard(this->card1)) {
					if (!this->returnIfUsed(this->ai->getFirst(i))) {
						return this->ai->getFirst(i);
					}
				}

				// Check for the second card.
				if (this->getCard(ai->getSecond(i)) == this->getCard(this->card1)) {
					if (!this->returnIfUsed(this->ai->getSecond(i))) {
						return this->ai->getSecond(i);
					}
				}
			}

			return this->doRandomTurn();
			// If we have -1.
		} else {
			for (int i = 0; i < ai->getSize(); i++) {
				// Check for the first card.
				if (this->getCard(ai->getFirst(i)) == this->getCard(this->card1)) {
					if (!this->returnIfUsed(ai->getFirst(i))) {
						return this->ai->getFirst(i);
					}
				}

				// Check for the second card.
				if (this->getCard(ai->getSecond(i)) == this->getCard(this->card1)) {
					if (!this->returnIfUsed(ai->getSecond(i))) {
						return this->ai->getSecond(i);
					}
				}
			}
		}

		return this->doRandomTurn();
	} else {
		return this->doRandomTurn(); // Because AI is unused / AI is nullptr.
	}
}

// Do some bit of prediction play from the last turn. Is probably not the best, so that's a TODO.
int Game::doPrediction() {
	// Make sure the AI is not an nullptr to avoid crashes.
	if (this->ai != nullptr) {
		// Check if the first last card matches our first card.
		if ((this->getCard(this->card1) == this->getCard(this->ai->getFirst())) && !this->returnIfShown(this->ai->getFirst())) {
			return this->ai->getFirst();
			// Check if the second last card matches our first card.
		} else if ((this->getCard(this->card1) == this->getCard(this->ai->getSecond())) && !this->returnIfShown(this->ai->getSecond())) {
			return this->ai->getSecond();
			// Well, no one matches, so do a random play.
		} else {
			return this->doRandomTurn();
		}
	} else {
		// Because the AI is an nullptr, we will do a random play.
		return this->doRandomTurn();
	}
}

// Our actual AI turn call.
int Game::doAITurn(bool predict, int amount) {
	if (this->cardSelect == CardSelectMode::DrawFirst) {
		return this->doRandomTurn();
	} else if (this->cardSelect == CardSelectMode::DrawSecond) {
		if (predict) {
			if (this->rememberMoreAI) {
				return this->doPredictLonger(amount);
			} else {
				return this->doPrediction();
			}
		// because we don't predict here, we'd do random.
		} else {
			return this->doRandomTurn();
		}
	} else {
		return this->doRandomTurn(); // No idea what else to return if cardSelect is 2.
	}
}

// Get the proper second card for the pair, cause why not. ;P
int Game::returnProperPair() {
	for (int i = 0; i < this->getPairs() * 2; i++) {
		if ((this->getCard(i) == this->getCard(this->card1)) && (!this->returnIfShown(i))) {
			return i;
		}
	}

	return 0; // Should never happen.
}