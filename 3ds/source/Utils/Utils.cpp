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

#include "Common.hpp"
#include "Utils.hpp"
#include <unistd.h>

std::vector<size_t> Utils::Cards;


/*
	Initialize the used Cards at app-start.
	Can also be used to reset the used cards.
*/
void Utils::InitCards(const bool Init) {
	Utils::Cards.clear();
	for (size_t Idx = 0; Idx < Utils::GetCardSheetSize(); Idx++) Utils::Cards.push_back(Idx);

	if (Init) {
		if (Utils::Cards.empty()) return;

		/* Check for activated cards here and set them into Utils::Cards. */
		for (size_t Index = Utils::Cards.size() - 1; Index > 0; Index--) { // Going backwards due to erases.
			if (!_3DZwei::CFG->CardIndexIncluded(Utils::Cards[Index])) {
				Utils::Cards.erase(Utils::Cards.begin() + Index); // Not included, so erase!
			}
		}
	}
}


/*
	ONLY enable the first 10 pairs when initing a new Card Spritesheet, so it'd fill a full page as max.
*/
void Utils::InitNewCardSheet() {
	Utils::Cards.clear();

	if (Utils::GetCardSheetSize() >= 10) { // 10+ -> Only enable 10 pairs.
		for (size_t Idx = 0; Idx < 10; Idx++) Utils::Cards.push_back(Idx);

	} else { // 9- -> enable all pairs.
		for (size_t Idx = 0; Idx < Utils::GetCardSheetSize(); Idx++) Utils::Cards.push_back(Idx);
	}
}


/*
	Returns the amount of cards from the active Cards Spritesheet.
	This excludes the back cover card which should be the last one.
*/
size_t Utils::GetCardSheetSize() {
	if (GFX::Cards) {
		/* Check if the amount of cards are larger than 0. */
		if (C2D_SpriteSheetCount(GFX::Cards) > 0) return C2D_SpriteSheetCount(GFX::Cards) - 1;
	}

	return 0;
}


/*
	Returns the amount of characters from the active Characters Spritesheet.
*/
size_t Utils::GetCharSheetSize() {
	if (GFX::Characters) return C2D_SpriteSheetCount(GFX::Characters);

	return 0;
}


/*
	Check, if a set is good.

	const std::string &Set: The set to check.
	const bool CheckChars: If the characters should be checked (true) or cards (false).
*/
bool Utils::CheckSetContent(const std::string &Set, const bool CheckChars) {
	/* Ensure Set is not "" or something with '/' inside it, that causes access() to break. */
	if (Set != "") {
		for (size_t Idx = 0; Idx < Set.size(); Idx++) {
			if (Set[Idx] == '/') return false; // Contains a '/', so not valid!
		}

		if (Set.size() > 4) {
			/* Ensure it is '.t3x'. */
			if (Set.substr(Set.size() - 4) == ".t3x") {
				if (CheckChars) {
					if (Set != "3DZwei-RomFS" && (access((std::string("sdmc:/3ds/ut-games/sets/characters/") + Set).c_str(), F_OK) == 0)) return true;

				} else {
					if (Set != "3DZwei-RomFS" && (access((std::string("sdmc:/3ds/ut-games/sets/3DZwei/") + Set).c_str(), F_OK) == 0)) return true;
				}
			}
		}
	}

	return ((!Set.empty()) && (Set == "3DZwei-RomFS"));
}


/*
	Load a cardset.

	const std::string &Set: The cardset to load.
*/
void Utils::LoadCardSet(const std::string &Set) {
	/* Load the included Set. */
	if (Set == "3DZwei-RomFS") {
		if (GFX::Cards) C2D_SpriteSheetFree(GFX::Cards); // Unload first.
		GFX::Cards = C2D_SpriteSheetLoad("romfs:/gfx/cards.t3x"); // Reload now.

	} else {
		if (Utils::CheckSetContent(Set, false)) { // Check for cards.
			if (GFX::Cards) C2D_SpriteSheetFree(GFX::Cards); // Unload first.
			GFX::Cards = C2D_SpriteSheetLoad((std::string("sdmc:/3ds/ut-games/sets/3DZwei/") + Set).c_str()); // Reload now.

		} else {
			if (GFX::Cards) C2D_SpriteSheetFree(GFX::Cards); // Unload first.
			GFX::Cards = C2D_SpriteSheetLoad("romfs:/gfx/cards.t3x"); // Reload now.
			_3DZwei::CFG->CardSet("3DZwei-RomFS"); // Set 3DZwei-RomFS else to Cardset.
		}
	}
}


/*
	Load a character set.

	const std::string &Set: The Character set to load.
*/
void Utils::LoadCharSet(const std::string &Set) {
	/* Load the included Set. */
	if (Set == "3DZwei-RomFS") {
		if (GFX::Characters) C2D_SpriteSheetFree(GFX::Characters); // Unload first.
		GFX::Characters = C2D_SpriteSheetLoad("romfs:/gfx/chars.t3x"); // Reload now.

	} else {
		if (Utils::CheckSetContent(Set, true)) { // Check for Chars.
			if (GFX::Characters) C2D_SpriteSheetFree(GFX::Characters); // Unload first.
			GFX::Characters = C2D_SpriteSheetLoad((std::string("sdmc:/3ds/ut-games/sets/characters/") + Set).c_str()); // Reload now.

		} else {
			if (GFX::Characters) C2D_SpriteSheetFree(GFX::Characters); // Unload first.
			GFX::Characters = C2D_SpriteSheetLoad("romfs:/gfx/chars.t3x"); // Reload now.
			_3DZwei::CFG->CharSet("3DZwei-RomFS"); // Set 3DZwei-RomFS else to Charset.
		}
	}
}