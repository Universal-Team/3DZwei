/*
*   This file is part of 3DZwei
*   Copyright (C) 2020 Universal-Team
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
#include "keyboard.hpp"
#include "timePlay.hpp"
#include "utils.hpp"

extern std::unique_ptr<Config> config;
extern bool cardTouch(touchPosition touch, CardStr card);
extern C2D_SpriteSheet cards; // Needed for getting size the spritesheet.

TimePlay::TimePlay() {
	this->useDelay = Msg::promptMsg(Lang::get("PLAY_WITH_DELAY"));
	this->seconds = 0; this->minutes = 0; this->hours = 0; this->trys = 0;

	char msg[100];
	snprintf(msg, sizeof(msg), Lang::get("ENTER_PAIR_AMOUNT").c_str(), C2D_SpriteSheetCount(cards)-1);

	/* Pair Selection here. */
	int amount = Keyboard::setInt(C2D_SpriteSheetCount(cards) - 1, msg);
	if (amount > 0) {
		this->pairAmount = amount;
	} else {
		this->pairAmount = C2D_SpriteSheetCount(cards)-1;
	}

	this->currentGame = std::make_unique<Game>(this->pairAmount, false, false); // Create game.
	this->delay = config->delay();
}

/* Format time. */
std::string TimePlay::formatTime() const {
	return Utils::formatText("%s%02i:%02i:%02i", Lang::get("ELAPSED_TIME").c_str(), this->hours, this->minutes, this->seconds);
}

/* Time ticking logic. */
void TimePlay::doTime() {
	if (this->ticking) {
		this->millisecs++;

		/* Do Seconds. */
		if (this->millisecs > 59) {
			this->seconds++;
			this->millisecs = 0;

			/* Do minutes. */
			if (this->seconds > 59) {
				this->minutes++;
				this->seconds = 0;
				this->millisecs = 0;
				
				/*  Do Hours. I doubt it'll be reached tho. */
				if (this->minutes > 59) {
					this->hours++;
					this->minutes = 0;
					this->seconds = 0;
					this->millisecs = 0;
				}
			}
		}
	}
}


void TimePlay::Draw(void) const {
	const std::string temp = std::to_string(this->page + 1) + " | " + std::to_string(((this->pairAmount / (10 + 1)) + 1));
	GFX::DrawGameBG(true);
	Gui::DrawStringCentered(0, -2, 0.8f, config->textColor(), "3DZwei - " + Lang::get("PLAY_WITH_TIME"), 390);
	Gui::DrawStringCentered(0, 30, 0.6f, config->textColor(), Lang::get("CARDPAIRS") + std::to_string(this->currentGame->getPairs()));
	Gui::DrawString(397-Gui::GetStringWidth(0.6f, temp), 239-Gui::GetStringHeight(0.6f, temp), 0.6f, config->textColor(), temp);

	Gui::DrawStringCentered(0, 80, 0.6f, config->textColor(), this->formatTime());
	Gui::DrawStringCentered(0, 140, 0.6f, config->textColor(), Lang::get("TRIES") + std::to_string(this->trys));

	/* For no delay mode, show that you have to press X to do the play check. */
	if (this->currentGame->getCardSelect() == CardSelectMode::DoCheck && !this->useDelay) {
		Gui::DrawStringCentered(0, 185, 0.6, config->textColor(), Lang::get("X_GAME_CHECK"), 390);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawGameBG(false);

	for (int i = 0 + (this->page * 20), i2 = 0; (i < this->currentGame->getPairs() * 2) && (i < (0 + (this->page * 20) + 20)); i++, i2++) {
		if (this->currentGame->returnIfShown(i)) {
			if (!this->currentGame->isCollected(i)) {
				GFX::DrawCard(this->currentGame->getCard(i), cardPos[i2].X, cardPos[i2].Y);
			}

		} else {
			GFX::DrawCard(-1, cardPos[i2].X, cardPos[i2].Y);
		}
	}

	GFX::DrawGrid(19.5, 7.5);
	/* Only show pointer, if not on check. */
	if (this->currentGame->getCardSelect() != CardSelectMode::DoCheck) {
		GFX::DrawSprite(sprites_pointer_idx, cardPos[this->selectedCard].X + 15.5, cardPos[this->selectedCard].Y + 17.5);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void TimePlay::playerLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->currentGame->getCardSelect() == CardSelectMode::DrawFirst || this->currentGame->getCardSelect() == CardSelectMode::DrawSecond) {

		if (hDown & KEY_RIGHT) {
			/* In case we'd reach the last card from the page. */
			if (this->selectedCard == 4 || this->selectedCard == 9 || this->selectedCard == 14 || this->selectedCard == 19) {
				if (this->page < (this->currentGame->getPairs() / (10 + 1))) {
					this->page++;
					this->selectedCard = (this->selectedCard - 4);
				}

			} else if (this->selectedCard < 19) this->selectedCard++;
		}

		if (hDown & KEY_LEFT) {
			/* In case we'd reach the first card from the page. */
			if (this->selectedCard == 15 || this->selectedCard == 10 || this->selectedCard == 5 || this->selectedCard == 0) {
				if (this->page > 0) {
					this->page--;
					this->selectedCard = (this->selectedCard + 4);
				}

			} else if (this->selectedCard > 0) this->selectedCard--;
		}

		if (hDown & KEY_DOWN) {
			if (this->selectedCard < 15) this->selectedCard += 5;
		}

		if (hDown & KEY_UP) {
			if (this->selectedCard > 4)	this->selectedCard -=5;
		}

		if (hDown & KEY_R) {
			if (this->currentGame->getPairs() > 10) {
				if (this->page < (this->currentGame->getPairs() / (10 + 1))) this->page++;
			}
		}

		if (hDown & KEY_L) {
			if (this->page > 0) this->page--;
		}

		if (hDown & KEY_A) {
			if (!this->currentGame->returnIfShown(0 + (this->page * 20) + this->selectedCard)) {
				this->currentGame->play(0 + (this->page * 20) + this->selectedCard);
			}
		}

		if (hDown & KEY_TOUCH) {
			for (int i = 0; i < 20; i++) {
				if (cardTouch(touch, cardPos[i])) {
					if (!this->currentGame->returnIfShown(i + (this->page * 20))) {
						this->selectedCard = i;
						this->currentGame->play(i + (this->page * 20));
					}
				}
			}
		}
	}
}

void TimePlay::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	this->doTime();

	if (hDown & KEY_START) {
		if (Msg::promptMsg(Lang::get("EXIT_GAME"))) {
			Gui::screenBack(true);
			return;
		}
	}

	this->playerLogic(hDown, hHeld, touch);

	if (this->currentGame->getCardSelect() == CardSelectMode::DoCheck) {
		if (!this->useDelay) {
			if (hDown & KEY_X) {
				if (this->currentGame->setCardPair()) {
					/* Check if over. */
					if (this->currentGame->checkOver() != GameWinner::NotOver) {

						const GameWinner checkOver = this->currentGame->checkOver();

						switch(checkOver) {
							case GameWinner::Player1:
								Msg::DisplayWaitMsg(Lang::get("YOUR_RESULT") + this->formatTime() + "\n" + Lang::get("TRIES") + std::to_string(this->trys));
								break;
							case GameWinner::Player2:
								break;
							case GameWinner::None:
								break;
							case GameWinner::NotOver:
								break;
						}

						Gui::screenBack(true);
						return;
					}
				} else {
					/* No match, so do ++ tries and + 2 seconds. */
					this->trys++;
					this->seconds += 2;
				}

				this->currentGame->setCardSelect(CardSelectMode::DrawFirst);
			}
		} else {
			if (this->delay > 0) {
				this->delay--;
				if (this->delay < 1) {
					if (this->currentGame->setCardPair()) {
						/* Check if over. */
						if (this->currentGame->checkOver() != GameWinner::NotOver) {
							const GameWinner checkOver = this->currentGame->checkOver();

							switch(checkOver) {
								case GameWinner::Player1:
									Msg::DisplayWaitMsg(Lang::get("YOUR_RESULT") + this->formatTime() + "\n" + Lang::get("TRIES") + std::to_string(this->trys));
									break;
								case GameWinner::Player2:
									break;
								case GameWinner::None:
									break;
								case GameWinner::NotOver:
									break;
							}

							Gui::screenBack(true);
							return;
						}
					} else {
						/* No match, so do ++ tries and + 2 seconds. */
						this->trys++;
						this->seconds += 2;
					}

					this->currentGame->setCardSelect(CardSelectMode::DrawFirst);
					this->delay = config->delay();
				}
			}
		}
	}
}