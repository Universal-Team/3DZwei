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

#ifndef _3DZWEI_CONFIG_HPP
#define _3DZWEI_CONFIG_HPP

#include "JSON.hpp"

class Config {
public:
	Config() { this->Load(); };
	void Load();
	void Initialize();
	void Sav();

	/* The Card Set. */
	std::string CardSet() const { return this->VCardSet; };
	void CardSet(const std::string &V) { this->VCardSet = V; if (!this->ChangesMade) this->ChangesMade = true; };

	/* The Character Set. */
	std::string CharSet() const { return this->VCharSet; };
	void CharSet(const std::string &V) { this->VCharSet = V; if (!this->ChangesMade) this->ChangesMade = true; };

	/* The current Language. */
	std::string Lang() const { return this->VLang; };
	void Lang(const std::string &V) { this->VLang = V; if (!this->ChangesMade) this->ChangesMade = true; };

	/* Show the Splash at startup. */
	bool ShowSplash() const { return this->VShowSplash; };
	void ShowSplash(const bool V) { this->VShowSplash = V; if (!this->ChangesMade) this->ChangesMade = true; };

	/* Animation enabled State. */
	bool DoAnimation() const { return this->VAnimation; };
	void DoAnimation(const bool V) { this->VAnimation = V; if (!this->ChangesMade) this->ChangesMade = true; };

	/* The Activated Cards. */
	bool CardIndexIncluded(const size_t Idx);
	void ActivatedCards(const std::vector<size_t> &Cards);

	/* The Pointer Speed. */
	uint8_t PointerSpeed() const { return this->VPointerSpeed; };
	void PointerSpeed(const uint8_t V) { this->VPointerSpeed = V; if (!this->ChangesMade) this->ChangesMade = true; };

	/* The Game Animation. */
	uint8_t GameAnimation() const { return this->VGameAnimation; };
	void GameAnimation(const uint8_t V) { this->VGameAnimation = V; if (!this->ChangesMade) this->ChangesMade = true; };

	/* If doing the page switches visually or not. */
	bool PageSwitch() const { return this->VPageSwitch; };
	void PageSwitch(const bool V) { this->VPageSwitch = V; if (!this->ChangesMade) this->ChangesMade = true; };

	/* If using the fade effect or nah. */
	bool DoFade() const { return this->VDoFade; };
	void DoFade(const bool V) { this->VDoFade = V; if (!this->ChangesMade) this->ChangesMade = true; };
private:
	template <typename T>
	T Get(const std::string &Key, const T IfNotFound) {
		if (this->CFG.is_discarded() || !this->CFG.contains(Key)) return IfNotFound;

		return this->CFG.at(Key).get_ref<const T &>();
	};

	template <typename T>
	void Set(const std::string &Key, const T Data) {
		if (!this->CFG.is_discarded()) this->CFG[Key] = Data;
	};

	bool ChangesMade = false, VShowSplash = true, VAnimation = true, VPageSwitch = true, VDoFade = true;
	nlohmann::json CFG = nullptr;
	uint8_t VPointerSpeed = 4, VGameAnimation = 1;
	std::string VCardSet = "3DZwei-RomFS", VCharSet = "3DZwei-RomFS", VLang = "en";
};

#endif