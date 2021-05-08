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
#include "GameScreen.hpp"
#include "Utils.hpp"

/* Initialize with the Game Parameters. */
GameScreen::GameScreen(const GameSettings::GameParams Param) {
	this->Params = Param;
	this->Game = std::make_unique<StackMem>(Utils::Cards.size(), this->Params.AIUsed, this->Params.Difficulty);

	if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) { // Only select the Starter at the normal play mode.
		switch(this->Params.Starter) { // Get the starter.
			case GameSettings::RoundStarter::Player1:
				this->Game->SetCurrentPlayer(StackMem::Players::Player1);
				break;

			case GameSettings::RoundStarter::Player2:
				this->Game->SetCurrentPlayer(StackMem::Players::Player2);
				break;

			case GameSettings::RoundStarter::Random:
				this->Game->SelectRandomPlayer();
					break;
		};
	}
};

/* Goes to the previous page. */
void GameScreen::PrevPage() {
	if (this->Page > 0) this->Page--;
};

/* Goes to the next page. */
void GameScreen::NextPage() {
	if (this->CanGoNext()) this->Page++;
};

/* Returns if you can go to the next page. */
bool GameScreen::CanGoNext() const { return ((this->Page * 20) + 20 < (this->Game->GetPairs() * 2)); };

/* The main play of the players. */
void GameScreen::CheckCard(const uint8_t Idx) {
	if (((this->Page * 20) + Idx < this->Game->GetPairs() * 2)) {
		if (!this->Game->IsCardShown((this->Page * 20) + Idx)) {
			this->Game->DoPlay((this->Page * 20) + Idx);
		}
	}
};

/* Least amount of tries Play Draw. */
void GameScreen::DrawTryPlay(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("GAME_SCREEN_TITLE"), 395);

	/* Draw Player 1. */
	if (this->Params.Characters[0] < Utils::GetCharSheetSize()) {
		Gui::DrawSprite(GFX::Characters, this->Params.Characters[0], 30, 30);
		Gui::Draw_Rect(37, 159, 106, 20, KD_KEYUNPRESSED);
		Gui::DrawString(40, 162, 0.45f, TEXT_COLOR, this->Params.Names[0], 100);
	}

	Gui::DrawString(150, 100, 0.5f, TEXT_COLOR, Lang::Get("TRIES") + std::to_string(this->Params.Tries), 200);

	if (this->Game->GetPairs() > 10) { // Only 11+ Pairs have pages.
		Gui::DrawString(150, 130, 0.5f, TEXT_COLOR, Lang::Get("GAME_SCREEN_CURRENT_PAGE") + std::to_string(this->Page + 1) + " / " + std::to_string(((this->Game->GetPairs() * 2) / 20) + 1), 200);
	}

	if (!this->Params.CardDelayUsed || this->Params.CardDelay == 0) { // The checks only exist on non delay mode.
		if (this->Game->GetState() == StackMem::TurnState::DoCheck) {
			Gui::DrawStringCentered(0, 225, 0.4f, TEXT_COLOR, Lang::Get("GAME_SCREEN_ANY_KEY"), 390);
		}
	}

	Gui::DrawStringCentered(0, 200, 0.5f, TEXT_COLOR, Lang::Get("GAME_SCREEN_EXIT"), 395);

	GFX::DrawBottom();

	/* Only draw those, if over 10. */
	if (this->Game->GetPairs() > 10) {
		/* Because we can go back. */
		if (this->Page > 0) {
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0, 0); // Draw the small top corner.
			Gui::Draw_Rect(0, 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0, 220, 1.0f, -1.0f); // Draw the small bottom corner.
			Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 0, 110); // Now the arrow!
		}

		/* Because we can go forward. */
		if (this->CanGoNext()) {
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300, 0, -1.0f, 1.0f); // Draw the small top corner.
			Gui::Draw_Rect(300, 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300, 220, -1.0f, -1.0f); // Draw the small bottom corner.
			Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 300, 110, -1.0f, 1.0f); // Now the arrow!
		}
	}

	/* Draw Field. */
	for (size_t Idx = (this->Page * 20), Idx2 = 0; Idx < (this->Page * 20) + 20 && Idx < (this->Game->GetPairs() * 2); Idx++, Idx2++) {
		if (!this->Game->IsCardCollected(Idx)) { // If the card is not already collected.
			if (this->Game->IsCardShown(Idx)) GFX::DrawCard(this->Game->GetCardType(Idx), this->CPos[Idx2].X, this->CPos[Idx2].Y);
			else Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[Idx2].X, this->CPos[Idx2].Y); // Back cover because hidden.
		}
	}

	Gui::DrawSprite(GFX::Sprites, sprites_grid_idx, 19.5, 7.5);
	Pointer::Draw();
};


/* Normal Play Draw. */
void GameScreen::DrawNormalPlay(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("GAME_SCREEN_TITLE"), 395);

	/* Draw Player 1. */
	if (this->Params.Characters[0] < Utils::GetCharSheetSize()) {
		Gui::DrawSprite(GFX::Characters, this->Params.Characters[0], 30, 30);
		Gui::Draw_Rect(37, 159, 106, 20, KD_KEYUNPRESSED);
		Gui::DrawString(40, 162, 0.45f, TEXT_COLOR, this->Params.Names[0], 100);

		Gui::DrawString(37, 190, 0.45f, TEXT_COLOR, Lang::Get("PAIRS") + std::to_string(this->Game->GetPlayerPairs(StackMem::Players::Player1)), 100);
	}

	/* Draw Player 2. */
	if (this->Params.Characters[1] < Utils::GetCharSheetSize()) {
		Gui::DrawSprite(GFX::Characters, this->Params.Characters[1], 250, 30);
		Gui::Draw_Rect(257, 159, 106, 20, KD_KEYUNPRESSED);
		Gui::DrawString(260, 162, 0.45f, TEXT_COLOR, this->Params.Names[1], 100);

		Gui::DrawString(257, 190, 0.45f, TEXT_COLOR, Lang::Get("PAIRS") + std::to_string(this->Game->GetPlayerPairs(StackMem::Players::Player2)), 100);
	}

	/* Draw Current Player. */
	if (this->Game->GetCurrentPlayer() == StackMem::Players::Player1) {
		Gui::DrawStringCentered(0, 205, 0.4f, TEXT_COLOR, Lang::Get("GAME_SCREEN_CURRENT_PLAYER") + this->Params.Names[0], 390);

	} else {
		Gui::DrawStringCentered(0, 205, 0.4f, TEXT_COLOR, Lang::Get("GAME_SCREEN_CURRENT_PLAYER") + this->Params.Names[1], 390);
	}

	if (this->Game->GetPairs() > 10) { // Only 11+ Pairs have pages.
		Gui::DrawStringCentered(0, 215, 0.4f, TEXT_COLOR, Lang::Get("GAME_SCREEN_CURRENT_PAGE") + std::to_string(this->Page + 1) + " / " + std::to_string(((this->Game->GetPairs() * 2) / 20) + 1), 390);
	}

	if (!this->Params.CardDelayUsed || this->Params.CardDelay == 0) { // The checks only exist on non delay mode.
		/* Only display if current State is the check state.. OR the AI's turn. */
		if ((this->Game->GetState() == StackMem::TurnState::DoCheck) || (this->Game->GetCurrentPlayer() == StackMem::Players::Player2 && this->Game->AIEnabled())) {
			Gui::DrawStringCentered(0, 225, 0.4f, TEXT_COLOR, Lang::Get("GAME_SCREEN_ANY_KEY"), 390);

		} else {
			Gui::DrawStringCentered(0, 225, 0.4f, TEXT_COLOR, Lang::Get("GAME_SCREEN_EXIT"), 390); // Cause dunno where else to place it.
		}
	}

	GFX::DrawBottom();

	/* Only draw those, if over 10. */
	if (this->Game->GetPairs() > 10) {
		/* Because we can go back. */
		if (this->Page > 0) {
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0, 0); // Draw the small top corner.
			Gui::Draw_Rect(0, 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0, 220, 1.0f, -1.0f); // Draw the small bottom corner.
			Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 0, 110); // Now the arrow!
		}

		/* Because we can go forward. */
		if (this->CanGoNext()) {
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300, 0, -1.0f, 1.0f); // Draw the small top corner.
			Gui::Draw_Rect(300, 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300, 220, -1.0f, -1.0f); // Draw the small bottom corner.
			Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 300, 110, -1.0f, 1.0f); // Now the arrow!
		}
	}

	/* Draw Field. */
	for (size_t Idx = (this->Page * 20), Idx2 = 0; Idx < (this->Page * 20) + 20 && Idx < (this->Game->GetPairs() * 2); Idx++, Idx2++) {
		if (!this->Game->IsCardCollected(Idx)) { // If the card is not already collected.
			if (this->Game->IsCardShown(Idx)) GFX::DrawCard(this->Game->GetCardType(Idx), this->CPos[Idx2].X, this->CPos[Idx2].Y);
			else Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[Idx2].X, this->CPos[Idx2].Y); // Back cover because hidden.
		}
	}

	Gui::DrawSprite(GFX::Sprites, sprites_grid_idx, 19.5, 7.5);
	Pointer::Draw();
};

/* Main Draw. */
void GameScreen::Draw(void) const {
	if (this->Params.GameMode == GameSettings::GameModes::TryPlay) this->DrawTryPlay(); // Least amount of tries play mode.
	else this->DrawNormalPlay(); // Normal mode.
};

/* Normal Player Logic. */
void GameScreen::PlayerLogic(const uint32_t Down, const uint32_t Held, const touchPosition T) {
	Pointer::ScrollHandling(Held);

	if (Down & KEY_A) {
		for (auto &Position : this->CPos) {
			if (Pointer::Clicked(Position, true)) break;
		}
	}

	if (Down & KEY_TOUCH) {
		for (auto &Position : this->CPos) {
			if (Touched(Position, T, true)) break;
		}
	}
};

/* Normal AI Logic. */
void GameScreen::AILogic(const uint32_t Down) {
	if (this->Params.CardDelayUsed && this->Params.CardDelay > 0) {
		if (this->RefreshFrame) {
			this->RefreshFrame = false;
			return;
		};

		for (size_t Idx = 0; Idx < this->Params.CardDelay; Idx++) { gspWaitForVBlank(); }; // Delay.

		const size_t Card = this->Game->AIPlay();
		this->Page = (Card / 20);
		this->Game->DoPlay(Card);
		this->RefreshFrame = true;

	} else {
		if (Down) {
			const size_t Card = this->Game->AIPlay();
			this->Page = (Card / 20);
			this->Game->DoPlay(Card);
		}
	}
};

/* Normal Play Logic. */
void GameScreen::NormalPlayLogic(const uint32_t Down, const uint32_t Held, const touchPosition T) {
	if (this->Game->GetState() != StackMem::TurnState::DoCheck) { // As long as the State is not check, we can play.
		/* Player 1 is ALWAYS a player. */
		if (this->Game->GetCurrentPlayer() == StackMem::Players::Player1) this->PlayerLogic(Down, Held, T);

		/* Player 2 Logic. */
		else {
			if (this->Game->AIEnabled()) this->AILogic(Down); // It's the AI's turn.
			else this->PlayerLogic(Down, Held, T); // It's the second player's turn.
		}

	} else { // State -> DoCheck.
		if (this->Params.CardDelayUsed && this->Params.CardDelay > 0) { // If we are using the delay way.
			if (this->RefreshFrame) {
				this->RefreshFrame = false;
				return;
			};

			if (this->Params.CardDelayUsed && this->Params.CardDelay > 0) {
				for (uint8_t Idx = 0; Idx < this->Params.CardDelay; Idx++) { gspWaitForVBlank(); }; // Do the delay, if enabled.
			}

			if (this->Game->DoCheck()) {
				/* Check if over. */
				if (this->Game->CheckGameState() != StackMem::GameState::NotOver) {
					const StackMem::GameState Res = this->Game->CheckGameState();

					switch(Res) {
						case StackMem::GameState::NotOver:
							break;

						case StackMem::GameState::Tie:
						case StackMem::GameState::Player1:
						case StackMem::GameState::Player2: // All 3 states are valid end states.
						{
							/* Set Pairs. */
							this->Params.PlayerPairs[0x0] = this->Game->GetPlayerPairs(StackMem::Players::Player1);
							this->Params.PlayerPairs[0x1] = this->Game->GetPlayerPairs(StackMem::Players::Player2);

							std::unique_ptr<GameResult> Ovl = std::make_unique<GameResult>();

							/* Get the proper Overlay. They also set the win count to ++ for the winner. */
							if (Res == StackMem::GameState::Tie) Ovl->Action(this->Params, 0);
							else if (Res == StackMem::GameState::Player1) Ovl->Action(this->Params, 1);
							else if (Res == StackMem::GameState::Player2) Ovl->Action(this->Params, 2);

							/* If we reached the needed win amount, go the screen back to the MainMenu again. */
							if (this->Params.Wins[0] >= this->Params.RoundsToWin || this->Params.Wins[1] >= this->Params.RoundsToWin) {
								Gui::screenBack();
								return;

							} else {
								/* Init a new game. */
								this->Game->InitializeGame(this->Game->GetPairs(), this->Game->AIEnabled(), this->Game->GetDifficulty(), false);

								/* Set the Starting Player. */
								switch(this->Params.Starter) {
									case GameSettings::RoundStarter::Player1:
										this->Game->SetCurrentPlayer(StackMem::Players::Player1);
										break;

									case GameSettings::RoundStarter::Player2:
										this->Game->SetCurrentPlayer(StackMem::Players::Player2);
										break;

									case GameSettings::RoundStarter::Random:
										this->Game->SelectRandomPlayer();
										break;
								};

								this->RefreshFrame = true;
							}
						}
						break;
					};

				} else {
					this->RefreshFrame = true;
					this->Game->SetState(StackMem::TurnState::DrawFirst); // We were able to play a card, so let us continue!
				}

			} else {
				this->RefreshFrame = true;
				this->Game->NextPlayer(); // Nah, no match. Next player!
			}

		} else { // At this point, we'd expect an input.
			if (Down) {
				if (this->Game->DoCheck()) {
					/* Check if over. */
					if (this->Game->CheckGameState() != StackMem::GameState::NotOver) {
						const StackMem::GameState Res = this->Game->CheckGameState();

						switch(Res) {
							case StackMem::GameState::NotOver:
								break;

							case StackMem::GameState::Tie:
							case StackMem::GameState::Player1:
							case StackMem::GameState::Player2: // All 3 states are valid end states.
							{
								/* Set Pairs. */
								this->Params.PlayerPairs[0x0] = this->Game->GetPlayerPairs(StackMem::Players::Player1);
								this->Params.PlayerPairs[0x1] = this->Game->GetPlayerPairs(StackMem::Players::Player2);

								std::unique_ptr<GameResult> Ovl = std::make_unique<GameResult>();

								/* Get the proper Overlay. They also set the win count to ++ for the winner. */
								if (Res == StackMem::GameState::Tie) Ovl->Action(this->Params, 0);
								else if (Res == StackMem::GameState::Player1) Ovl->Action(this->Params, 1);
								else if (Res == StackMem::GameState::Player2) Ovl->Action(this->Params, 2);

								/* If we reached the needed win amount, go the screen back to the MainMenu again. */
								if (this->Params.Wins[0] >= this->Params.RoundsToWin || this->Params.Wins[1] >= this->Params.RoundsToWin) {
									Gui::screenBack();
									return;

								} else {
									/* Init a new game. */
									this->Game->InitializeGame(this->Game->GetPairs(), this->Game->AIEnabled(), this->Game->GetDifficulty(), false);

									/* Set the Starting Player. */
									switch(this->Params.Starter) {
										case GameSettings::RoundStarter::Player1:
											this->Game->SetCurrentPlayer(StackMem::Players::Player1);
											break;

										case GameSettings::RoundStarter::Player2:
											this->Game->SetCurrentPlayer(StackMem::Players::Player2);
											break;

										case GameSettings::RoundStarter::Random:
											this->Game->SelectRandomPlayer();
											break;
									};

								}
							}
							break;
						};

					} else {
						this->Game->SetState(StackMem::TurnState::DrawFirst); // We were able to play a card, so let us continue!
					}

				} else {
					this->Game->NextPlayer(); // Nah, no match. Next player!
				}
			}
		}
	}
};

/* Least amount of tries Play Logic. */
void GameScreen::TryPlayLogic(const uint32_t Down, const uint32_t Held, const touchPosition T) {
	Pointer::ScrollHandling(Held);

	if (this->Game->GetState() != StackMem::TurnState::DoCheck) { // As long as the State is not check, we can play.
		if (Down & KEY_A) {
			for (auto &Position : this->CPos) {
				if (Pointer::Clicked(Position, true)) break;
			}
		}

		if (Down & KEY_TOUCH) {
			for (auto &Position : this->CPos) {
				if (Touched(Position, T, true)) break;
			}
		}

	} else {
		if (this->Params.CardDelayUsed && this->Params.CardDelay > 0) {
			if (this->RefreshFrame) {
				this->RefreshFrame = false;
				return;
			};

			if (this->Params.CardDelayUsed && this->Params.CardDelay > 0) {
				for (size_t Idx = 0; Idx < this->Params.CardDelay; Idx++) { gspWaitForVBlank(); }; // Do the delay, if enabled.
			}

			if (this->Game->DoCheck()) {
				/* Check if over. */
				if (this->Game->CheckGameState() != StackMem::GameState::NotOver) {
					std::unique_ptr<GameResult> Ovl = std::make_unique<GameResult>();
					Ovl->Action(this->Params, 0);
					Gui::screenBack();
					return;

				} else {
					this->RefreshFrame = true;
					this->Game->SetState(StackMem::TurnState::DrawFirst); // Set first State again.
				}

			} else {
				this->Params.Tries++; // Increase the Tries.
				this->RefreshFrame = true;
				this->Game->SetState(StackMem::TurnState::DrawFirst); // Set first State again.
			}

		} else { // At this point, we'd expect an input.
			if (Down) {
				if (this->Game->DoCheck()) {
					/* Check if over. */
					if (this->Game->CheckGameState() != StackMem::GameState::NotOver) {
						std::unique_ptr<GameResult> Ovl = std::make_unique<GameResult>();
						Ovl->Action(this->Params, 0);
						Gui::screenBack();
						return;

					} else {
						this->RefreshFrame = true;
						this->Game->SetState(StackMem::TurnState::DrawFirst); // Set first State again.
					}

				} else {
					this->Params.Tries++; // Increase the Tries.
					this->RefreshFrame = true;
					this->Game->SetState(StackMem::TurnState::DrawFirst); // Set first State again.
				}
			}
		}
	}
};


void GameScreen::Logic(uint32_t Down, uint32_t Held, touchPosition T) {
	if (Held & KEY_START && Held & KEY_SELECT) { // START + SELECT -> Cancel because why not.
		this->Params.CancelGame = true;
		Gui::screenBack();
		return;
	}

	if (this->Params.GameMode == GameSettings::GameModes::TryPlay) this->TryPlayLogic(Down, Held, T); // Least amount of tries mode.
	else this->NormalPlayLogic(Down, Held, T); // Normal Mode.
};