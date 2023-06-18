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

#ifndef _3DZWEI_3DZWEI_HPP
#define _3DZWEI_3DZWEI_HPP

#include "Config.hpp"
#include "Pointer.hpp"
#include <memory>
#include <string>

class _3DZwei {
public:
	_3DZwei();
	int Action();

	static std::unique_ptr<Config> CFG; // Need to do this static for the other overlays to access.
private:
	bool Exiting = false, FullExit = false, FadeIn = true, FadeOut = false;
	uint8_t FAlpha = 0;

	void PrepareGame();
	void ShowRules();
	void AccessSettings();
	void ShowCredits();

	void OverlayReturn();
	void FadeOutHandler();
	void FadeInHandler();
	void Draw();

	const std::vector<FuncCallback> Positions = {
		{ 90, 35, 140, 35, [this]() { this->PrepareGame(); } }, // Game.
		{ 90, 80, 140, 35, [this]() { this->ShowRules(); } }, // Rules.
		{ 90, 125, 140, 35, [this]() { this->AccessSettings(); } }, // Settings.
		{ 90, 170, 140, 35, [this]() { this->ShowCredits(); } } // Credits.
	};

	const std::vector<std::string> ButtonNames = { "MAIN_MENU_GAME", "MAIN_MENU_RULES", "MAIN_MENU_SETTINGS", "MAIN_MENU_CREDITS" };
};

#endif