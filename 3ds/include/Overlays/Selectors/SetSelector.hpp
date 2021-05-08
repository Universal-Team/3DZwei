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

#ifndef _3DZWEI_SET_SELECTOR_HPP
#define _3DZWEI_SET_SELECTOR_HPP

#include "Pointer.hpp"
#include <string>
#include <vector>

class SetSelector {
public:
	SetSelector(const bool LoadCharacters = false);
	std::string Action() const;
private:
	mutable size_t Selected = 0, ScreenPos = 0;
	mutable std::string Res = "";
	mutable bool Done = false;
	bool LoadCharacters = false;
	mutable std::vector<std::string> Sets;

	void PrevPage();
	void NextPage();
	void SelectSet(const size_t Idx);
	void Cancel() const;

	const std::vector<FuncCallback> Positions = {
		{ 0, 25, 25, 190, [this]() { this->PrevPage(); } },

		/* Sets. */
		{ 50, 30, 300, 32, [this]() { this->SelectSet(0); } },
		{ 50, 67, 300, 32, [this]() { this->SelectSet(1); } },
		{ 50, 104, 300, 32, [this]() { this->SelectSet(2); } },
		{ 50, 141, 300, 32, [this]() { this->SelectSet(3); } },
		{ 50, 178, 300, 32, [this]() { this->SelectSet(4); } },

		{ 375, 25, 25, 190, [this]() { this->NextPage(); } }
	};

	const std::vector<FuncCallback> BottomPos = {
		{ 70, 25, 25, 190, [this]() { this->PrevPage(); } },

		/* Sets. */
		{ 95, 30, 130, 32, [this]() { this->SelectSet(0); } },
		{ 95, 67, 130, 32, [this]() { this->SelectSet(1); } },
		{ 95, 104, 130, 32, [this]() { this->SelectSet(2); } },
		{ 95, 141, 130, 32, [this]() { this->SelectSet(3); } },
		{ 95, 178, 130, 32, [this]() { this->SelectSet(4); } },

		{ 225, 25, 25, 190, [this]() { this->NextPage(); } },
		{ 0, 223, 17, 17, [this]() { this->Cancel(); } }
	};
};

#endif