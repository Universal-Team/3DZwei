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

#ifndef _3DZWEI_UTILS_HPP
#define _3DZWEI_UTILS_HPP

#include <vector>

namespace Utils {
	extern std::vector<size_t> Cards;

	void InitCards(const bool Init = false);
	void InitNewCardSheet();
	size_t GetCardSheetSize();
	size_t GetCharSheetSize();

	bool CheckSetContent(const std::string &Set, const bool CheckChars = false);
	void LoadCardSet(const std::string &Set);
	void LoadCharSet(const std::string &Set);
}

#endif