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

#include "AISelector.hpp"
#include "CardSelector.hpp"
#include "CharacterSelector.hpp"
#include "Common.hpp"
#include "GameSettings.hpp"
#include "Keyboard.hpp"
#include "Numpad.hpp"
#include "Utils.hpp"

/* Toggles. */
void GameSettings::ToggleAI() {
	if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) { // Only available on Normal Play.
		this->Params.AIUsed = !this->Params.AIUsed;
	}
};

void GameSettings::ToggleDelay() { this->Params.CardDelayUsed = !this->Params.CardDelayUsed; };

void GameSettings::ToggleGameMode() {
	switch(this->Params.GameMode) {
		case GameSettings::GameModes::NormalPlay:
			this->Params.GameMode = GameSettings::GameModes::TryPlay;
			break;

		case GameSettings::GameModes::TryPlay:
			this->Params.GameMode = GameSettings::GameModes::NormalPlay;
			break;
	}
};

void GameSettings::ToggleStarter() {
	switch(this->Params.Starter) {
		case GameSettings::RoundStarter::Player1:
			this->Params.Starter = GameSettings::RoundStarter::Player2;
			break;

		case GameSettings::RoundStarter::Player2:
			this->Params.Starter = GameSettings::RoundStarter::Random;
			break;

		case GameSettings::RoundStarter::Random:
			this->Params.Starter = GameSettings::RoundStarter::Player1;
			break;
	}
};

/* Select the AI Difficulty. */
void GameSettings::SelectDifficulty() {
	if (this->Params.AIUsed) {
		if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) { // Only available on Normal Play.
			std::unique_ptr<AISelector> Ovl = std::make_unique<AISelector>(this->Params.Difficulty);
			this->Params.Difficulty = Ovl->Action();
		};
	}
};

/* Select the Amount of card pairs. */
void GameSettings::SelectCards() {
	std::unique_ptr<CardSelector> Ovl = std::make_unique<CardSelector>();
	Ovl->Action();
};

/* Give an OK for the Settings. */
void GameSettings::OK() { this->Done = true; };

/* Cancel the Game. */
void GameSettings::Cancel() { this->Params.CancelGame = true, this->Done = true; };

/* Selects the Character Picture. */
void GameSettings::SelectPicture(const bool AI) {
	if (AI) {
		if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) { // 2 Player only available on Normal Play.
			std::unique_ptr<CharacterSelector> Ovl = std::make_unique<CharacterSelector>(this->Params.Characters[1]);
			const int Res = Ovl->Action();
			if (Res != -1)  this->Params.Characters[1] = Res;
		};

	} else {
		std::unique_ptr<CharacterSelector> Ovl = std::make_unique<CharacterSelector>(this->Params.Characters[0]);
		const int Res = Ovl->Action();
		if (Res != -1)  this->Params.Characters[0] = Res;
	}
};

/* If Card Delay used -> Set the delay. */
void GameSettings::SetCardDelay() {
	if (this->Params.CardDelayUsed) {
		std::unique_ptr<Numpad> Ovl = std::make_unique<Numpad>(3, this->Params.CardDelay, 255, Lang::Get("GAME_SETTINGS_CARD_DELAY_TXT"));
		this->Params.CardDelay = Ovl->Action();
	}
};

/* Set the amount of wins you need to win the game. */
void GameSettings::SetWinRounds() {
	if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) { // Only available on Normal Play.
		std::unique_ptr<Numpad> Ovl = std::make_unique<Numpad>(3, this->Params.RoundsToWin, 255, Lang::Get("GAME_SETTINGS_ROUND_WIN_TXT"));
		this->Params.RoundsToWin = Ovl->Action();
	};
};

/* Set the name. */
void GameSettings::SetName(const bool AI) {
	if (AI) {
		if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) { // 2 Player only available on Normal Play.
			std::unique_ptr<Keyboard> Ovl = std::make_unique<Keyboard>(16, this->Params.Names[1], Lang::Get("GAME_SETTINGS_P2_ENTER_NAME"));
			this->Params.Names[1] = Ovl->Action();
		}

	} else {
		std::unique_ptr<Keyboard> Ovl = std::make_unique<Keyboard>(16, this->Params.Names[0], Lang::Get("GAME_SETTINGS_P1_ENTER_NAME"));
		this->Params.Names[0] = Ovl->Action();
	}
};

/* MAIN Action! */
GameSettings::GameParams GameSettings::Action() {
	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);

	/* Get the Player Names from the Translations. */
	this->Params.Names[0] = Lang::Get("PLAYER_1");
	this->Params.Names[1] = Lang::Get("PLAYER_2");

	while(!this->Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_TITLE"), 395);
		Gui::DrawSprite(GFX::Sprites, sprites_logo_idx, 72, 69); // Display Logo.
		GFX::DrawBottom();

		/* Draw Tabs. */
		Gui::Draw_Rect(0, 0, 160, 20, (this->Tab == Tabs::General ? KBD_KEYPRESSED : KD_KEYUNPRESSED));
		Gui::Draw_Rect(160, 0, 160, 20, (this->Tab == Tabs::Player ? KBD_KEYPRESSED : KD_KEYUNPRESSED));
		Gui::DrawStringCentered(-80, 3, 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_TAB_GENERAL"), 150);
		Gui::DrawStringCentered(80, 3, 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_TAB_PLAYERS"), 150);

		/* Draw Tab Content. */
		switch(this->Tab) {
			case Tabs::General: // AI, Pairs, Delay, Win...
				/* Game Mode. */
				Gui::DrawString(15, 40, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_GM"), 130);
				Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 150, 35);

				switch(this->Params.GameMode) {
					case GameSettings::GameModes::NormalPlay:
						Gui::DrawString(190, 40, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_GM_NORMAL"), 120);
						break;

					case GameSettings::GameModes::TryPlay:
						Gui::DrawString(190, 40, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_GM_TRIES"), 120);
						break;
				};


				/* Using Card Delay. */
				Gui::DrawString(15, 80, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_CARD_DELAY"), 130);
				GFX::DrawCheckbox(150, 75, this->Params.CardDelayUsed);

				/* Card Delay Value. */
				if (this->Params.CardDelayUsed) { // Only show if Card Delay enabled.
					Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 200, 75);
					Gui::DrawString(240, 80, 0.5f, TEXT_COLOR, std::to_string(this->Params.CardDelay));
				}

				/* Card Pairs. */
				Gui::DrawString(15, 120, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_CARD_PAIRS"), 130);
				Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 150, 115);
				Gui::DrawString(190, 120, 0.5f, TEXT_COLOR, std::to_string(Utils::Cards.size()));

				/* Using AI. */
				if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) { // Only available in Normal Play.
					Gui::DrawString(15, 160, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_AI_METHOD"), 130);
					GFX::DrawCheckbox(150, 155, this->Params.AIUsed);

					/* AI Difficulty. */
					if (this->Params.AIUsed) { // Only show if AI enabled.
						Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 200, 155);

						switch(this->Params.Difficulty) {
							case StackMem::AIDifficulty::Random:
								Gui::DrawString(240, 160, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_AI_RANDOM"), 70);
								break;

							case StackMem::AIDifficulty::Hard:
								Gui::DrawString(240, 160, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_AI_HARD"), 70);
								break;

							case StackMem::AIDifficulty::Extreme:
								Gui::DrawString(240, 160, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_AI_EXTREME"), 70);
								break;
						};
					}

					/* Rounds to win Value. */
					Gui::DrawString(15, 200, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_ROUND_WIN"), 130);
					Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 150, 195);
					Gui::DrawString(190, 200, 0.5f, TEXT_COLOR, std::to_string(this->Params.RoundsToWin));
				};

				break;

			case Tabs::Player:
				/* Player 1. */
				if (this->Params.Characters[0] < Utils::GetCharSheetSize()) Gui::DrawSprite(GFX::Characters, this->Params.Characters[0], 30, 30);
				Gui::Draw_Rect(37, 159, 106, 20, KD_KEYUNPRESSED);
				Gui::DrawString(40, 162, 0.45f, TEXT_COLOR, this->Params.Names[0], 100);

				/* Player 2. */
				if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) { // Only available in Normal Play.
					if (this->Params.Characters[1] < Utils::GetCharSheetSize()) Gui::DrawSprite(GFX::Characters, this->Params.Characters[1], 170, 30);
					Gui::Draw_Rect(177, 159, 106, 20, KD_KEYUNPRESSED);
					Gui::DrawString(180, 162, 0.45f, TEXT_COLOR, this->Params.Names[1], 100);
				};

				if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) { // Only available in Normal Play.
					Gui::DrawString(15, 200, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_STARTER"), 130);
					Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 150, 195);

					switch(this->Params.Starter) {
						case GameSettings::RoundStarter::Player1:
							Gui::DrawString(190, 200, 0.5f, TEXT_COLOR, this->Params.Names[0], 120);
							break;

						case GameSettings::RoundStarter::Player2:
							Gui::DrawString(190, 200, 0.5f, TEXT_COLOR, this->Params.Names[1], 120);
							break;

						case GameSettings::RoundStarter::Random:
							Gui::DrawString(190, 200, 0.5f, TEXT_COLOR, Lang::Get("RANDOM"), 120);
							break;
					}
				}
				break;
		};

		Gui::Draw_Rect(this->GeneralPos[9].X, this->GeneralPos[9].Y, this->GeneralPos[9].W, this->GeneralPos[9].H, KBD_KEYPRESSED); // Back.
		Gui::DrawSprite(GFX::Sprites, sprites_back_btn_idx, this->GeneralPos[9].X, this->GeneralPos[9].Y);

		Gui::Draw_Rect(this->GeneralPos[10].X, this->GeneralPos[10].Y, this->GeneralPos[10].W, this->GeneralPos[10].H, KBD_KEYPRESSED); // Back.
		Gui::DrawSprite(GFX::Sprites, sprites_next_btn_idx, this->GeneralPos[10].X, this->GeneralPos[10].Y);
		Pointer::Draw();
		C3D_FrameEnd(0);

		this->TabLogic();
	}

	Pointer::SetPos(0, 0);
	return this->Params;
};

void GameSettings::TabLogic() {
	hidScanInput();
	touchPosition T;
	hidTouchRead(&T);
	const uint32_t Down = hidKeysDown();
	const uint32_t Held = hidKeysHeld();
	Pointer::ScrollHandling(Held);

	if (Down & KEY_B) this->Cancel();
	if (Down & KEY_START) this->OK();
	if (Down & KEY_R) this->PlayerTab();
	if (Down & KEY_L) this->GeneralTab();

	switch(this->Tab) {
		case Tabs::General:
			if (Down & KEY_A) {
				for (auto &Position : this->GeneralPos) {
					if (Pointer::Clicked(Position, true)) break;
				}
			}

			if (Down & KEY_TOUCH) {
				for (auto &Position : this->GeneralPos) {
					if (Touched(Position, T, true)) break;
				}
			}
			break;

		case Tabs::Player:
			if (Down & KEY_A) {
				for (auto &Position : this->PlayerPos) {
					if (Pointer::Clicked(Position, true)) break;
				}
			}

			if (Down & KEY_TOUCH) {
				for (auto &Position : this->PlayerPos) {
					if (Touched(Position, T, true)) break;
				}
			}
			break;
	};
};