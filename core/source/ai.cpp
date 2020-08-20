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

#include "ai.hpp"

/* AI Constructor. */
AI::AI(bool rememberLonger) {
	this->rememberLonger = rememberLonger;
	this->clearCards();
}

/* In case a game has ended, here we can reset the AI's mind. */
void AI::clearCards() {
	if (this->rememberLonger) {
		this->cards.clear();
	} else {
		this->lastCards = { -1, -1 };
	}
}

/* Here we gonna set the last cards to the AI's mind. */
void AI::setLastCards(int index1, int index2) {
	if (this->rememberLonger) {
		this->cards.push_back({index1, index2});
	} else {
		this->lastCards = { index1, index2 };
	}
}

/* Get the amount of cards from the AI's mind. */
int AI::getSize() { return (int)this->cards.size(); }

/* Get the first card. */
int AI::getFirst(int index) {
	if (this->rememberLonger) {
		if (index > this->getSize()-1)	return -1;
		return this->cards[index].first;
	} else {
		return this->lastCards.first;
	}
}

/* Get the second card. */
int AI::getSecond(int index) {
	if (this->rememberLonger) {
		if (index > this->getSize()-1)	return -1;
		return this->cards[index].second;
	} else {
		return this->lastCards.second;
	}
}