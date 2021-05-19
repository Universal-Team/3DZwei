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

/* Selectors. */
#include "AISelector.hpp"
#include "CardSelector.hpp"
#include "CharacterSelector.hpp"
#include "GameSettings.hpp"

/* Inputs. */
#include "Keyboard.hpp"
#include "Numpad.hpp"

/* Card Utils. */
#include "Utils.hpp"


/* Constructor. */
GameSettings::GameSettings(const GameSettings::GameParams &Defaults, const bool IsSetting) : Params(Defaults), IsSetting(IsSetting) {
	if (!_3DZwei::CFG->DoAnimation()) this->FAlpha = 0;
};


/* Toggle if the AI should be used. */
void GameSettings::ToggleAI() {
	if (this->Params.GameMode == GameSettings::GameModes::Versus) { // Only available on Versus Mode.
		this->Params.AIUsed = !this->Params.AIUsed;
	}
};


/* Toggle if the card delay should be used. */
void GameSettings::ToggleDelay() { this->Params.CardDelayUsed = !this->Params.CardDelayUsed; };


/* Toggle which game mode should be used. */
void GameSettings::ToggleGameMode() {
	switch(this->Params.GameMode) {
		case GameSettings::GameModes::Versus:
			this->Params.GameMode = GameSettings::GameModes::Solo;
			break;

		case GameSettings::GameModes::Solo:
			this->Params.GameMode = GameSettings::GameModes::Versus;
			break;
	}
};


/* Toggle who should start. */
void GameSettings::ToggleStarter() {
	switch(this->Params.Starter) {
		case GameSettings::RoundStarter::Player1:
			this->Params.Starter = GameSettings::RoundStarter::Player2;
			break;

		case GameSettings::RoundStarter::Player2:
			this->Params.Starter = GameSettings::RoundStarter::Random;
			break;

		case GameSettings::RoundStarter::Random:
			this->Params.Starter = GameSettings::RoundStarter::Loser;
			break;

		case GameSettings::RoundStarter::Loser:
			this->Params.Starter = GameSettings::RoundStarter::Winner;
			break;

		case GameSettings::RoundStarter::Winner:
			this->Params.Starter = GameSettings::RoundStarter::Player1;
			break;
	}
};


/* Select the AI Method. */
void GameSettings::SelectAIMethod() {
	if (this->Params.AIUsed) {
		if (this->Params.GameMode == GameSettings::GameModes::Versus) { // Only available on Versus Mode.
			this->FadeOut();

			std::unique_ptr<AISelector> Ovl = std::make_unique<AISelector>(this->Params.Method);
			this->Params.Method = Ovl->Action();

			this->FAlpha = 255;
		};
	}
};


/* Select the Amount of card pairs. */
void GameSettings::SelectCards() {
	this->FadeOut();

	std::unique_ptr<CardSelector> Ovl = std::make_unique<CardSelector>();
	Ovl->Action();

	this->FAlpha = 255;
};


/* Give an OK for the Settings and start the game. */
void GameSettings::OK() { this->Done = true; };


/* Cancel the Game and return back to the Main Overlay. */
void GameSettings::Cancel() { this->Params.CancelGame = true, this->Done = true; };


/*
	Selects the Character Picture.

	const bool AI: If the AI or Player 2 picture should be selected or not.
*/
void GameSettings::SelectPicture(const bool AI) {
	if (AI) {
		if (this->Params.GameMode == GameSettings::GameModes::Versus) { // 2 Player only available on Versus Mode.
			this->FadeOut();

			std::unique_ptr<CharacterSelector> Ovl = std::make_unique<CharacterSelector>(this->Params.Characters[1]);
			const int Res = Ovl->Action();
			if (Res != -1)  this->Params.Characters[1] = Res;

			this->FAlpha = 255;
		};

	} else {
		this->FadeOut();

		std::unique_ptr<CharacterSelector> Ovl = std::make_unique<CharacterSelector>(this->Params.Characters[0]);
		const int Res = Ovl->Action();
		if (Res != -1)  this->Params.Characters[0] = Res;

		this->FAlpha = 255;
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
	if (this->Params.GameMode == GameSettings::GameModes::Versus) { // Only available on Versus Mode.
		std::unique_ptr<Numpad> Ovl = std::make_unique<Numpad>(3, this->Params.RoundsToWin, 255, Lang::Get("GAME_SETTINGS_ROUND_WIN_TXT"));
		this->Params.RoundsToWin = Ovl->Action();
	};
};


/*
	Set the Name of the Players.

	const bool AI: If the AI or Player 2 Name should be set or not.
*/
void GameSettings::SetName(const bool AI) {
	if (AI) {
		if (this->Params.GameMode == GameSettings::GameModes::Versus) { // 2 Player only available on Versus Mode.
			std::unique_ptr<Keyboard> Ovl = std::make_unique<Keyboard>(16, this->Params.Names[1], Lang::Get("GAME_SETTINGS_P2_ENTER_NAME"));
			this->Params.Names[1] = Ovl->Action();
		}

	} else {
		std::unique_ptr<Keyboard> Ovl = std::make_unique<Keyboard>(16, this->Params.Names[0], Lang::Get("GAME_SETTINGS_P1_ENTER_NAME"));
		this->Params.Names[0] = Ovl->Action();
	}
};


/* Draw the Overlay. */
void GameSettings::Draw(const bool IsSetting) {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, (IsSetting ? Lang::Get("SETTINGS_GAME_DEFAULTS") : Lang::Get("GAME_SETTINGS_TITLE")), 395);
	Gui::DrawSprite(GFX::Sprites, sprites_logo_idx, 72, 69); // Display Logo.
	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));
	};

	GFX::DrawBottom();

	/* Draw Tabs. */
	if (this->Tab != Tabs::General) Gui::Draw_Rect(0, 0, 160, 20, KBD_KEYUNPRESSED);
	if (this->Tab != Tabs::Player) Gui::Draw_Rect(160, 0, 160, 20, KBD_KEYUNPRESSED);
	Gui::DrawStringCentered(-80, 3, 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_TAB_GENERAL"), 150);
	Gui::DrawStringCentered(80, 3, 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_TAB_PLAYERS"), 150);

	/* Only draw General Tab, if the Tab is larger as -320. */
	if (this->T1Offs > -320) {
		/* Game Mode. */
		Gui::DrawString(15 + this->T1Offs, 40 - (Lang::Get("GAME_SETTINGS_GM").length() / 25 * 10 / 2), 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_GM"), 125, 15, nullptr, C2D_WordWrap);
		Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 150 + this->T1Offs, 35);

		switch(this->Params.GameMode) {
			case GameSettings::GameModes::Versus:
				Gui::DrawString(190 + this->T1Offs, 40, 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_GM_NORMAL"), 120);
				break;

			case GameSettings::GameModes::Solo:
				Gui::DrawString(190 + this->T1Offs, 40, 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_GM_TRIES"), 120);
				break;
		};


		/* Using Card Delay. */
		Gui::DrawString(15 + this->T1Offs, 80 - (Lang::Get("GAME_SETTINGS_CARD_DELAY").length() / 25 * 10 / 2), 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_CARD_DELAY"), 125, 15, nullptr, C2D_WordWrap);
		GFX::DrawCheckbox(150 + this->T1Offs, 75, this->Params.CardDelayUsed);

		/* Card Delay Value. */
		if (this->Params.CardDelayUsed) { // Only show if Card Delay enabled.
			Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 200 + this->T1Offs, 75);
			Gui::DrawString(240 + this->T1Offs, 80, 0.45f, TEXT_COLOR, std::to_string(this->Params.CardDelay));
		}

		/* Card Pairs. */
		Gui::DrawString(15 + this->T1Offs, 120 - (Lang::Get("GAME_SETTINGS_CARD_PAIRS").length() / 25 * 10 / 2), 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_CARD_PAIRS"), 125, 15, nullptr, C2D_WordWrap);
		Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 150 + this->T1Offs, 115);
		Gui::DrawString(190 + this->T1Offs, 120, 0.45f, TEXT_COLOR, std::to_string(Utils::Cards.size()));

		/* Using AI. */
		if (this->Params.GameMode == GameSettings::GameModes::Versus) { // Only available in Versus Mode.
			Gui::DrawString(15 + this->T1Offs, 160 - (Lang::Get("GAME_SETTINGS_AI_METHOD").length() / 25 * 10 / 2), 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_AI_METHOD"), 125, 15, nullptr, C2D_WordWrap);
			GFX::DrawCheckbox(150 + this->T1Offs, 155, this->Params.AIUsed);

			/* AI Method. */
			if (this->Params.AIUsed) { // Only show if AI enabled.
				Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 200 + this->T1Offs, 155);

				switch(this->Params.Method) {
					case StackMem::AIMethod::Random:
						Gui::DrawString(240 + this->T1Offs, 160, 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_AI_RANDOM"), 70);
						break;

					case StackMem::AIMethod::Hard:
						Gui::DrawString(240 + this->T1Offs, 160, 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_AI_HARD"), 70);
						break;

					case StackMem::AIMethod::Extreme:
						Gui::DrawString(240 + this->T1Offs, 160, 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_AI_EXTREME"), 70);
						break;
				};
			}

			/* Rounds to win Value. */
			Gui::DrawString(15 + this->T1Offs, 200 - (Lang::Get("GAME_SETTINGS_ROUND_WIN").length() / 25 * 10 / 2), 0.45f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_ROUND_WIN"), 125, 15, nullptr, C2D_WordWrap);
			Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 150 + this->T1Offs, 195);
			Gui::DrawString(190 + this->T1Offs, 200, 0.45f, TEXT_COLOR, std::to_string(this->Params.RoundsToWin));
		};
	};

	/* Only draw Players Tab, if the Tab is smaller as 320. */
	if (this->T2Offs < 320) {
		/* Player 1. */
		if (this->Params.Characters[0] < Utils::GetCharSheetSize()) Gui::DrawSprite(GFX::Characters, this->Params.Characters[0], 30 + this->T2Offs, 30);
		Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 28 + this->T2Offs, 28);
		Gui::Draw_Rect(37 + this->T2Offs, 159, 106, 20, KBD_KEYUNPRESSED);
		Gui::DrawString(40 + this->T2Offs, 162, 0.45f, TEXT_COLOR, this->Params.Names[0], 100);

		/* Player 2. */
		if (this->Params.GameMode == GameSettings::GameModes::Versus) { // Only available in Versus Mode.
			if (this->Params.Characters[1] < Utils::GetCharSheetSize()) Gui::DrawSprite(GFX::Characters, this->Params.Characters[1], 170 + this->T2Offs, 30);
			Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 168 + this->T2Offs, 28);
			Gui::Draw_Rect(177 + this->T2Offs, 159, 106, 20, KBD_KEYUNPRESSED);
			Gui::DrawString(180 + this->T2Offs, 162, 0.45f, TEXT_COLOR, this->Params.Names[1], 100);
		};

		if (this->Params.GameMode == GameSettings::GameModes::Versus) { // Only available in Versus Mode.
			Gui::DrawString(15 + this->T2Offs, 200, 0.5f, TEXT_COLOR, Lang::Get("GAME_SETTINGS_STARTER"), 130);
			Gui::DrawSprite(GFX::Sprites, sprites_stripe_idx, 150 + this->T2Offs, 195);

			switch(this->Params.Starter) {
				case GameSettings::RoundStarter::Player1:
					Gui::DrawString(190 + this->T2Offs, 200, 0.5f, TEXT_COLOR, this->Params.Names[0], 120);
					break;

				case GameSettings::RoundStarter::Player2:
					Gui::DrawString(190 + this->T2Offs, 200, 0.5f, TEXT_COLOR, this->Params.Names[1], 120);
					break;

				case GameSettings::RoundStarter::Random:
					Gui::DrawString(190 + this->T2Offs, 200, 0.5f, TEXT_COLOR, Lang::Get("RANDOM"), 120);
					break;

				case GameSettings::RoundStarter::Loser:
					Gui::DrawString(190 + this->T2Offs, 200, 0.5f, TEXT_COLOR, Lang::Get("LOSER"), 120);
					break;

				case GameSettings::RoundStarter::Winner:
					Gui::DrawString(190 + this->T2Offs, 200, 0.5f, TEXT_COLOR, Lang::Get("WINNER"), 120);
					break;
			}
		}
	};

	Gui::Draw_Rect(this->GeneralPos[9].X, this->GeneralPos[9].Y, this->GeneralPos[9].W, this->GeneralPos[9].H, KBD_KEYPRESSED); // Back.
	Gui::DrawSprite(GFX::Sprites, sprites_back_btn_idx, this->GeneralPos[9].X, this->GeneralPos[9].Y);
	Gui::Draw_Rect(this->GeneralPos[10].X, this->GeneralPos[10].Y, this->GeneralPos[10].W, this->GeneralPos[10].H, KBD_KEYPRESSED); // Next.
	Gui::DrawSprite(GFX::Sprites, sprites_next_btn_idx, this->GeneralPos[10].X, this->GeneralPos[10].Y);
	Pointer::Draw();

	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
	};
};


/* Handle Fade-Outs. */
void GameSettings::FadeOut() {
	this->FAlpha = 0;

	while(aptMainLoop() && this->FAlpha < 255) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		this->Draw();
		C3D_FrameEnd(0);

		hidScanInput();
		const uint32_t Down = hidKeysDown();
		if ((!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade()) || Down) this->FAlpha = 255;
		else this->FAlpha += 5;
	};
};


/* MAIN Action! */
GameSettings::GameParams GameSettings::Action() {
	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);

	/* Get the Player Names from the Translations if not from the defaults. */
	if (this->Params.Names[0] == "%Player1%") this->Params.Names[0] = Lang::Get("PLAYER_1");
	if (this->Params.Names[1] == "%Player2%") this->Params.Names[1] = Lang::Get("PLAYER_2");

	/* Check for in range, and if out of range, set to 0. */
	if (this->Params.Characters[0] >= Utils::GetCharSheetSize()) this->Params.Characters[0] = 0;
	if (this->Params.Characters[1] >= Utils::GetCharSheetSize()) this->Params.Characters[1] = 0;

	while(aptMainLoop() && !this->FullDone) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		this->Draw(this->IsSetting);
		C3D_FrameEnd(0);
		this->TabLogic();
	};

	Pointer::SetPos(0, 0);
	return this->Params;
};


/* Handles the Tab Logic. */
void GameSettings::TabLogic() {
	hidScanInput();
	const uint32_t Down = hidKeysDown();

	/* Because we gave it an OK, we're done and fade out. */
	if (this->Done) {
		if (this->FAlpha < 255) {
			if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade() || Down) this->FAlpha = 0, this->FullDone = true;
			else {
				this->FAlpha += 5;

				if (this->FAlpha >= 255) this->FullDone = true;
			}
		}

		return;
	};

	/* Basically Fade-In handler. */
	if (this->FAlpha > 0) {
		if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade()) this->FAlpha = 0;
		else this->FAlpha -= 5;
	}

	/* Initial Swipe in the first tab. */
	if (this->InitialSwipe) {
		if (!_3DZwei::CFG->DoAnimation()) {
			this->T1Offs = 0;
			this->Tab = Tabs::General;
			this->InitialSwipe = false;

		} else {
			if (this->T1Offs <= 0) {
				this->Cubic = std::lerp(this->Cubic, 321.0f, 0.1f);
				this->T1Offs = -320 + this->Cubic;

				if (this->Cubic >= 320.0f) {
					this->T1Offs = 0;
					this->Cubic = 0.0f;
					this->Tab = Tabs::General;
					this->InitialSwipe = false;
				}
			}
		}

		return;
	};

	/* Swipe Handling. */
	if (this->DoSwipe) {
		if (this->SwipeDirection) { // Players Tab.
			if (!_3DZwei::CFG->DoAnimation() || Down) {
				this->T1Offs = -320;
				this->T2Offs = 0;
				this->Tab = Tabs::Player;
				this->DoSwipe = false;
				this->Cubic = 0.0f;

			} else {
				if (this->Cubic < 320.0f) {
					this->Cubic = std::lerp(this->Cubic, 321.0f, 0.1f);

					this->T1Offs = 0 - this->Cubic;
					this->T2Offs = 320 - this->Cubic;

					if (this->Cubic >= 320.0f) {
						this->T1Offs = -320;
						this->T2Offs = 0;
						this->Cubic = 0.0f;
						this->Tab = Tabs::Player;
						this->DoSwipe = false;
					}
				}
			}

		} else { // General Tab.
			if (!_3DZwei::CFG->DoAnimation() || Down) {
				this->T1Offs = 0;
				this->T2Offs = 320;
				this->Tab = Tabs::General;
				this->DoSwipe = false;
				this->Cubic = 0.0f;

			} else {
				if (this->Cubic < 320.0f) {
					this->Cubic = std::lerp(this->Cubic, 321.0f, 0.1f);
					this->T2Offs = this->Cubic;
					this->T1Offs = -320 + this->Cubic;

					if (this->Cubic >= 320.0f) {
						this->T1Offs = 0;
						this->T2Offs = 320;
						this->Tab = Tabs::General;
						this->Cubic = 0.0f;
						this->DoSwipe = false;
					}
				}
			}
		}

		return;
	};

	touchPosition T;
	hidTouchRead(&T);
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