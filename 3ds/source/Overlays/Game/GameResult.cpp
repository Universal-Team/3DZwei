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


GameResult::GameResult() {
	if (!_3DZwei::CFG->DoAnimation()) {
		this->DoSwipe = false, this->Delay = 0;
		this->InitialScroll = false, this->ScrollIdx = 0; // No animation.
	}
};


/*
	Draws a card background on the top screen, when the game is over.

	const size_t Page: The page to draw. (12 cards per page).
	const int AddOffs: The additional offsets to draw to.
*/
void GameResult::DrawCardBG(const size_t Page, const int AddOffs) {
	for (size_t Idx = (Page * 12), Idx2 = 0; Idx < (Page * 12) + 12 && Idx < Utils::Cards.size(); Idx++, Idx2++) {
		GFX::DrawCard(Idx, this->InitialScrollPos[Idx2].X, this->InitialScrollPos[Idx2].Y + AddOffs);
	};
};


/*
	Draw this, when the game is fully over aka rounds to win match the full win.

	const bool P2Wins: If P2 wins (true) or not (false).
	const GameSettings::GameParams &Params: The parameters of the game.
*/
void GameResult::DrawOver(const bool P2Wins, const GameSettings::GameParams &Params) {
	float topDelay = this->Delay * 1.1f; // Speed up delay on top so character makes it off screen.

	char Buffer[100]; snprintf(Buffer, sizeof(Buffer), Lang::Get("WON").c_str(), Params.Names[P2Wins].c_str());
	Gui::DrawStringCentered(0 + topDelay, 28, 0.6f, TEXT_WHITE, Buffer, 380);

	/* Draw Winner. */
	if (Params.Characters[P2Wins] < Utils::GetCharSheetSize()) {

		/* Draw Winner + Outline. */
		Gui::DrawSprite(GFX::Characters, Params.Characters[P2Wins], 141 + topDelay, 51);
		Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 139 + topDelay, 49);

		/* Name bar. */
		Gui::Draw_Rect(148 + topDelay, 180, 106, 20, BAR_BLUE);
		Gui::DrawString(151 + topDelay, 183, 0.45f, TEXT_WHITE, Params.Names[P2Wins], 100);

		/* Wins. */
		Gui::DrawString(148 + topDelay, 205, 0.45f, TEXT_WHITE, Lang::Get("PAIRS") + std::to_string(Params.PlayerPairs[P2Wins]), 200);
		Gui::DrawString(148 + topDelay, 225, 0.45f, TEXT_WHITE, Lang::Get("WINS") + std::to_string(Params.Wins[P2Wins]), 200);
	};

	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->Delay > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->Delay));
	};

	GFX::DrawBottom();

	/* Draw Loser. */
	if (Params.Characters[!P2Wins] < Utils::GetCharSheetSize()) {
		/* Draw Loser. */
		Gui::DrawSprite(GFX::Characters, Params.Characters[!P2Wins], 96 - this->Delay, 31);

		/* Name bar. */
		Gui::Draw_Rect(103 - this->Delay, 160, 106, 20, BAR_BLUE);
		Gui::DrawString(107 - this->Delay, 163, 0.45f, TEXT_WHITE, Params.Names[!P2Wins], 100);

		/* Wins. */
		Gui::DrawString(103 - this->Delay, 185, 0.45f, TEXT_WHITE, Lang::Get("PAIRS") + std::to_string(Params.PlayerPairs[!P2Wins]), 150);
		Gui::DrawString(103 - this->Delay, 205, 0.45f, TEXT_WHITE, Lang::Get("WINS") + std::to_string(Params.Wins[!P2Wins]), 150);
	};

	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->Delay > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->Delay));
	};
};

/*
	Draws the versus results.
	Winner on top, loser on bottom, like the final / over result.

	const bool P2Wins: If P2 wins (true) or not (false).
	const GameSettings::GameParams &Params: The parameters of the game.
	const bool Tie: If it's a Tie or not.
*/
void GameResult::DrawVersus(const bool P2Wins, const GameSettings::GameParams &Params, const bool Tie) {
	/* Draw Winner. */
	if (Params.Characters[P2Wins] < Utils::GetCharSheetSize()) {
		float topDelay = this->Delay * 1.1f; // Speed up delay on top so character makes it off screen.

		/* Draw Winner + Outline. */
		Gui::DrawSprite(GFX::Characters, Params.Characters[P2Wins], 141 + topDelay, 51);
		Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 139 + topDelay, 49);

		/* Name bar. */
		Gui::Draw_Rect(148 + topDelay, 180, 106, 20, BAR_BLUE);
		Gui::DrawString(151 + topDelay, 183, 0.45f, TEXT_WHITE, Params.Names[P2Wins], 100);

		/* Wins. */
		Gui::DrawString(148 + topDelay, 205, 0.45f, TEXT_WHITE, Lang::Get("PAIRS") + std::to_string(Params.PlayerPairs[P2Wins]), 200);
		Gui::DrawString(148 + topDelay, 225, 0.45f, TEXT_WHITE, Lang::Get("WINS") + std::to_string(Params.Wins[P2Wins]), 200);
	};

	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->Delay > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->Delay));
	};

	GFX::DrawBottom();

	/* Draw Loser. */
	if (Params.Characters[!P2Wins] < Utils::GetCharSheetSize()) {
		/* Draw Loser. */
		Gui::DrawSprite(GFX::Characters, Params.Characters[!P2Wins], 96 - this->Delay, 21);

		/* A Tie! */
		if (Tie) {
			if (Params.Wins[0] == Params.Wins[1]) {
				Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 94 - this->Delay, 19);
			}
		}

		/* Name bar. */
		Gui::Draw_Rect(103 - this->Delay, 150, 106, 20, BAR_BLUE);
		Gui::DrawString(107 - this->Delay, 153, 0.45f, TEXT_WHITE, Params.Names[!P2Wins], 100);

		/* Wins. */
		Gui::DrawString(103 - this->Delay, 175, 0.45f, TEXT_WHITE, Lang::Get("PAIRS") + std::to_string(Params.PlayerPairs[!P2Wins]), 150);
		Gui::DrawString(103 - this->Delay, 195, 0.45f, TEXT_WHITE, Lang::Get("WINS") + std::to_string(Params.Wins[!P2Wins]), 150);

		Gui::DrawStringCentered(0 + this->Delay, 215, 0.5f, TEXT_WHITE, Lang::Get("WINS_WIN") + std::to_string(Params.RoundsToWin), 310);
	};

	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->Delay > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->Delay));
	};
};

/*
	Draws the Solo play mode.

	const GameSettings::GameParams &Params: The parameters of the game.
*/
void GameResult::DrawSolo(const GameSettings::GameParams &Params) {
	/* Draw Player 1. */
	if (Params.Characters[0] < Utils::GetCharSheetSize()) {
		Gui::DrawSprite(GFX::Characters, Params.Characters[0], 30 - this->Delay, 30);
		Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 28 - this->Delay, 28);
		Gui::Draw_Rect(37 - this->Delay, 159, 106, 20, BAR_BLUE);
		Gui::DrawString(40 - this->Delay, 162, 0.45f, TEXT_WHITE, Params.Names[0], 100);
	}

	Gui::DrawString(220, 80 - this->Delay, 0.45f, TEXT_WHITE, Lang::Get("PAIRS") + std::to_string(Params.PlayerPairs[0]), 150);
	Gui::DrawString(220, 100 + this->Delay, 0.45f, TEXT_WHITE, Lang::Get("YOUR_TRIES") + std::to_string(Params.Tries), 100);

	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->Delay > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->Delay));
	};

	GFX::DrawBottom();
	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->Delay > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->Delay));
	};
};


/*
	Swipes the characters in and fades the screen in when initializing this overlay.
	Swipes the characters out and fades the screen out when exiting this overlay.

	This overlay just shows the results of the played game.
*/
void GameResult::Action(GameSettings::GameParams &Params, const uint8_t Won) {
	/* Increase Wins on Versus Mode.. */
	if (Params.GameMode == GameSettings::GameModes::Versus) {
		if (Won == 1) Params.Wins[0]++;
		else if (Won == 2) Params.Wins[1]++;
	};

	/* Get if the game is fully over. */
	this->Over = ((Params.GameMode == GameSettings::GameModes::Versus) && (Params.Wins[0] == Params.RoundsToWin || Params.Wins[1] == Params.RoundsToWin));
	this->ScrollMode = (this->Over && Utils::Cards.size() > 12);

	while(aptMainLoop() && !this->FullDone) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		if (this->Over) {
			Gui::ScreenDraw(Top);
			Gui::Draw_Rect(0, 25, 400, 215, BG_BLUE); // Draw BG.
			/* Then the cards. */
			if (this->DoScrollSwipe || this->InitialScroll) {
				if (!this->InitialScroll) {
					if ((this->ScrollPage * 12) + 12 < Utils::Cards.size()) {
						this->DrawCardBG(this->ScrollPage + 1, this->ScrollIdx - 240);

					} else {
						this->DrawCardBG(0, this->ScrollIdx - 240);
					}
				}

				this->DrawCardBG(this->ScrollPage, this->ScrollIdx);

			} else {
				this->DrawCardBG(this->ScrollPage, 0);
			}

			Gui::Draw_Rect(0, 0, 400, 25, BAR_BLUE); // Then the Bar, so the cards don't go over it.
			Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190)); // Then the darken.
			Gui::DrawStringCentered(0, 3, 0.6f, TEXT_WHITE, Lang::Get("GAME_RESULT_TITLE"), 395);
			this->DrawOver(Params.Wins[0] < Params.Wins[1], Params); // Game is fully over.

		} else {
			GFX::DrawTop();
			Gui::DrawStringCentered(0, 3, 0.6f, TEXT_WHITE, Lang::Get("GAME_RESULT_ROUND"), 395);

			if (Params.GameMode == GameSettings::GameModes::Solo) this->DrawSolo(Params); // Solo Mode.
			else {
				/* Versus Mode. */
				if (Won == 0) this->DrawVersus(Params.Wins[1] > Params.Wins[0], Params, true); // Tie, so display most winner on top.
				else this->DrawVersus(Won == 2, Params, false); // There exist a real winner.
			}
		}

		C3D_FrameEnd(0);
		this->Handler();
	};

	/* Reset Pairs. */
	if (Params.GameMode == GameSettings::GameModes::Versus) {
		for (uint8_t Idx = 0; Idx < 2; Idx++) Params.PlayerPairs[Idx] = 0;
	};
};


/* The Handler. */
void GameResult::Handler() {
	hidScanInput();
	const uint32_t Down = hidKeysDown();

	if (Down & KEY_A || Down & KEY_TOUCH) {
		if (!_3DZwei::CFG->DoAnimation()) this->FullDone = true; // No animation -> Directly go to FullDone.
		else {
			if (!this->Done) this->Done = true;
			else this->FullDone = true;
			this->DoSwipe = true;
		}
	};

	/* Fade and Move Logic. */
	if (!this->Done && this->DoSwipe) {
		if (this->Cubic < 255.0f) {
			this->Cubic = std::lerp(this->Cubic, 256.0f, 0.1f);
			this->Delay = 255 - this->Cubic;

			if (this->Cubic >= 255.0f) {
				this->Delay = 0;
				this->Cubic = 0;
				this->DoSwipe = false;
			}
		}
	};

	if (this->Done && this->DoSwipe) {
		if (this->Cubic < 255.0f) {
			this->Cubic = std::lerp(this->Cubic, 256.0f, 0.1f);
			this->Delay = this->Cubic;

			if (this->Cubic >= 255.0f) this->Delay = 0, this->Cubic = 0, this->DoSwipe = false, this->FullDone = true;
		}
	};

	if (this->Over) {
		if (this->InitialScroll) {
			if (this->ScrollCubic < 240.0f) {
				this->ScrollCubic = std::lerp(this->ScrollCubic, 241.0f, 0.1f);
				this->ScrollIdx = (-240) + this->ScrollCubic;

				if (this->ScrollCubic >= 240.0f) {
					this->ScrollCubic = 0.0f, this->ScrollIdx = 0;
					this->InitialScroll = false;
				}
			}

			return;
		};

		if (this->ScrollMode) { // Only allow scrolls, if 13+ cards.
			if (!_3DZwei::CFG->DoAnimation()) return;

			if (this->DoScrollSwipe) {
				if (this->ScrollCubic < 240.0f) {
					this->ScrollCubic = std::lerp(this->ScrollCubic, 241.0f, 0.1f);
					this->ScrollIdx = this->ScrollCubic;

					if (this->ScrollCubic >= 240.0f) {
						this->ScrollCubic = 0.0f, this->ScrollIdx = 0;
						this->ScrollPage = (((this->ScrollPage * 12) + 12 < Utils::Cards.size()) ? this->ScrollPage + 1 : 0);
						this->DoScrollSwipe = false;
					}
				}

				return;
			};

			if (this->ScrollDelay > 0) {
				this->ScrollDelay--;

				if (this->ScrollDelay == 0) this->DoScrollSwipe = true, this->ScrollDelay = 60;
			};
		};
	};
};