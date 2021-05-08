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

#ifndef _3DZWEI_CARD_SELECTOR_HPP
#define _3DZWEI_CARD_SELECTOR_HPP

#include "Pointer.hpp"
#include <vector>

class CardSelector {
public:
	CardSelector();
	void Action();
private:
	std::vector<std::pair<bool, size_t>> Cards = { };
	bool Done = false, Changed = false;
	size_t Page = 0;

	void PrevPage();
	void NextPage();
	bool CanGoNext() const;
	void OK();
	void ToggleCard(const uint8_t Idx);

	const std::vector<FuncCallback> Positions = {
		{ 0, 25, 25, 215, [this]() { this->PrevPage(); } },
		{ 375, 25, 25, 215, [this]() { this->NextPage(); } },

		/* Card Images. */
		{ 60, 35, 55, 55, [this]() { this->ToggleCard(0); } },
		{ 160, 35, 55, 55, [this]() { this->ToggleCard(1); } },
		{ 260, 35, 55, 55, [this]() { this->ToggleCard(2); } },

		{ 60, 105, 55, 55, [this]() { this->ToggleCard(3); } },
		{ 160, 105, 55, 55, [this]() { this->ToggleCard(4); } },
		{ 260, 105, 55, 55, [this]() { this->ToggleCard(5); } },

		{ 60, 175, 55, 55, [this]() { this->ToggleCard(6); } },
		{ 160, 175, 55, 55, [this]() { this->ToggleCard(7); } },
		{ 260, 175, 55, 55, [this]() { this->ToggleCard(8); } },

		/* Checkboxes. */
		{ 120, 50, 24, 24, [this]() { this->ToggleCard(0); } },
		{ 220, 50, 24, 24, [this]() { this->ToggleCard(1); } },
		{ 320, 50, 24, 24, [this]() { this->ToggleCard(2); } },

		{ 120, 120, 24, 24, [this]() { this->ToggleCard(3); } },
		{ 220, 120, 24, 24, [this]() { this->ToggleCard(4); } },
		{ 320, 120, 24, 24, [this]() { this->ToggleCard(5); } },

		{ 120, 190, 24, 24, [this]() { this->ToggleCard(6); } },
		{ 220, 190, 24, 24, [this]() { this->ToggleCard(7); } },
		{ 320, 190, 24, 24, [this]() { this->ToggleCard(8); } }
	};

	const std::vector<FuncCallback> BottomPos = {
		{ 0, 15, 25, 215, [this]() { this->PrevPage(); } },
		{ 295, 15, 25, 215, [this]() { this->NextPage(); } },

		{ 88, 50, 24, 24, [this]() { this->ToggleCard(0); } },
		{ 148, 50, 24, 24, [this]() { this->ToggleCard(1); } },
		{ 208, 50, 24, 24, [this]() { this->ToggleCard(2); } },

		{ 88, 110, 24, 24, [this]() { this->ToggleCard(3); } },
		{ 148, 110, 24, 24, [this]() { this->ToggleCard(4); } },
		{ 208, 110, 24, 24, [this]() { this->ToggleCard(5); } },

		{ 88, 170, 24, 24, [this]() { this->ToggleCard(6); } },
		{ 148, 170, 24, 24, [this]() { this->ToggleCard(7); } },
		{ 208, 170, 24, 24, [this]() { this->ToggleCard(8); } },

		{ 105, 210, 110, 25, [this]() { this->OK(); } }
	};
};

#endif