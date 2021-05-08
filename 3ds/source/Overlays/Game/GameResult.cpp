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

#include "Common.hpp"
#include "GameResult.hpp"
#include "Utils.hpp"

/*
	Pass the GameParameters as reference, because we modify the Win count + reset the pairs.
*/
void GameResult::Action(GameSettings::GameParams &Params, const uint8_t Won) {
	/* Increase Wins on Normal Play. */
	if (Params.GameMode == GameSettings::GameModes::NormalPlay) {
		if (Won == 1) Params.Wins[0]++;
		else if (Won == 2) Params.Wins[1]++;
	};

	while(!this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("GAME_RESULT_TITLE"), 395);

		switch(Params.GameMode) {
			case GameSettings::GameModes::NormalPlay:
				/* Draw Player 1. */
				if (Params.Characters[0] < Utils::GetCharSheetSize()) {
					Gui::DrawSprite(GFX::Characters, Params.Characters[0], 10, 30);
					Gui::Draw_Rect(17, 159, 106, 20, KD_KEYUNPRESSED);
					Gui::DrawString(20, 162, 0.45f, TEXT_COLOR, Params.Names[0], 100);

					Gui::DrawString(20, 190, 0.45f, TEXT_COLOR, Lang::Get("PAIRS") + std::to_string(Params.PlayerPairs[0]), 100);
					Gui::DrawString(20, 210, 0.45f, TEXT_COLOR, Lang::Get("WINS") + std::to_string(Params.Wins[0]), 100);
				}

				/* Draw Player 2. */
				if (Params.Characters[1] < Utils::GetCharSheetSize()) {
					Gui::DrawSprite(GFX::Characters, Params.Characters[1], 270, 30);
					Gui::Draw_Rect(277, 159, 106, 20, KD_KEYUNPRESSED);
					Gui::DrawString(280, 162, 0.45f, TEXT_COLOR, Params.Names[1], 100);

					Gui::DrawString(277, 190, 0.45f, TEXT_COLOR, Lang::Get("PAIRS") + std::to_string(Params.PlayerPairs[1]), 100);
					Gui::DrawString(277, 210, 0.45f, TEXT_COLOR, Lang::Get("WINS") + std::to_string(Params.Wins[1]), 100);
				}

				Gui::DrawStringCentered(0, 115, 0.4f, TEXT_COLOR, Lang::Get("WINS_WIN") + std::to_string(Params.RoundsToWin), 240);

				switch(Won) {
					case 0:
						Gui::DrawStringCentered(0, 100, 0.4f, TEXT_COLOR, Lang::Get("TIE_RESULT"), 240);
						break;

					case 1:
						{
							char Buffer[100]; snprintf(Buffer, sizeof(Buffer), Lang::Get("WON").c_str(), Params.Names[0].c_str());
							Gui::DrawStringCentered(0, 100, 0.4f, TEXT_COLOR, Buffer, 240);
						}
						break;

					case 2:
						{
							char Buffer[100]; snprintf(Buffer, sizeof(Buffer), Lang::Get("WON").c_str(), Params.Names[1].c_str());
							Gui::DrawStringCentered(0, 100, 0.4f, TEXT_COLOR, Buffer, 240);
						}
						break;
				};
				break;

			case GameSettings::GameModes::TryPlay:
				/* Draw Player 1. */
				if (Params.Characters[0] < Utils::GetCharSheetSize()) {
					Gui::DrawSprite(GFX::Characters, Params.Characters[0], 30, 30);
					Gui::Draw_Rect(37, 159, 106, 20, KD_KEYUNPRESSED);
					Gui::DrawString(40, 162, 0.45f, TEXT_COLOR, Params.Names[0], 100);
				}

				Gui::DrawStringCentered(0, 100, 0.45f, TEXT_COLOR, Lang::Get("YOUR_TRIES") + std::to_string(Params.Tries), 100);
				break;
		}

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190)); // Darker screen.
		C3D_FrameEnd(0);

		hidScanInput();
		const uint32_t Down = hidKeysDown();

		if (Down & KEY_A || Down & KEY_TOUCH) this->Done = true;
	};

	/* Reset Pairs. */
	if (Params.GameMode == GameSettings::GameModes::NormalPlay) {
		for (uint8_t Idx = 0; Idx < 2; Idx++) Params.PlayerPairs[Idx] = 0;
	}
};