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

#include "Common.hpp"
#include "JSON.hpp"
#include "Lang.hpp"
#include <stdio.h>
#include <unistd.h>


static nlohmann::json AppJSON;


/*
	Gets a translated string from the JSON.

	const std::string &Key: The string to get from the translation.
*/
std::string Lang::Get(const std::string &Key) {
	if (!AppJSON.contains(Key)) return "";

	return AppJSON.at(Key).get_ref<const std::string &>();
}


/* Loads the Language Strings. */
void Lang::Load() {
	FILE *In = nullptr;
	bool Good = true;

	if (_3DZwei::CFG->Lang() != "") { // Ensure it isn't ''.
		for (size_t Idx = 0; Idx < _3DZwei::CFG->Lang().size(); Idx++) {
			if (_3DZwei::CFG->Lang()[Idx] == '/') { // Contains a '/' and hence breaks.
				Good = false;
				break;
			}
		}
	}

	if (Good) {
		if (access(("romfs:/lang/" + _3DZwei::CFG->Lang() + "/app.json").c_str(), F_OK) == 0) { // Ensure access is ok.
			In = fopen(("romfs:/lang/" + _3DZwei::CFG->Lang() + "/app.json").c_str(), "r");
			if (In)	AppJSON = nlohmann::json::parse(In, nullptr, false);
			fclose(In);

		} else {
			Good = false;
		}
	}

	if (!Good) {
		/* Load English. */
		In = fopen("romfs:/lang/en/app.json", "r");
		if (In)	AppJSON = nlohmann::json::parse(In, nullptr, false);
		fclose(In);
		_3DZwei::CFG->Lang("en"); // Set back to english too.
	}
}