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

#ifndef _3DZWEI_SETTINGS_OVERLAY_HPP
#define _3DZWEI_SETTINGS_OVERLAY_HPP

#include "Pointer.hpp"
#include <vector>

class SettingsOverlay {
public:
	SettingsOverlay() { };
	void Action();
private:
	enum class SettingsTab : uint8_t { Configuration, Animation, AppInfo };

	SettingsTab Tab = SettingsTab::Configuration;
	const std::vector<std::string> TabNames = { "SETTINGS_TAB_CONFIG", "SETTINGS_ANIMATION_TAB", "SETTINGS_TAB_APPINFO" };
	bool Done = false, SwipeDir = false, DoSwipe = false, InitialSwipe = true, FadeIn = true, FadeOut = false;
	int CurTabOffs[3] = { -320, 320, 640 }, PrevTabOffs[3] = { -320, 320, 640 }, FAlpha = 255;
	float Swipe = 0.0f, ToSwipe = 0.0f;

	std::string GetLanguage() const;
	void DrawTop() const;
	void DrawTabs() const;

	void ConfigTab();
	void AnimationTab();
	void AppinfoTab();
	void Back();

	/* Configurations Operations. */
	void SelectLang();
	void SelectCardSet();
	void SelectCharSet();
	void ToggleSplash();
	void ShowSplash();
	void SetPointerSpeed();
	void SetGameDefaults();

	void ToggleAnimation();
	void SwitchGameAnim();
	void TogglePageSwitch();
	void ToggleFade();

	void FadeOutHandler();
	void FadeInHandler();
	void TabHandler();

	const std::vector<FuncCallback> AnimPos = {
		{ 1, 0, 106, 20, [this]() { this->ConfigTab(); } },
		{ 107, 0, 106, 20, [this]() { this->AnimationTab(); } },
		{ 213, 0, 106, 20, [this]() { this->AppinfoTab(); } },

		{ 150, 30, 24, 24, [this]() { this->ToggleAnimation(); } },
		{ 150, 65, 24, 24, [this]() { this->SwitchGameAnim(); } },
		{ 150, 100, 24, 24, [this]() { this->TogglePageSwitch(); } },
		{ 150, 135, 24, 24, [this]() { this->ToggleFade(); } },

		{ 0, 223, 17, 17, [this]() { this->Back(); } }
	};

	const std::vector<FuncCallback> Positions = {
		{ 1, 0, 106, 20, [this]() { this->ConfigTab(); } },
		{ 107, 0, 106, 20, [this]() { this->AnimationTab(); } },
		{ 213, 0, 106, 20, [this]() { this->AppinfoTab(); } },

		{ 150, 30, 24, 24, [this]() { this->SelectLang(); } },
		{ 150, 65, 24, 24, [this]() { this->SelectCardSet(); } },
		{ 150, 100, 24, 24, [this]() { this->SelectCharSet(); } },
		{ 150, 135, 24, 24, [this]() { this->ToggleSplash(); } },
		{ 200, 135, 24, 24, [this]() { this->ShowSplash(); } },
		{ 150, 170, 24, 24, [this]() { this->SetPointerSpeed(); } },
		{ 150, 205, 24, 24, [this]() { this->SetGameDefaults(); } },

		{ 0, 223, 17, 17, [this]() { this->Back(); } }
	};
};

#endif