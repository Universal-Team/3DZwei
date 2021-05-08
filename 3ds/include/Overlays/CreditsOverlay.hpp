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

#ifndef _3DZWEI_CREDITS_OVERLAY_HPP
#define _3DZWEI_CREDITS_OVERLAY_HPP

#include "Pointer.hpp"
#include <vector>

class CreditsOverlay {
public:
	CreditsOverlay() { };
	void Action();
private:
	enum class Pages : uint8_t { Contributors = 0, SMemCore = 1, UnivCore = 2, Translators = 3 };
	Pages Page = Pages::Contributors;
	bool Done = false;

	void PrevPage();
	void OK();
	void NextPage();


	const std::vector<FuncCallback> Positions = {
		{ 40, 60, 25, 145, [this]() { this->PrevPage(); } }, // Prev.
		{ 335, 60, 25, 145, [this]() { this->NextPage(); } } // Next.
	};

	/* Same as above, but on the bottom for touch compatibility! */
	const std::vector<FuncCallback> BottomPos = {
		{ 70, 75, 25, 90, [this]() { this->PrevPage(); } },
		{ 123, 109, 74, 22, [this]() { this->OK(); } },
		{ 225, 75, 25, 90, [this]() { this->NextPage(); } }
	};
};

#endif