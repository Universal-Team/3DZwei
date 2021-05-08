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

#include "Config.hpp"
#include <unistd.h>

/* Loads the Configuration file. */
void Config::Load() {
	if (access("sdmc:/3ds/3DZwei/Config.json", F_OK) != 0) this->Initialize();

	FILE *File = fopen("sdmc:/3ds/3DZwei/Config.json", "r");
	this->CFG = nlohmann::json::parse(File, nullptr, false);
	fclose(File);

	if (!this->CFG.is_discarded()) {
		this->CardSet(this->Get<std::string>("Cardset", this->CardSet()));
		this->CharSet(this->Get<std::string>("Charset", this->CharSet()));
		this->Lang(this->Get<std::string>("Lang", this->Lang()));
		this->PointerSpeed(this->Get<nlohmann::json::number_integer_t>("PointerSpeed", this->PointerSpeed()));
		this->ShowSplash(this->Get<bool>("ShowSplash", this->ShowSplash()));
	}
};

/* Initializes the Configuration file properly as a JSON. */
void Config::Initialize() {
	FILE *Temp = fopen("sdmc:/3ds/3DZwei/Config.json", "w");

	const nlohmann::json OBJ = {
		{ "Cardset", this->CardSet() }, // Cardset.
		{ "Charset", this->CharSet() }, // Character Set.
		{ "Lang", this->Lang() }, // Language.
		{ "PointerSpeed", this->PointerSpeed() }, // Pointer Speed.
		{ "ShowSplash", this->ShowSplash() }, // Show the Startup Splash.
		{ "ActivatedCards", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } } // Activated Indexes.
	};

	const std::string Dump = OBJ.dump(1, '\t');
	fwrite(Dump.c_str(), 1, Dump.size(), Temp);
	fclose(Temp);
};

/* SAV changes to the Configuration, if changes made. */
void Config::Sav() {
	if (this->ChangesMade) {
		this->Set<std::string>("Cardset", this->CardSet()); // Cardset.
		this->Set<std::string>("Charset", this->CharSet()); // Cardset.
		this->Set<std::string>("Lang", this->Lang()); // Language.
		this->Set<nlohmann::json::number_integer_t>("PointerSpeed", this->PointerSpeed()); // Pointer Speed
		this->Set<bool>("ShowSplash", this->ShowSplash()); // Startup Splash.

		FILE *Out = fopen("sdmc:/3ds/3DZwei/Config.json", "w");

		/* Write changes to file. */
		const std::string Dump = this->CFG.dump(1, '\t');
		fwrite(Dump.c_str(), 1, Dump.size(), Out);
		fclose(Out);
	}
};

/*
	Returns if the provided index is activated.

	const size_t Idx: The index to check.
*/
bool Config::CardIndexIncluded(const size_t Idx) {
	if (!this->CFG.is_discarded() && this->CFG.contains("ActivatedCards") && this->CFG["ActivatedCards"].is_array()) {
		for (size_t Idx2 = 0; Idx2 < this->CFG["ActivatedCards"].size(); Idx2++) {
			if (this->CFG["ActivatedCards"][Idx2] == Idx) return true;
		}
	}

	return false;
};

/*
	Set the activated cards to the config.

	const std::vector<size_t> &Cards: The cards which are activated.
*/
void Config::ActivatedCards(const std::vector<size_t> &Cards) {
	if (!this->CFG.is_discarded()) this->CFG["ActivatedCards"] = Cards;
};