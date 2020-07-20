/*
*   This file is part of 3DZwei
*   Copyright (C) 2020 SuperSaiyajinStackZ
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

#include "config.hpp"
#include "gameScreen.hpp"
#include "keyboard.hpp"
#include "overlay.hpp"

extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

GameScreen::GameScreen(bool useDelay, bool useAI, bool doBetterPredict) {
	this->useDelay = useDelay;
	this->useAI = useAI;
	this->betterPredict = doBetterPredict;
	this->currentGame = std::make_unique<Game>(10, this->useAI, this->betterPredict); // Create game.
	this->delay = config->delay();

	this->avatar1 = Overlays::SelectAvatar(1);
	this->avatar2 = Overlays::SelectAvatar(2);
	int tempWins = Keyboard::setInt(99, Lang::get("ENTER_WINS"));
	if (tempWins != -1 || tempWins != 0) {
		this->wins = tempWins;
	} else {
		this->wins = 3;
	}
}


void GameScreen::Draw(void) const {
	GFX::DrawTop(true);
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), "3DZwei - " + Lang::get("GAME_SCREEN"), 390);
	Gui::DrawStringCentered(0, 30, 0.6f, config->textColor(), Lang::get("CARDPAIRS") + std::to_string(this->currentGame->getPairs()));

	// Player 1.
	GFX::DrawChar(this->avatar1, 10, 35);
	Gui::DrawString(16, 170, 0.6f, config->textColor(), Lang::get("PAIRS") + std::to_string(this->currentGame->getPairs(Players::Player1)), 110);
	// Player 2.
	GFX::DrawChar(this->avatar2, 280, 35);
	Gui::DrawString(286, 170, 0.6f, config->textColor(), Lang::get("PAIRS") + std::to_string(this->currentGame->getPairs(Players::Player2)), 110);

	Gui::DrawStringCentered(0, 215, 0.8f, config->textColor(), Lang::get("CURRENT_PLAYER") + std::to_string((int)this->currentGame->getCurrentPlayer()+1), 390);

	// For no delay mode, show that you have to press Y to do the play.
	if ((this->currentGame->getCardSelect() == CardSelectMode::DrawFirst || this->currentGame->getCardSelect() == CardSelectMode::DrawSecond) && (!this->useDelay && this->useAI && this->currentGame->getCurrentPlayer() == Players::Player2)) {
		Gui::DrawStringCentered(0, 185, 0.6, config->textColor(), Lang::get("Y_PLAY"), 390);
	}

	// For no delay mode, show that you have to press X to do the play check.
	if (this->currentGame->getCardSelect() == CardSelectMode::DoCheck && !this->useDelay) {
		Gui::DrawStringCentered(0, 185, 0.6, config->textColor(), Lang::get("X_GAME_CHECK"), 390);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom(false);

	for (int i = 0; i < 20; i++) {
		if (this->currentGame->returnIfShown(i)) {
			GFX::DrawCard(this->currentGame->getCard(i), cardPos[i].x, cardPos[i].y);
		} else {
			GFX::DrawCard(PairType::None, cardPos[i].x, cardPos[i].y);
		}
	}

	GFX::DrawCardSelector(cardPos[this->selectedCard].x, cardPos[this->selectedCard].y);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void GameScreen::playerLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->currentGame->getCardSelect() == CardSelectMode::DrawFirst || this->currentGame->getCardSelect() == CardSelectMode::DrawSecond) {

		if (hDown & KEY_RIGHT) {
			if (this->selectedCard < 19) this->selectedCard++;
		}

		if (hDown & KEY_LEFT) {
			if (this->selectedCard > 0)	this->selectedCard--;
		}

		if (hDown & KEY_DOWN) {
			if (this->selectedCard < 15) this->selectedCard += 5;
		}

		if (hDown & KEY_UP) {
			if (this->selectedCard > 4)	this->selectedCard -=5;
		}

		if (hDown & KEY_A) {
			if (!this->currentGame->returnIfUsed(this->selectedCard)) {
				this->currentGame->play(this->selectedCard);
			}
		}

		if (hDown & KEY_TOUCH) {
			for (int i = 0; i < 20; i++) {
				if (touching(touch, cardPos[i])) {
					if (!this->currentGame->returnIfUsed(i)) {
						this->selectedCard = i;
						this->currentGame->play(i);
					}
				}
			}
		}
	}
}

void GameScreen::AILogic(u32 hDown) {
	if (this->useAI) {
		if (this->currentGame->getCardSelect() == CardSelectMode::DrawFirst || this->currentGame->getCardSelect() == CardSelectMode::DrawSecond) {
			if (!this->useDelay) {
				if (hDown & KEY_Y) {
					if (this->currentGame->getCardSelect() == CardSelectMode::DrawFirst) {
						Msg::DebugMessage("Do int card1 = this->currentGame->doAITurn();.");
						int card1 = this->currentGame->doAITurn();
						this->currentGame->play(card1);
					} else if (this->currentGame->getCardSelect() == CardSelectMode::DrawSecond) {
						Msg::DebugMessage("Do int card2 = this->currentGame->doAITurn(true);.");
						int card2 = this->currentGame->doAITurn(true); // We do our prediction play here. ;D
						this->currentGame->play(card2);
					}
				}
			} else {
				if (this->delay > 0) {
					this->delay--;
					if (this->delay < 1) {
						if (this->currentGame->getCardSelect() == CardSelectMode::DrawFirst) {
							Msg::DebugMessage("Do int card1 = this->currentGame->doAITurn();.");
							int card1 = this->currentGame->doAITurn();
							this->currentGame->play(card1);
							this->delay = config->delay();
						} else if (this->currentGame->getCardSelect() == CardSelectMode::DrawSecond) {
							Msg::DebugMessage("Do int card2 = this->currentGame->doAITurn(true);.");
							int card2 = this->currentGame->doAITurn(true); // We do our prediction play here. ;D
							this->currentGame->play(card2);
							this->delay = config->delay();
						}
					}
				}
			}
		}
	}
}

void GameScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_START) {
		if (Msg::promptMsg(Lang::get("EXIT_GAME"))) {
			Gui::screenBack(true);
			return;
		}
	}

	if (this->currentGame->getCurrentPlayer() == Players::Player1) {
		this->playerLogic(hDown, hHeld, touch);
	} else {
		if (this->useAI) {
			this->AILogic(hDown);
		} else {
			this->playerLogic(hDown, hHeld, touch);
		}
	}

	if (this->currentGame->getCardSelect() == CardSelectMode::DoCheck) {
		if (!this->useDelay) {
			if (hDown & KEY_X) {
				if (this->currentGame->setCardPair()) {
					// Check if over.
					if (this->currentGame->checkOver() != GameWinner::NotOver) {

						const GameWinner checkOver = this->currentGame->checkOver();

						char msg[100];
						switch(checkOver) {
							case GameWinner::Player1:
								this->currentGame->setWins(Players::Player1, this->currentGame->getWins(Players::Player1)+1);
								snprintf(msg, sizeof(msg), Lang::get("PLAYER_1_WINS").c_str(), this->currentGame->getPairs(Players::Player1));
								Msg::DisplayWarnMsg(msg);
								break;
							case GameWinner::Player2:
								this->currentGame->setWins(Players::Player2, this->currentGame->getWins(Players::Player2)+1);
								snprintf(msg, sizeof(msg), Lang::get("PLAYER_2_WINS").c_str(), this->currentGame->getPairs(Players::Player2));
								Msg::DisplayWarnMsg(msg);
								break;
							case GameWinner::None:
								Msg::DisplayWarnMsg(Lang::get("NO_ONE_WINS"));
								break;
							case GameWinner::NotOver:
								break;
						}

						if (Overlays::ResultOverlay(this->currentGame, this->wins, this->avatar1, this->avatar2)) {
							Gui::screenBack(true);
							return;
						} else {
							this->currentGame->restart(); // Restart!
							return;
						}
					}

					this->currentGame->setCardSelect(CardSelectMode::DrawFirst);
				} else {
					this->currentGame->nextPlayer();
					this->currentGame->setCardSelect(CardSelectMode::DrawFirst);
				}
			}
		} else {
			if (this->delay > 0) {
				this->delay--;
				if (this->delay < 1) {
					if (this->currentGame->setCardPair()) {
						// Check if over.
						if (this->currentGame->checkOver() != GameWinner::NotOver) {

							const GameWinner checkOver = this->currentGame->checkOver();

							char msg[100];
							switch(checkOver) {
								case GameWinner::Player1:
									this->currentGame->setWins(Players::Player1, this->currentGame->getWins(Players::Player1)+1);
									snprintf(msg, sizeof(msg), Lang::get("PLAYER_1_WINS").c_str(), this->currentGame->getPairs(Players::Player1));
									Msg::DisplayWarnMsg(msg);
									break;
								case GameWinner::Player2:
									this->currentGame->setWins(Players::Player2, this->currentGame->getWins(Players::Player2)+1);
									snprintf(msg, sizeof(msg), Lang::get("PLAYER_2_WINS").c_str(), this->currentGame->getPairs(Players::Player2));
									Msg::DisplayWarnMsg(msg);
									break;
								case GameWinner::None:
									Msg::DisplayWarnMsg(Lang::get("NO_ONE_WINS"));
									break;
								case GameWinner::NotOver:
									break;
							}

							if (Overlays::ResultOverlay(this->currentGame, this->wins, this->avatar1, this->avatar2)) {
								Gui::screenBack(true);
								return;
							} else {
								this->currentGame->restart(); // Restart!
								this->delay = config->delay();
								return;
							}
						}

						this->currentGame->setCardSelect(CardSelectMode::DrawFirst);
						this->delay = config->delay();
					} else {
						this->currentGame->nextPlayer();
						this->currentGame->setCardSelect(CardSelectMode::DrawFirst);
						this->delay = config->delay();
					}
				}
			}
		}
	}
}