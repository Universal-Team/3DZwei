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

#ifndef _3DZWEI_GAME_OVERLAY_HPP
#define _3DZWEI_GAME_OVERLAY_HPP

#include "GameHelper.hpp"
#include "Pointer.hpp"
#include <vector>

class GameOverlay {
public:
	GameOverlay(const GameSettings::GameParams Params = { });
	void Action();
private:
	std::unique_ptr<GameHelper> Helper = nullptr;
	bool Running = true, SwipeIn = false, SwipeOut = false, PromptHandle = false, PromptAnswer = false;
	int16_t SwipePos = 320, FAlpha = 0;
	float Cubic = 0.0f;

	void Draw();
	void PromptLogic();
	void Confirm();
	void Cancel();

	const std::vector<FuncCallback> Prompt = {
		{ 24, 140, 124, 48, [this]() { this->Cancel(); } },
		{ 172, 140, 124, 48, [this]() { this->Confirm(); } }
	};
};

#endif