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
GameOverlay::GameOverlay(const GameSettings::GameParams Params) {
	this->Helper = std::make_unique<GameHelper>(Params);
	hidScanInput();
};

void GameOverlay::Draw() {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

	this->Helper->DrawTop();
	if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha)); // Dimming.
	this->Helper->DrawField();
	if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha)); // Dimming.

	if (this->PromptHandle && !this->PromptAnswer) {
		if (this->SwipeIn) {
			Gui::DrawStringCentered(0, 80 + 190 - this->FAlpha, 0.6f, TEXT_WHITE, Lang::Get("EXIT_GAME"));

		} else if (this->SwipeOut) {
			Gui::DrawStringCentered(0, 80 + 190 - this->FAlpha, 0.6f, TEXT_WHITE, Lang::Get("EXIT_GAME"));

		} else {
			Gui::DrawStringCentered(0, 80, 0.6f, TEXT_WHITE, Lang::Get("EXIT_GAME"));
		}

		/* Draw Prompt buttons. */
		for (size_t Idx = 0; Idx < 2; Idx++) {
			if (this->SwipeIn) {
				Gui::Draw_Rect(this->Prompt[Idx].X, this->Prompt[Idx].Y + 190 - this->FAlpha, this->Prompt[Idx].W, this->Prompt[Idx].H, BAR_BLUE);
				Gui::DrawStringCentered(this->Prompt[Idx].X - 160 + this->Prompt[Idx].W / 2, this->Prompt[Idx].Y + 15 + 190 - this->FAlpha, 0.6f, TEXT_WHITE, Lang::Get((Idx == 0 ? "CANCEL" : "CONFIRM")), this->Prompt[Idx].W - 10);

			} else if (this->SwipeOut) {
				Gui::Draw_Rect(this->Prompt[Idx].X, this->Prompt[Idx].Y + 190 - this->FAlpha, this->Prompt[Idx].W, this->Prompt[Idx].H, BAR_BLUE);
				Gui::DrawStringCentered(this->Prompt[Idx].X - 160 + this->Prompt[Idx].W / 2, this->Prompt[Idx].Y + 15 + 190 - this->FAlpha, 0.6f, TEXT_WHITE, Lang::Get((Idx == 0 ? "CANCEL" : "CONFIRM")), this->Prompt[Idx].W - 10);

			} else {
				Gui::Draw_Rect(this->Prompt[Idx].X, this->Prompt[Idx].Y, this->Prompt[Idx].W, this->Prompt[Idx].H, BAR_BLUE);
				Gui::DrawStringCentered(this->Prompt[Idx].X - 160 + this->Prompt[Idx].W / 2, this->Prompt[Idx].Y + 15, 0.6f, TEXT_WHITE, Lang::Get((Idx == 0 ? "CANCEL" : "CONFIRM")), this->Prompt[Idx].W - 10);
			}
		}
	};

	C3D_FrameEnd(0);
};


void GameOverlay::Confirm() { this->PromptAnswer = true, this->SwipeOut = true; };
void GameOverlay::Cancel() {
	this->PromptAnswer = false, this->SwipeOut = true, this->Helper->ReturnParams().CancelGame = false;
};


/* Prompt logic. */
void GameOverlay::PromptLogic() {
	hidScanInput();
	touchPosition T;
	hidTouchRead(&T);
	const uint32_t Down = hidKeysDown();

	if (this->SwipeIn) {
		if (!_3DZwei::CFG->DoAnimation()) this->FAlpha = 0, this->SwipeIn = false;
		else {
			if (this->Cubic < 190.0f) {
				this->Cubic = std::lerp(this->Cubic, 191.0f, 0.1f);
				this->FAlpha = this->Cubic;

				if (this->Cubic >= 190.0f) {
					this->FAlpha = 190, this->Cubic = 0.0f, this->SwipeIn = false;
				}
			}
		}

		return;
	};

	if (this->SwipeOut) {
		if (this->PromptAnswer) {
			if (!_3DZwei::CFG->DoAnimation()) this->FAlpha = 255, this->SwipeOut = false, this->Running = false;
			else {
				if (this->Cubic < 60.0f) {
					this->Cubic = std::lerp(this->Cubic, 61.0f, 0.1f);
					this->FAlpha = 190 + this->Cubic;

					if (this->Cubic >= 60.0f) {
						this->FAlpha = 0, this->Cubic = 0.0f, this->SwipeOut = false, this->Running = false;
					}
				}
			};

		} else {
			if (!_3DZwei::CFG->DoAnimation()) this->FAlpha = 0, this->SwipeOut = false, this->PromptAnswer = false;
			else {
				if (this->Cubic < 190.0f) {
					this->Cubic = std::lerp(this->Cubic, 191.0f, 0.1f);
					this->FAlpha = (190 - this->Cubic);

					if (this->Cubic >= 190.0f) {
						this->FAlpha = 0, this->Cubic = 0.0f, this->SwipeOut = false, this->PromptHandle = false;
					}
				}
			}
		};

		return;
	};

	if (Down & KEY_TOUCH) {
		for (auto Pos : this->Prompt) {
			if (Touched(Pos, T, true)) break;
		}
	};
};


void GameOverlay::Action() {
	while(aptMainLoop() && this->Running) {
		this->Draw();

		if (this->PromptHandle) this->PromptLogic();
		else {
			if (this->PromptAnswer) this->Running = false;
			hidScanInput();
			touchPosition T;
			hidTouchRead(&T);
			const uint32_t Down = hidKeysDown();
			const uint32_t Held = hidKeysHeld();
			const uint32_t Repeat = hidKeysDownRepeat();

			/* Logic. */
			const GameHelper::LogicState State = this->Helper->Logic(Down, Held, Repeat, T);
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

				/* If we reached the needed win amount, are on Solo Mode -> go the screen back to the MainMenu again. */
				if ((this->Helper->ReturnParams().Wins[0] >= this->Helper->ReturnParams().RoundsToWin)
				|| (this->Helper->ReturnParams().Wins[1] >= this->Helper->ReturnParams().RoundsToWin)
				|| (this->Helper->ReturnParams().GameMode == GameSettings::GameModes::Solo)) {
					this->Running = false; // Game is not running anymore.
					return;

				} else {
					this->Helper->StartGame(true, { }, State == GameHelper::LogicState::P2Won);
					hidScanInput();
				}
			}

			if (this->Helper->ReturnParams().CancelGame) {
				this->PromptHandle = true;
				this->SwipeIn = true;
			};
		};
	};
};