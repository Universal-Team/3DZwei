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

#ifndef _3DZWEI_MAIN_MENU_HPP
#define _3DZWEI_MAIN_MENU_HPP

#include "Pointer.hpp"
#include "screen.hpp"
#include <vector>

class MainMenu : public Screen {
public:
	MainMenu() { };
	void Draw(void) const override;
	void Logic(uint32_t Down, uint32_t Held, touchPosition T) override;
private:
	void GotoGame();
	void ShowRules();
	void GotoSettings();
	void ShowCredits();

	const std::vector<FuncCallback> Positions = {
		{ 90, 35, 140, 35, [this]() { this->GotoGame(); } }, // Game.
		{ 90, 80, 140, 35, [this]() { this->ShowRules(); } }, // Rules.
		{ 90, 125, 140, 35, [this]() { this->GotoSettings(); } }, // Settings.
		{ 90, 170, 140, 35, [this]() { this->ShowCredits(); } } // Credits.
	};

	const std::vector<std::string> ButtonNames = { "MAIN_MENU_GAME", "MAIN_MENU_RULES", "MAIN_MENU_SETTINGS", "MAIN_MENU_CREDITS" };
};

#endif