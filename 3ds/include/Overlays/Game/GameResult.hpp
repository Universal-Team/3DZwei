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

#ifndef _3DZWEI_GAME_RESULT_HPP
#define _3DZWEI_GAME_RESULT_HPP

#include "GameSettings.hpp"
#include "Pointer.hpp"
#include <vector>

class GameResult {
public:
	GameResult();
	void Action(GameSettings::GameParams &Params, const uint8_t Won);
private:
	void DrawCardBG(const size_t Page, const int AddOffs);
	void Handler();

	/* Draw Modes. */
	void DrawOver(const bool P2Wins, const GameSettings::GameParams &Params);
	void DrawVersus(const bool P2Wins, const GameSettings::GameParams &Params, const bool Tie);
	void DrawSolo(const GameSettings::GameParams &Params);

	bool Done = false, FullDone = false, Over = false, DoSwipe = true, DoScrollSwipe = false, InitialScroll = true, ScrollMode = false;
	int16_t Delay = 255, ScrollIdx = -240, ScrollDelay = 60;
	size_t ScrollPage = 0;
	float Cubic = 0.0f, ScrollCubic = 0.0f;

	const std::vector<FuncCallback> InitialScrollPos = {
		{ 36, 37, 55, 55 },
		{ 127, 37, 55, 55 },
		{ 218, 37, 55, 55 },
		{ 309, 37, 55, 55 },

		{ 36, 104, 55, 55 },
		{ 127, 104, 55, 55 },
		{ 218, 104, 55, 55 },
		{ 309, 104, 55, 55 },

		{ 36, 171, 55, 55 },
		{ 127, 171, 55, 55 },
		{ 218, 171, 55, 55 },
		{ 309, 171, 55, 55 }
	};
};

#endif