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

#ifndef _3DZWEI_LANGUAGE_SELECTOR_HPP
#define _3DZWEI_LANGUAGE_SELECTOR_HPP

#include "Pointer.hpp"
#include <vector>

class LanguageSelector {
public:
	LanguageSelector() { };
	void Action();
private:
	bool Done = false, FullDone = false, Start = true;
	int16_t Delay = 255;
	float Cubic = 0.0f;

	void SelectLang(const uint8_t Idx);
	void Cancel();

	const std::vector<FuncCallback> Positions = {
		{ 120, 25, 24, 24, [this]() { this->SelectLang(0); } }, // Bruh.
		{ 120, 65, 24, 24, [this]() { this->SelectLang(1); } }, // Deutsch.
		{ 120, 105, 24, 24, [this]() { this->SelectLang(2); } }, // English.
		{ 120, 145, 24, 24, [this]() { this->SelectLang(3); } }, // Español.
		{ 120, 185, 24, 24, [this]() { this->SelectLang(4); } }, // Français.
		{ 240, 25, 24, 24, [this]() { this->SelectLang(5); } }, // Italiano.
		{ 240, 65, 24, 24, [this]() { this->SelectLang(6); } }, // 日本語.
		{ 240, 105, 24, 24, [this]() { this->SelectLang(7); } }, // Polski.
		{ 240, 145, 24, 24, [this]() { this->SelectLang(8); } }, // Português.
		{ 240, 185, 24, 24, [this]() { this->SelectLang(9); } }, // Русский.

		{ 0, 223, 17, 17, [this]() { this->Cancel(); } }
	};
};
#endif