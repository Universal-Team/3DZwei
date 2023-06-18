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

#include "Config.hpp"
#include "Utils.hpp" // Required for 'Utils::GetCharSheetSize()'.
#include <unistd.h>


/*
	Detects system language and is used later to set app language to system language.
*/
std::string Config::sysLang(void) {
	uint8_t Language = 1;
	CFGU_GetSystemLanguage(&Language);

	switch(Language) {
		case 0:
			return "ja"; // Japanese

		case 1:
		default:
			return "en"; // English

		case 2:
			return "fr"; // French

		case 3:
			return "de"; // German

		case 4:
			return "it"; // Italian

		case 5:
			return "es"; // Spanish

		case 6:
			return "zh-CN"; // Chinese (Simplified)

		case 7:
			return "ko"; // Korean

		case 8:
			return "nl"; // Dutch

		case 9:
			return "pt"; // Portuguese

		case 10:
			return "ru"; // Russian

		case 11:
			return "zh-TW"; // Chinese (Traditional)
	}
}

/* Loads the Configuration file. */
void Config::Load() {
	if (access("sdmc:/3ds/ut-games/3DZwei/Config.json", F_OK) != 0) this->Initialize();

	FILE *File = fopen("sdmc:/3ds/ut-games/3DZwei/Config.json", "r");

	if (File) {
		this->CFG = nlohmann::json::parse(File, nullptr, false);
		fclose(File);
	}

	if (!this->CFG.is_discarded()) {
		this->CardSet(this->Get<std::string>("Cardset", this->CardSet()));
		this->CharSet(this->Get<std::string>("Charset", this->CharSet()));
		this->DoAnimation(this->Get<bool>("DoAnimation", this->DoAnimation()));
		this->DoFade(this->Get<bool>("DoFade", this->DoFade()));
		this->GameAnimation(this->Get<nlohmann::json::number_integer_t>("GameAnimation", this->GameAnimation()));
		this->Lang(this->Get<std::string>("Lang", this->Lang()));
		this->PageSwitch(this->Get<bool>("PageSwitch", this->PageSwitch()));
		this->PointerSpeed(this->Get<nlohmann::json::number_integer_t>("PointerSpeed", this->PointerSpeed()));
		this->ShowSplash(this->Get<bool>("ShowSplash", this->ShowSplash()));
	}
}


/* Initializes the Configuration file properly as a JSON. */
void Config::Initialize() {
	const nlohmann::json OBJ = {
		{ "Cardset", this->CardSet() }, // Cardset.
		{ "Charset", this->CharSet() }, // Character Set.
		{ "DoAnimation", this->DoAnimation() }, // Animation.
		{ "DoFade", this->DoFade() }, // If fade effects are activated.
		{ "GameAnimation", 1 }, // Game Animation Type.
		{ "PageSwitch", true }, // Page Switch.
		{ "Lang", sysLang() }, // Language.
		{ "PointerSpeed", this->PointerSpeed() }, // Pointer Speed.
		{ "ShowSplash", this->ShowSplash() }, // Show the Startup Splash.
		{ "ActivatedCards", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } } // Activated Indexes.
	};

	FILE *Temp = fopen("sdmc:/3ds/ut-games/3DZwei/Config.json", "w");

	if (Temp) {
		const std::string Dump = OBJ.dump(1, '\t');
		fwrite(Dump.c_str(), 1, Dump.size(), Temp);
		fclose(Temp);
	}
}


/* SAV changes to the Configuration, if changes made. */
void Config::Sav() {
	if (this->ChangesMade) {
		this->Set<std::string>("Cardset", this->CardSet()); // Cardset.
		this->Set<std::string>("Charset", this->CharSet()); // Cardset.
		this->Set<bool>("DoAnimation", this->DoAnimation()); // Animation.
		this->Set<bool>("DoFade", this->DoFade()); // If fade effects are activated.
		this->Set<nlohmann::json::number_integer_t>("GameAnimation", this->GameAnimation()); // Game Animation.
		this->Set<std::string>("Lang", this->Lang()); // Language.
		this->Set<bool>("PageSwitch", this->PageSwitch()); // PageSwitch.
		this->Set<nlohmann::json::number_integer_t>("PointerSpeed", this->PointerSpeed()); // Pointer Speed.
		this->Set<bool>("ShowSplash", this->ShowSplash()); // Startup Splash.

		FILE *Out = fopen("sdmc:/3ds/ut-games/3DZwei/Config.json", "w");

		if (Out) {
			/* Write changes to file. */
			const std::string Dump = this->CFG.dump(1, '\t');
			fwrite(Dump.c_str(), 1, Dump.size(), Out);
			fclose(Out);
		}
	}
}


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
}


/*
	Set the activated cards to the config.

	const std::vector<size_t> &Cards: The cards which are activated.
*/
void Config::ActivatedCards(const std::vector<size_t> &Cards) {
	if (!this->CFG.is_discarded()) this->CFG["ActivatedCards"] = Cards;
}


/*
	Fetch the content from the config for the default Game Settings.
*/
void Config::FetchDefaults() {
	if (!this->CFG.is_discarded()) {
		/* Ensure it is an object and exist. */
		if (this->CFG.contains("GameDefaults") && this->CFG["GameDefaults"].is_object()) {
			/* Here we begin the fetch! Fetch Game Mode. */
			if (this->CFG["GameDefaults"].contains("GameMode") && this->CFG["GameDefaults"]["GameMode"].is_number()) {
				const int Res = this->CFG["GameDefaults"]["GameMode"];
				if (Res >= 0 && Res < 2) this->VDefaultParams.GameMode = (GameSettings::GameModes)Res; // 0 - 1 valid.
			}

			/* Card Delay. */
			if (this->CFG["GameDefaults"].contains("CardDelay") && this->CFG["GameDefaults"]["CardDelay"].is_number()) {
				const int Res = this->CFG["GameDefaults"]["CardDelay"];
				if (Res > 0 && Res < 256) { // Exist, between 1 and 255 -> Good.
					this->VDefaultParams.CardDelay = Res;
					this->VDefaultParams.CardDelayUsed = true;
				}
			}

			/* AI Mode. */
			if (this->CFG["GameDefaults"].contains("AIMode") && this->CFG["GameDefaults"]["AIMode"].is_number()) {
				const int Res = this->CFG["GameDefaults"]["AIMode"];
				if (Res >= 0 && Res < 3) { // Exist, between 0 - 2 (Random, Hard, Default) == good.
					this->VDefaultParams.Method = (StackMem::AIMethod)Res;
					this->VDefaultParams.AIUsed = true;
				}
			}

			/* Rounds to win the game. */
			if (this->CFG["GameDefaults"].contains("RoundsToWin") && this->CFG["GameDefaults"]["RoundsToWin"].is_number()) {
				const int Res = this->CFG["GameDefaults"]["RoundsToWin"];
				if (Res > 0 && Res < 256) this->VDefaultParams.RoundsToWin = Res; // Exist, 1 and 255 -> Good.
			}

			/* First character index. */
			if (this->CFG["GameDefaults"].contains("Player1Idx") && this->CFG["GameDefaults"]["Player1Idx"].is_number()) {
				const int Res = this->CFG["GameDefaults"]["Player1Idx"];
				if (Res >= 0 && Res < (int)Utils::GetCharSheetSize()) this->VDefaultParams.Characters[0] = Res; // Smaller than the max amount -> Good.
			}

			/* Second character index. */
			if (this->CFG["GameDefaults"].contains("Player2Idx") && this->CFG["GameDefaults"]["Player2Idx"].is_number()) {
				const int Res = this->CFG["GameDefaults"]["Player2Idx"];
				if (Res >= 0 && Res < (int)Utils::GetCharSheetSize()) this->VDefaultParams.Characters[1] = Res; // Smaller than the max amount -> Good.
			}

			/* First character name. */
			if (this->CFG["GameDefaults"].contains("Player1Name") && this->CFG["GameDefaults"]["Player1Name"].is_string()) {
				const std::string Res = this->CFG["GameDefaults"]["Player1Name"];

				if (Res.size() < 17) this->VDefaultParams.Names[0] = Res; // Smaller than 17 characters -> Good.
			}

			/* Second character name. */
			if (this->CFG["GameDefaults"].contains("Player2Name") && this->CFG["GameDefaults"]["Player2Name"].is_string()) {
				const std::string Res = this->CFG["GameDefaults"]["Player2Name"];

				if (Res.size() < 17) this->VDefaultParams.Names[1] = Res; // Smaller than 17 characters -> Good.
			}

			/* Round Starter. */
			if (this->CFG["GameDefaults"].contains("RoundStarter") && this->CFG["GameDefaults"]["RoundStarter"].is_number()) {
				const int Res = this->CFG["GameDefaults"]["RoundStarter"];
				if (Res >= 0 && Res < 5) this->VDefaultParams.Starter = (GameSettings::RoundStarter)Res; // Smaller than the max amount -> Good.
			}
		}
	}
}


/*
	Set Default Game Config.

	const GameSettings::GameParams &Defaults: The config to set to default.
*/
void Config::SetDefault(const GameSettings::GameParams &Defaults) {
	if (!this->CFG.is_discarded()) {
		this->CFG["GameDefaults"]["GameMode"] = (uint8_t)Defaults.GameMode; // Game Mode.

		/* Card Delay. */
		if (Defaults.CardDelayUsed && Defaults.CardDelay > 0) this->CFG["GameDefaults"]["CardDelay"] = Defaults.CardDelay;
		else {
			if (this->CFG.contains("GameDefaults") && this->CFG["GameDefaults"].is_object()) {
				if (this->CFG["GameDefaults"].contains("CardDelay") && this->CFG["GameDefaults"]["CardDelay"].is_number()) {
					this->CFG["GameDefaults"]["CardDelay"] = 0; // That'd work as well just fine, since we check the amount too.
				}
			}
		}

		/* AI Mode. */
		if (Defaults.AIUsed) this->CFG["GameDefaults"]["AIMode"] = (uint8_t)Defaults.Method;
		else {
			if (this->CFG.contains("GameDefaults") && this->CFG["GameDefaults"].is_object()) {
				if (this->CFG["GameDefaults"].contains("AIMode") && this->CFG["GameDefaults"]["AIMode"].is_number()) {
					this->CFG["GameDefaults"]["AIMode"] = -1; // Set to -1 because unused.
				}
			}
		}

		if (Defaults.RoundsToWin > 0) this->CFG["GameDefaults"]["RoundsToWin"] = Defaults.RoundsToWin; // Rounds needed to win the game.
		this->CFG["GameDefaults"]["Player1Idx"] = Defaults.Characters[0]; // First character image index.
		this->CFG["GameDefaults"]["Player2Idx"] = Defaults.Characters[1]; // Second character image index.
		this->CFG["GameDefaults"]["Player1Name"] = Defaults.Names[0]; // First player name.
		this->CFG["GameDefaults"]["Player2Name"] = Defaults.Names[1]; // Second player name.
		this->CFG["GameDefaults"]["RoundStarter"] = (uint8_t)Defaults.Starter; // Round Starter.

		this->VDefaultParams = Defaults; // Set new defaults to current session as well.
		if (!this->ChangesMade) this->ChangesMade = true; // We modified it.
	}
}