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
#include "GameOverlay.hpp"
#include "GameResult.hpp"


/* Initialize with the Game Parameters. */
GameOverlay::GameOverlay(const GameSettings::GameParams Params) { this->Helper = std::make_unique<GameHelper>(Params); };


void GameOverlay::Action() {
	while(aptMainLoop() && this->Running) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		this->Helper->DrawTop();
		this->Helper->DrawField();
		C3D_FrameEnd(0);

		hidScanInput();
		touchPosition T;
		hidTouchRead(&T);
		const uint32_t Down = hidKeysDown();
		const uint32_t Held = hidKeysHeld();

		/* Logic. */
		const GameHelper::LogicState State = this->Helper->Logic(Down, Held, T);
		if (State != GameHelper::LogicState::Nothing) { // Someone won already.
			std::unique_ptr<GameResult> Ovl = std::make_unique<GameResult>();

			/* Get the proper Overlay. They also set the win count to ++ for the winner. */
			switch(State) {
				case GameHelper::LogicState::Tie:
					Ovl->Action(this->Helper->ReturnParams(), 0);
					break;

				case GameHelper::LogicState::P1Won:
					Ovl->Action(this->Helper->ReturnParams(), 1);
					break;

				case GameHelper::LogicState::P2Won:
					Ovl->Action(this->Helper->ReturnParams(), 2);
					break;

				case GameHelper::LogicState::Nothing:
					break; // Should never happen.
			};

			/* If we reached the needed win amount, are on the Try Play Mode -> go the screen back to the MainMenu again. */
			if ((this->Helper->ReturnParams().Wins[0] >= this->Helper->ReturnParams().RoundsToWin)
			|| (this->Helper->ReturnParams().Wins[1] >= this->Helper->ReturnParams().RoundsToWin)
			|| (this->Helper->ReturnParams().GameMode == GameSettings::GameModes::TryPlay)) {
				this->Running = false; // Game is not running anymore.
				return;

			} else {
				this->Helper->StartGame(true, { }, State == GameHelper::LogicState::P2Won);
			}
		}
	};
};