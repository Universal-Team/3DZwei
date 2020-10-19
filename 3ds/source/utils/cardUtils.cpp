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

#include "cardUtils.hpp"
#include "coreHelper.hpp"

extern C2D_SpriteSheet cards; // Include it here, cause why not.
extern std::vector<int> cardIndex;

/*
	Randomize CardIndex.
*/
void CardUtils::randomizeCards() {
	cardIndex.clear(); // Clear first.

	for (int i = 0; i < (int)(C2D_SpriteSheetCount(cards)) - 1; i++) {
		cardIndex.push_back({i});
	}

	std::shuffle(cardIndex.begin(), cardIndex.end(), randomGen);
}

/*
	Fill CardIndex normally.
*/
void CardUtils::fillIndex() {
	cardIndex.clear(); // Clear first.

	for (int i = 0; i < (int)(C2D_SpriteSheetCount(cards)) - 1; i++) {
		cardIndex.push_back({i});
	}
}