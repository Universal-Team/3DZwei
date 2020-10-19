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

#ifndef _3DZWEI_OVERLAY_HPP
#define _3DZWEI_OVERLAY_HPP

#include "common.hpp"
#include "game.hpp"
#include <citro2d.h>

namespace Overlays {
	/* The RGB Overlay. */
	u32 SelectRGB(u32 oldColor);

	/* Display results of the game. */
	bool ResultOverlay(std::unique_ptr<Game> &game, int neededWins = 3, int avatar1 = 0, int avatar2 = 1);

	/* Select an avatar. */
	int SelectAvatar(int player = 0);

	/* Select a cardset. */
	std::string SelectCardSet();

	/* Preview the cardset. */
	void PreviewCards(C2D_SpriteSheet &sheet, C2D_SpriteSheet &BG, std::string folder);

	/* Select a Language. */
	void SelectLanguage();

	/* Display the Splash. */
	void SplashOverlay();

	/* Show the rules. */
	void ShowRules();

	/* Select a game-mode. */
	int SelectGame();

	/* Show the credits. */
	void showCredits();
};

#endif