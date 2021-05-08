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

#ifndef _3DZWEI_SETTINGS_HPP
#define _3DZWEI_SETTINGS_HPP

#include "Pointer.hpp"
#include "screen.hpp"
#include <vector>

class Settings : public Screen {
public:
	Settings() { Pointer::SetPos(this->Positions[0]); };
	void Draw(void) const override;
	void Logic(uint32_t Down, uint32_t Held, touchPosition T) override;
private:
	enum class SettingsTab : uint8_t { Configuration, AppInfo };

	SettingsTab Tab = SettingsTab::Configuration;
	const std::vector<std::string> TabNames = { "SETTINGS_TAB_CONFIG", "SETTINGS_TAB_APPINFO" };

	void DrawTop() const;
	void DrawTabs() const;

	void ConfigTab();
	void AppinfoTab();
	void Back();

	/* Configurations Operations. */
	void SelectLang();
	void SelectCardSet();
	void SelectCharSet();
	void ToggleSplash();
	void ShowSplash();
	void SetPointerSpeed();

	const std::vector<FuncCallback> Positions = {
		{ 0, 0, 160, 20, [this]() { this->ConfigTab(); } },
		{ 160, 0, 160, 20, [this]() { this->AppinfoTab(); } },

		{ 150, 35, 24, 24, [this]() { this->SelectLang(); } },
		{ 150, 75, 24, 24, [this]() { this->SelectCardSet(); } },
		{ 150, 115, 24, 24, [this]() { this->SelectCharSet(); } },
		{ 150, 155, 24, 24, [this]() { this->ToggleSplash(); } },
		{ 200, 155, 24, 24, [this]() { this->ShowSplash(); } },
		{ 150, 195, 24, 24, [this]() { this->SetPointerSpeed(); } },

		{ 0, 223, 17, 17, [this]() { this->Back(); } }
	};
};

#endif