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
#include "GameHelper.hpp"
#include "Utils.hpp"

#define START_ANIMATION_AMOUNT 2


/*
	Constructor of the GameHelper class.

	Expects a GameSettings::GameParams to start with.
*/
GameHelper::GameHelper(const GameSettings::GameParams Params) { this->StartGame(false, Params, true); };


/*
	Starts a game, like the function name says.

	const bool AlreadyInitialized: If the game is already initialized or not.
	const GameSettings::GameParams &Params: The Game Parameter to pass, in case the game wasn't initialized.
	const bool P2Won: If Player 2 won or not.
*/
void GameHelper::StartGame(const bool AlreadyInitialized, const GameSettings::GameParams Params, const bool P2Won) {
	if (AlreadyInitialized) { // There's no need to initialize from scratch, so just re-init from the current Game Parameters.
		this->Game->InitializeGame(this->Game->GetPairs(), this->Game->AIEnabled(), this->Game->GetMethod(), false);

	} else { // We need to do this way, because the game hasn't been initialized before.
		this->Params = Params;
		this->Game = std::make_unique<StackMem>(Utils::Cards.size(), this->Params.AIUsed, this->Params.Method);
	}

	/* Set the Game Starter, if using Normal-Play mode. */
	if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) {
		switch(this->Params.Starter) { // Get the starter.
			case GameSettings::RoundStarter::Player1: // Player 1.
				this->Game->SetCurrentPlayer(StackMem::Players::Player1);
				break;

			case GameSettings::RoundStarter::Player2: // Player 2.
				this->Game->SetCurrentPlayer(StackMem::Players::Player2);
				break;

			case GameSettings::RoundStarter::Random: // Just.. random.
				this->Game->SelectRandomPlayer();
				break;

			case GameSettings::RoundStarter::Loser: // Loser of the match.
				if (P2Won) this->Game->SetCurrentPlayer(StackMem::Players::Player1);
				else this->Game->SetCurrentPlayer(StackMem::Players::Player2);
				break;

			case GameSettings::RoundStarter::Winner: // Winner of the match.
				if (P2Won) this->Game->SetCurrentPlayer(StackMem::Players::Player2);
				else this->Game->SetCurrentPlayer(StackMem::Players::Player1);
				break;
		};
	}

	/* Set both of them to their initial values. */
	this->ClickedScale[0] = 1.0f, this->ClickedScale[1] = 1.0f;
	this->CardClicked[0] = false, this->CardClicked[1] = false;
	this->Selection = 0;
	this->Page = 0;
	Pointer::SetPos(this->CPos[this->Selection].X + 10, this->CPos[this->Selection].Y + 10);
	this->StartGameAnimation(); // Initial Animation when starting a game.
};


/* Least amount of tries Play Draw. */
void GameHelper::DrawTryPlay(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("GAME_SCREEN_TITLE"), 395);

	/* Draw Player 1. */
	if (this->Params.Characters[0] < Utils::GetCharSheetSize()) {
		Gui::DrawSprite(GFX::Characters, this->Params.Characters[0], 30, 30);
		Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 28, 28);
		Gui::Draw_Rect(37, 159, 106, 20, KBD_KEYUNPRESSED);
		Gui::DrawString(40, 162, 0.45f, TEXT_COLOR, this->Params.Names[0], 100);
	}

	Gui::DrawString(200, 100, 0.5f, TEXT_COLOR, Lang::Get("TRIES") + std::to_string(this->Params.Tries), 200);

	if (this->Game->GetPairs() > 10) { // Only 11+ Pairs have pages.
		Gui::DrawString(150, 130, 0.5f, TEXT_COLOR, Lang::Get("GAME_SCREEN_CURRENT_PAGE") + std::to_string(this->Page + 1) + " / " + std::to_string(((this->Game->GetPairs() * 2) / 20) + 1), 200);
	}

	if (!this->Params.CardDelayUsed || this->Params.CardDelay == 0) { // The checks only exist on non delay mode.
		if (this->Game->GetState() == StackMem::TurnState::DoCheck) {
			Gui::DrawStringCentered(0, 225, 0.4f, TEXT_COLOR, Lang::Get("GAME_SCREEN_ANY_KEY"), 390);
		}
	}

	Gui::DrawStringCentered(0, 200, 0.5f, TEXT_COLOR, Lang::Get("GAME_SCREEN_EXIT"), 395);
};


/* Normal Play Draw. */
void GameHelper::DrawNormalPlay(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("GAME_SCREEN_TITLE"), 395);

	/* Draw Player 1. */
	if (this->Params.Characters[0] < Utils::GetCharSheetSize()) {
		Gui::DrawSprite(GFX::Characters, this->Params.Characters[0], 30, 30);
		Gui::Draw_Rect(37, 159, 106, 20, KBD_KEYUNPRESSED);
		Gui::DrawString(40, 162, 0.45f, TEXT_COLOR, this->Params.Names[0], 100);

		Gui::DrawString(37, 190, 0.45f, TEXT_COLOR, Lang::Get("PAIRS") + std::to_string(this->Game->GetPlayerPairs(StackMem::Players::Player1)), 100);
	}

	/* Draw Player 2. */
	if (this->Params.Characters[1] < Utils::GetCharSheetSize()) {
		Gui::DrawSprite(GFX::Characters, this->Params.Characters[1], 250, 30);
		Gui::Draw_Rect(257, 159, 106, 20, KBD_KEYUNPRESSED);
		Gui::DrawString(260, 162, 0.45f, TEXT_COLOR, this->Params.Names[1], 100);

		Gui::DrawString(257, 190, 0.45f, TEXT_COLOR, Lang::Get("PAIRS") + std::to_string(this->Game->GetPlayerPairs(StackMem::Players::Player2)), 100);
	}

	/* Draw Current Player. */
	if (this->Game->GetCurrentPlayer() == StackMem::Players::Player1) {
		Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 28, 28);
		Gui::DrawStringCentered(0, 205, 0.4f, TEXT_COLOR, Lang::Get("GAME_SCREEN_CURRENT_PLAYER") + this->Params.Names[0], 390);

	} else {
		Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 248, 28);
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
};


void GameHelper::DrawTop(void) const {
	if (this->Params.GameMode == GameSettings::GameModes::TryPlay) this->DrawTryPlay();
	else this->DrawNormalPlay();
};


/*
	Draws the Gamefield with the cards.
*/
void GameHelper::DrawField(const bool ShowPointer) const {
	GFX::DrawBottom(); // Focus the drawing on the bottom.

	/*
		Draws the side pages buttons, if you can go backward / forward.
		This is only necessary, if you have more than 10 Pairs though, because 10 Pairs fit exactly on screen.
	*/
	if (this->Game->GetPairs() > 10) {
		if (this->Page > 0) { // Because we can go back.
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0, 0); // Draw the small top corner.
			Gui::Draw_Rect(0, 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0, 220, 1.0f, -1.0f); // Draw the small bottom corner.
			Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 0, 110); // Now the arrow!
		}

		if (this->CanGoForward(this->Page)) { // Because we can go forward.
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300, 0, -1.0f, 1.0f); // Draw the small top corner.
			Gui::Draw_Rect(300, 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
			Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300, 220, -1.0f, -1.0f); // Draw the small bottom corner.
			Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 300, 110, -1.0f, 1.0f); // Now the arrow!
		}
	}

	/* Draw the cards. */
	for (size_t Idx = (this->Page * 20), Idx2 = 0; Idx < (this->Page * 20) + 20 && Idx < (this->Game->GetPairs() * 2); Idx++, Idx2++) {
		if (!this->Game->IsCardCollected(Idx)) { // Ensure the card is NOT collected.

			/* This is a turn card, hence we draw it with scale. */
			if (this->Game->GetTurnCard(0) != -1 && this->Game->GetTurnCard(0) == (int)Idx) {
				if (this->CardClicked[0]) { // Animation is done, soo draw front cover.
					Gui::DrawSprite(GFX::Cards, this->Game->GetCardType(this->Game->GetTurnCard(0)), this->CPos[Idx2].X + (1.0f - this->ClickedScale[0]) * 55 / 2, this->CPos[Idx2].Y, this->ClickedScale[0], 1.0f);

				} else { // Draw Back cover.
					Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[Idx2].X + (1.0f - this->ClickedScale[0]) * 55 / 2, this->CPos[Idx2].Y, this->ClickedScale[0], 1.0f);
				}
			};

			/* This is a turn card, hence we draw it with scale. */
			if (this->Game->GetTurnCard(1) != -1 && this->Game->GetTurnCard(1) == (int)Idx) {
				if (this->CardClicked[1]) { // Animation is done, soo draw front cover.
					Gui::DrawSprite(GFX::Cards, this->Game->GetCardType(this->Game->GetTurnCard(1)), this->CPos[Idx2].X + (1.0f - this->ClickedScale[1]) * 55 / 2, this->CPos[Idx2].Y, this->ClickedScale[1], 1.0f);

				} else { // Draw Back cover.
					Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[Idx2].X + (1.0f - this->ClickedScale[1]) * 55 / 2, this->CPos[Idx2].Y, this->ClickedScale[1], 1.0f);
				}
			};

			if (this->Game->GetTurnCard(0) != (int)Idx && this->Game->GetTurnCard(1) != (int)Idx) {
				if (this->Game->IsCardShown(Idx)) GFX::DrawCard(this->Game->GetCardType(Idx), this->CPos[Idx2].X, this->CPos[Idx2].Y);
				else Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[Idx2].X, this->CPos[Idx2].Y); // Back cover because hidden.
			}
		}
	}

	if (this->Game->AIEnabled()) {
		if (this->Game->GetCurrentPlayer() != StackMem::Players::Player2 && ShowPointer) Pointer::Draw();

	} else {
		Pointer::Draw();
	}
};


/*
	If you can go forward or not.

	const size_t CurPage: The current page which should be checked, if you can go forward.
*/
bool GameHelper::CanGoForward(const size_t CurPage) const { return ((CurPage * 20) + 20 < (this->Game->GetPairs() * 2)); };


/*
	Goes to the previous page.
	Returns true, if it was able to go to the previous page.
*/
bool GameHelper::PrevPage() {
	if (this->Page > 0) {
		this->PageAnimation(false);
		return true;
	}

	return false;
};


/*
	Goes to the next page.
	Returns true, if it was able to go to the next page.
*/
bool GameHelper::NextPage() {
	if (this->CanGoForward(this->Page)) {
		this->PageAnimation(true);
		return true;
	}

	return false;
};


/*
	This is used for the Callbacks.

	const uint8_t Idx: The index which got clicked.
*/
void GameHelper::CheckCard(const uint8_t Idx) {
	if (((this->Page * 20) + Idx < this->Game->GetPairs() * 2)) {
		if (!this->Game->IsCardShown((this->Page * 20) + Idx)) this->PickAnimation((this->Page * 20) + Idx);
	}
};


/* Get the amount of indexes for a specified Pairs amount. */
uint8_t GameHelper::GetIndexCount() const {
	switch(this->Game->GetPairs()) {
		case 0: return 1;
		case 1: return 2;
		case 2: return 4;

		case 3:
		case 4:
		case 5:
		case 6:
		case 7: return 6;

		case 8:
		case 9: return 9;
	};

	if (this->Game->GetPairs() >= 10) return 8;
	else return 0;
};


/*
	Get the indexes for a specific "round".

	const uint8_t Round: The "round".
*/
std::vector<uint8_t> GameHelper::GetAnimationIdx(const uint8_t Round) const {
	if (this->Game->GetPairs() >= 10) {
		switch(Round) { // 10+.
			case 0: return { 19 };
			case 1: return { 18, 14 };
			case 2: return { 17, 13, 9 };
			case 3: return { 16, 12, 8, 4 };
			case 4: return { 15, 11, 7, 3 };
			case 5: return { 10, 6, 2 };
			case 6: return { 5, 1 };
			case 7: return { 0 };
		};

	} else { // 9 - 0.
		switch(this->Game->GetPairs()) {
			case 9:
				switch(Round) {
					case 0: return { 14 };
					case 1: return { 17, 13, 9 };
					case 2: return { 16, 12, 8, 4 };
					case 3: return { 15, 11, 7, 3 };
					case 4: return { 10, 6, 2 };
					case 5: return { 5, 1 };
					case 6: return { 0 };
				};
				break;

			case 8:
				switch(Round) {
					case 0: return { 14 };
					case 1: return { 13, 9 };
					case 2: return { 12, 8, 4 };
					case 3: return { 15, 11, 7, 3 };
					case 4: return { 10, 6, 2 };
					case 5: return { 5, 1 };
					case 6: return { 0 };
				};
				break;

			case 7:
				switch(Round) {
					case 0: return { 13, 9 };
					case 1: return { 12, 8, 4 };
					case 2: return { 11, 7, 3 };
					case 3: return { 10, 6, 2 };
					case 4: return { 5, 1 };
					case 5: return { 0 };
				};
				break;

			case 6:
				switch(Round) {
					case 0: return { 9 };
					case 1: return { 8, 4 };
					case 2: return { 11, 7, 3 };
					case 3: return { 10, 6, 2 };
					case 4: return { 5, 1 };
					case 5: return { 0 };
				};
				break;

			case 5:
				switch(Round) {
					case 0: return { 9 };
					case 1: return { 8, 4 };
					case 2: return { 7, 3 };
					case 3: return { 6, 2 };
					case 4: return { 5, 1 };
					case 5: return { 0 };
				};
				break;

			case 4:
				switch(Round) {
					case 0: return { 4 };
					case 2: return { 7, 3 };
					case 3: return { 6, 2 };
					case 4: return { 5, 1 };
					case 5: return { 0 };
				};
				break;

			case 3:
				switch(Round) {
					case 0: return { 4 };
					case 2: return { 3 };
					case 3: return { 2 };
					case 4: return { 5, 1 };
					case 5: return { 0 };
				};
				break;

			case 2:
				switch(Round) {
					case 0: return { 3 };
					case 1: return { 2 };
					case 2: return { 1 };
					case 3: return { 0 };
				};
				break;

			case 1:
				switch(Round) {
					case 0: return { 1 };
					case 1: return { 0 };
				};
				break;

			case 0: return { 0 };
		}
	};

	return { 0 };
};


/*
	Game Animation 1: Let the cards fall down in group from the top left.

	const size_t Pairs: The Pairs amount.
	const GameSettings::GameParams &Params: The Game Parameters.
*/
void GameHelper::StartGameAnimationFalling() {
	if (!_3DZwei::CFG->DoAnimation()) return; // No animation.

	/* Full state / Top Swipe related. */
	bool Done = false, DoSwipe = true;
	int Delay = 255, Dones = 0;
	float Swipe = 0.0f;

	/* Card Animation related. */
	std::vector<bool> Animations;
	std::vector<int> AnimPos;
	std::vector<float> Anim2;
	std::vector<std::vector<uint8_t>> AnimationIdxs;

	for (size_t Idx = 0; Idx < this->GetIndexCount(); Idx++) { // Init the animations.
		Animations.push_back(true); // Animation State.
		AnimationIdxs.push_back(GetAnimationIdx(Idx)); // Indexes.
		AnimPos.push_back(320); // Animation Position.
		Anim2.push_back(0.0f);
	};

	while(aptMainLoop() && !Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		/* Draw First character. It's included in both play modes. */
		if (this->Params.Characters[0] < Utils::GetCharSheetSize()) {
			Gui::DrawSprite(GFX::Characters, this->Params.Characters[0], 30 - Delay, 30);
		}

		/* Draw Second character. It's only included in Normal play mode. */
		if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) {
			if (this->Params.Characters[1] < Utils::GetCharSheetSize()) {
				Gui::DrawSprite(GFX::Characters, this->Params.Characters[1], 250 + Delay, 30);
			}
		}

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (Delay > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, Delay));
		};

		GFX::DrawBottom();

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (Delay > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, Delay));
		};

		/* Display Card Animation. */
		for (size_t Idx = 0; Idx < AnimationIdxs.size(); Idx++) { // Loop through the amount of animations.
			for (size_t Idx2 = 0; Idx2 < AnimationIdxs[Idx].size(); Idx2++) { // Loop through their contents.
				Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[AnimationIdxs[Idx][Idx2]].X - AnimPos[Idx], this->CPos[AnimationIdxs[Idx][Idx2]].Y - AnimPos[Idx]);
			}
		};

		C3D_FrameEnd(0);
		hidScanInput();
		const uint32_t Down = hidKeysDown();
		if (Down) Done = true; // You can always skip this by pressing any key, if it's too long for you.

		for (size_t Idx = 0; Idx < AnimationIdxs.size(); Idx++) {
			if (!Animations[Idx]) continue; // Animation for this already over, continue with the next one in the loop.

			if (Idx == 0) { // 0: Do NOT look back at previous position.
				Anim2[0] = std::lerp(Anim2[0], 320.0f, 0.1f);
				AnimPos[0] = 320 - Anim2[0];

				if (Anim2[0] >= 319.95f) {
					AnimPos[0] = 0;
					Anim2[0] = 0.0f;
					Animations[0] = false;
				}

			} else { // Look back at previous position.
				if (AnimPos[Idx - 1] <= 100) { // Wait some bit for next cards and check previous cards position.
					Anim2[Idx] = std::lerp(Anim2[Idx], 320.0f, 0.1f);
					AnimPos[Idx] = 320 - Anim2[Idx];

					if (Anim2[Idx] >= 319.95f) {
						AnimPos[Idx] = 0;
						Anim2[Idx] = 0.0f;
						Animations[Idx] = false;
					}
				}
			}
		}

		Dones = 0; // Reset otherwise it'd be way over the max.
		for (size_t Idx = 0; Idx < AnimationIdxs.size(); Idx++) {
			if (!Animations[Idx]) Dones++; // If animation is done, increase our Dones.
		}

		if (DoSwipe) {
			if (Swipe < 255) {
				Swipe = std::lerp(Swipe, 255.0f, 0.1f);
				Delay = 255 - Swipe;

				if (Swipe >= 254.95f) {
					Delay = 0;
					Swipe = 0;
					DoSwipe = false;
				}
			}
		}

		if ((Dones == (int)AnimationIdxs.size()) && (!DoSwipe)) Done = true; // Set to done when the move / fade delay is over.. and the cards too.
	};
};


/*
	Game Animation 2: Grow up all cards.

	const size_t Pairs: The Pairs amount.
	const GameSettings::GameParams &Params: The Game Parameters.
*/
void GameHelper::StartGameAnimationGrowing() {
	if (!_3DZwei::CFG->DoAnimation()) return; // No animation.

	bool Done = false, DoSwipe = true;
	int Delay = 255, CCard = 0, ToInit = 0;
	float CardScale = 0.1f, Swipe = 0.0f, Cubic = 0.0f;

	if (this->Game->GetPairs() >= 10) ToInit = 20; // 10+ -> 20.
	else ToInit = this->Game->GetPairs() * 2; // Else Pairs * 2.

	while(aptMainLoop() && !Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		/* Draw First character. It's included in both play modes. */
		if (this->Params.Characters[0] < Utils::GetCharSheetSize()) {
			Gui::DrawSprite(GFX::Characters, this->Params.Characters[0], 30 - Delay, 30);
		}

		/* Draw Second character. It's only included in Normal play mode. */
		if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) {
			if (this->Params.Characters[1] < Utils::GetCharSheetSize()) {
				Gui::DrawSprite(GFX::Characters, this->Params.Characters[1], 250 + Delay, 30);
			}
		}

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (Delay > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, Delay));
		}

		GFX::DrawBottom();

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (Delay > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, Delay));
		}

		/* Draw already growed cards. */
		for (int Idx = 0; Idx < CCard; Idx++) {
			Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[Idx].X, this->CPos[Idx].Y);
			if (Delay > 0) Gui::Draw_Rect(this->CPos[Idx].X, this->CPos[Idx].Y, 55, 55, C2D_Color32(0, 0, 0, Delay));
		}

		if (CCard < ToInit) Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[CCard].X, this->CPos[CCard].Y, CardScale, CardScale);
		C3D_FrameEnd(0);

		hidScanInput();
		const uint32_t Down = hidKeysDown();
		if (Down) Done = true; // You can always skip this by pressing any key, if it's too long for you.

		if (CCard < ToInit) {
			if (Cubic < 0.95f) {
				Cubic = std::lerp(Cubic, 1.0f, 0.3f);
				CardScale = Cubic;

				if (Cubic >= 0.95f) {
					CardScale = 0.0f, Cubic = 0.0f;
					CCard++;
				}
			}
		}

		if (DoSwipe) {
			if (Swipe < 255) {
				Swipe = std::lerp(Swipe, 255.0f, 0.1f);
				Delay = 255 - Swipe;

				if (Swipe >= 254.95f) {
					Delay = 0;
					Swipe = 0;
					DoSwipe = false;
				}
			}
		}

		if ((CCard == ToInit) && (!DoSwipe)) Done = true; // Set to done when the move / fade delay is over.. and the cards too.
	};
};


/* The Start Animation of the game. */
void GameHelper::StartGameAnimation() {
	if (_3DZwei::CFG->DoAnimation()) {
		switch(_3DZwei::CFG->GameAnimation()) {
			case 0:
				{ // Random.
					const uint8_t Idx = rand() % START_ANIMATION_AMOUNT + 1; // 1 - 2 currently.
					switch(Idx) {
						case 1:
							this->StartGameAnimationFalling(); // Falling.
							break;

						case 2:
							this->StartGameAnimationGrowing(); // Growing.
							break;
					};
				}
				break;

			case 1:
				this->StartGameAnimationFalling(); // Falling.
				break;

			case 2:
				this->StartGameAnimationGrowing(); // Growing.
				break;
		};
	}
};


/*
	The Page Switching Animation.

	const bool Forward: If going Forward (true) or backwards (false).
*/
void GameHelper::PageAnimation(const bool Forward) {
	const size_t NewPage = (Forward ? (this->Page + 1) : (this->Page - 1));

	if (!_3DZwei::CFG->PageSwitch()) {
		this->Page = NewPage;
		return; // No Animation.
	};

	bool Done = false;
	int SwipePos = 0;
	float Cubic = 0.0f;

	while(aptMainLoop() && !Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		this->DrawTop();
		GFX::DrawBottom();

		/* Current Page site navigators. */
		if (this->Page > 0) { // Because we can go back.
			if (Forward) { // Goes from <- to ->.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0 - SwipePos, 0); // Draw the small top corner.
				Gui::Draw_Rect(0 - SwipePos, 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0 - SwipePos, 220, 1.0f, -1.0f); // Draw the small bottom corner.
				Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 0 - SwipePos, 110); // Now the arrow!

			} else { // Goes from <- to ->.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0 + SwipePos, 0); // Draw the small top corner.
				Gui::Draw_Rect(0 + SwipePos, 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0 + SwipePos, 220, 1.0f, -1.0f); // Draw the small bottom corner.
				Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 0 + SwipePos, 110); // Now the arrow!
			}
		};
		if (this->CanGoForward(this->Page)) { // Because we can go forward.
			if (Forward) { // Goes from <- to ->.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300 - SwipePos, 0, -1.0f, 1.0f); // Draw the small top corner.
				Gui::Draw_Rect(300 - SwipePos, 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300 - SwipePos, 220, -1.0f, -1.0f); // Draw the small bottom corner.
				Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 300 - SwipePos, 110, -1.0f, 1.0f); // Now the arrow!

			} else { // Goes from <- to ->.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300 + SwipePos, 0, -1.0f, 1.0f); // Draw the small top corner.
				Gui::Draw_Rect(300 + SwipePos, 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300 + SwipePos, 220, -1.0f, -1.0f); // Draw the small bottom corner.
				Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 300 + SwipePos, 110, -1.0f, 1.0f); // Now the arrow!
			}
		};

		/* Prev / Next Page site navigators. */
		if (NewPage > 0) { // Because we can go back.
			if (Forward) {
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0 + (320 - SwipePos), 0); // Draw the small top corner.
				Gui::Draw_Rect(0 + (320 - SwipePos), 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0 + (320 - SwipePos), 220, 1.0f, -1.0f); // Draw the small bottom corner.
				Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 0 + (320 - SwipePos), 110); // Now the arrow!

			} else {
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0 - (320 - SwipePos), 0); // Draw the small top corner.
				Gui::Draw_Rect(0 - (320 - SwipePos), 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 0 - (320 - SwipePos), 220, 1.0f, -1.0f); // Draw the small bottom corner.
				Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 0 - (320 - SwipePos), 110); // Now the arrow!
			}
		};
		if (this->CanGoForward(NewPage)) { // Because we can go forward.
			if (Forward) {
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300 + (320 - SwipePos), 0, -1.0f, 1.0f); // Draw the small top corner.
				Gui::Draw_Rect(300 + (320 - SwipePos), 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300 + (320 - SwipePos), 220, -1.0f, -1.0f); // Draw the small bottom corner.
				Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 300 + (320 - SwipePos), 110, -1.0f, 1.0f); // Now the arrow!

			} else {
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300 - (320 - SwipePos), 0, -1.0f, 1.0f); // Draw the small top corner.
				Gui::Draw_Rect(300 - (320 - SwipePos), 20, 20, 200, CORNER_COLOR); // Draw the Middle corner bar.
				Gui::DrawSprite(GFX::Sprites, sprites_small_corner_idx, 300 - (320 - SwipePos), 220, -1.0f, -1.0f); // Draw the small bottom corner.
				Gui::DrawSprite(GFX::Sprites, sprites_arrow_idx, 300 - (320 - SwipePos), 110, -1.0f, 1.0f); // Now the arrow!
			}
		};


		/* Draw current Page cards. */
		for (size_t Idx = (this->Page * 20), Idx2 = 0; Idx < (this->Page * 20) + 20 && Idx < (this->Game->GetPairs() * 2); Idx++, Idx2++) {
			if (!this->Game->IsCardCollected(Idx)) {
				if (Forward) { // Goes from <- to ->.
					if (this->Game->IsCardShown(Idx)) GFX::DrawCard(this->Game->GetCardType(Idx), this->CPos[Idx2].X - SwipePos, this->CPos[Idx2].Y);
					else Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[Idx2].X - SwipePos, this->CPos[Idx2].Y); // Back cover because hidden.


				} else { // Goes from -> to <-.
					if (this->Game->IsCardShown(Idx))
						GFX::DrawCard(this->Game->GetCardType(Idx), this->CPos[Idx2].X + SwipePos, this->CPos[Idx2].Y);
					else
						Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[Idx2].X + SwipePos, this->CPos[Idx2].Y); // Back cover because hidden.
				}
			}
		};

		/* Draw new Page cards. */
		for (size_t Idx = (NewPage * 20), Idx2 = 0; Idx < (NewPage * 20) + 20 && Idx < (this->Game->GetPairs() * 2); Idx++, Idx2++) {
			if (!this->Game->IsCardCollected(Idx)) {
				if (Forward) { // Goes from -> to <-.
					if (this->Game->IsCardShown(Idx)) GFX::DrawCard(this->Game->GetCardType(Idx), this->CPos[Idx2].X + (320 - SwipePos), this->CPos[Idx2].Y);
					else Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[Idx2].X + (320 - SwipePos), this->CPos[Idx2].Y); // Back cover because hidden.


				} else { // Goes from <- to ->.
					if (this->Game->IsCardShown(Idx))
						GFX::DrawCard(this->Game->GetCardType(Idx), this->CPos[Idx2].X - (320 - SwipePos), this->CPos[Idx2].Y);
					else
						Gui::DrawSprite(GFX::Cards, Utils::GetCardSheetSize(), this->CPos[Idx2].X - (320 - SwipePos), this->CPos[Idx2].Y); // Back cover because hidden.
				}
			}
		};

		C3D_FrameEnd(0);
		if (Cubic < 320.0f) { // Page Switch handle.
			Cubic = std::lerp(Cubic, 321.0f, 0.1f);

			SwipePos = Cubic;
			if (Cubic >= 320.0f) Done = true;
		}
	}

	this->Page = NewPage; // Set the new page.
};


/*
	The card picker Animation.

	const size_t Idx: The index which to pick.
*/
void GameHelper::PickAnimation(const size_t Idx) {
	if (!this->Game->IsCardShown((Idx))) { // Make sure it is NOT shown at all.
		this->Game->DoPlay(Idx); // Play.
		const uint8_t Idx = (this->Game->GetState() == StackMem::TurnState::DoCheck ? 1 : 0);

		if (!_3DZwei::CFG->DoAnimation()) {
			this->CardClicked[Idx] = true;
			return; // No Animation.
		};

		float Cubic = 0.0f;
		bool Done = false;

		while(aptMainLoop() && !Done) {
			Gui::clearTextBufs();
			C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
			C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			this->DrawTop();
			this->DrawField(false);
			C3D_FrameEnd(0);

			if (!this->CardClicked[Idx]) { // Scale down the back cover -- first operation.
				if (Cubic < 0.95f) {
					Cubic = std::lerp(Cubic, 2.0f, 0.1f);

					this->ClickedScale[Idx] = 1.0f - Cubic;

					if (Cubic >= 0.95f) {
						Cubic = 0.0f;
						this->ClickedScale[Idx] = 0.0f;
						this->CardClicked[Idx] = true;
					}
				}

			} else { // Scale up the front cover -- second operation.
				if (Cubic < 0.95f) {
					Cubic = std::lerp(Cubic, 2.0f, 0.1f);
					this->ClickedScale[Idx] = Cubic;

					if (Cubic >= 0.95f) {
						this->ClickedScale[Idx] = 1.0f;
						Done = true;
					}
				}
			}
		}
	}

	this->RefreshFrame = true; // Refresh the frame.
};


/*
	If the two cards did not match -> Hide them again.
*/
void GameHelper::HideAnimation() {
	if (!_3DZwei::CFG->DoAnimation()) {
		this->CardClicked[0] = false, this->CardClicked[1] = false;
		return; // No Animation.
	};

	bool Done = false;
	float Cubic = 0.0f;

	while(aptMainLoop() && !Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		this->DrawTop();
		this->DrawField(false);
		C3D_FrameEnd(0);

		if (this->CardClicked[0]) { // Front cover handle.
			if (Cubic < 0.95f) {
				Cubic = std::lerp(Cubic, 2.0f, 0.1f);

				this->ClickedScale[0] = 1.0f - Cubic;
				this->ClickedScale[1] = 1.0f - Cubic;

				if (Cubic >= 0.95f) {
					Cubic = 0.0f;
					this->ClickedScale[0] = 0.0f, this->ClickedScale[1] = 0.0f;
					this->CardClicked[0] = false, this->CardClicked[1] = false;
				}
			}

		} else { // Back cover handle.
			if (Cubic < 0.95f) {
				Cubic = std::lerp(Cubic, 2.0f, 0.1f);

				this->ClickedScale[0] = Cubic;
				this->ClickedScale[1] = Cubic;

				if (Cubic >= 0.95f) {
					this->ClickedScale[0] = 1.0f, this->ClickedScale[1] = 1.0f;
					Done = true;
				}
			}
		}
	}

	this->RefreshFrame = true;
};


/*
	If you collected a pair, shrink both cards out.
*/
void GameHelper::ShrinkAnimation() {
	if (!_3DZwei::CFG->DoAnimation()) {
		this->CardClicked[0] = false, this->CardClicked[1] = false;
		this->ClickedScale[0] = 0.0f, this->ClickedScale[1] = 0.0f;
		this->RefreshFrame = true;
		return; // No Animation.
	};

	bool Done = false;
	float Cubic = 0.0f;

	while(aptMainLoop() && !Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		this->DrawTop();
		this->DrawField(false);
		C3D_FrameEnd(0);

		if (this->CardClicked[0]) { // Let both cards shrink and disappear.
			if (Cubic < 0.95f) {
				Cubic = std::lerp(Cubic, 2.0f, 0.1f);

				this->ClickedScale[0] = 1.0f - Cubic;
				this->ClickedScale[1] = 1.0f - Cubic;

				if (Cubic >= 0.95f) {
					/* Reset both values properly. */
					this->ClickedScale[0] = 0.0f, this->ClickedScale[1] = 0.0f;
					this->CardClicked[0] = false, this->CardClicked[1] = false;
					Done = true;
				}
			}
		}
	}

	this->RefreshFrame = true; // Refresh the frame.
};


/*
	The End-Game Animation.
	Basically fades out the screen and swipes the characters out.
*/
void GameHelper::EndGameAnimation() {
	if (!_3DZwei::CFG->DoAnimation()) return; // No Animation.

	bool Done = false;
	int Delay = 0;
	float Cubic = 0.0f;

	while(aptMainLoop() && !Done) {
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop();
		/* Draw First character. It's included in both play modes. */
		if (this->Params.Characters[0] < Utils::GetCharSheetSize()) {
			Gui::DrawSprite(GFX::Characters, this->Params.Characters[0], 30 - Delay, 30);
		}

		/* Draw Second character. It's only included in Normal play mode. */
		if (this->Params.GameMode == GameSettings::GameModes::NormalPlay) {
			if (this->Params.Characters[1] < Utils::GetCharSheetSize()) {
				Gui::DrawSprite(GFX::Characters, this->Params.Characters[1], 250 + Delay, 30);
			}
		}

		if (Delay > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, Delay));
		GFX::DrawBottom();
		if (Delay > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, Delay));
		C3D_FrameEnd(0);

		hidScanInput();
		const uint32_t Down = hidKeysDown();
		if (Down) Done = true; // You can always skip this by pressing any key, if it's too long for you.

		/* Fade and Move Logic. */
		if (!Done) {
			if (Cubic < 255.0f) {
				Cubic = std::lerp(Cubic, 256.0f, 0.1f);
				Delay = Cubic;

				if (Cubic >= 255.0f) {
					Delay = 0;
					Done = true;
				}
			}
		};
	};
};


/*
	This is called, when both cards are played.

	This returns the following states:
		P1 Wins: GameHelper::LogicState::P1Won.
		P2 Wins: GameHelper::LogicState::P2Won.
		No one Wins: GameHelper::LogicState::Tie.
		Not even over yet: GameHelper::LogicState::Nothing.
*/
GameHelper::LogicState GameHelper::TurnChecks() {
	if (this->Game->DoCheck(false)) { // Do not hide the cards directly there.
		/* Check if over. */
		if (this->Game->CheckGameState() != StackMem::GameState::NotOver) {
			this->ShrinkAnimation(); // Shrink cards.

			const StackMem::GameState Res = this->Game->CheckGameState();
			switch(Res) {
				case StackMem::GameState::NotOver:
					break;

				case StackMem::GameState::Tie:
				case StackMem::GameState::Player1:
				case StackMem::GameState::Player2: // All 3 states are valid end states.
				{
					/* Set Pairs. */
					this->Params.PlayerPairs[0] = this->Game->GetPlayerPairs(StackMem::Players::Player1);
					this->Params.PlayerPairs[1] = this->Game->GetPlayerPairs(StackMem::Players::Player2);
					this->EndGameAnimation(); // The game is over.

					switch(Res) {
						case StackMem::GameState::Tie:
							return GameHelper::LogicState::Tie;

						case StackMem::GameState::Player1:
							return GameHelper::LogicState::P1Won;

						case StackMem::GameState::Player2:
							return GameHelper::LogicState::P2Won;

						case StackMem::GameState::NotOver:
							return GameHelper::LogicState::Nothing;
					};
				}

				break;
			};

		} else { // A pair is collected, so shrink the cards + reset the state.
			this->ShrinkAnimation();
			/* We collected the cards. */
			this->Game->SetCardCollected(this->Game->GetTurnCard(0), true); this->Game->SetCardCollected(this->Game->GetTurnCard(1), true);
			this->Game->ResetTurn(true); // Only reset state, don't hide cards.
			this->ClickedScale[0] = 1.0f, this->ClickedScale[1] = 1.0f; // Reset too, just in case.
			this->Game->SetState(StackMem::TurnState::DrawFirst); // We were able to play a card, so let us continue!
		}

	} else { // Both cards do not match, hide them + reset the state.
		this->HideAnimation();
		this->Game->ResetTurn(false); // Hide cards, reset state.

		/* At this point, we need to make an exception for Try and Normal Mode. */
		if (this->Params.GameMode == GameSettings::GameModes::TryPlay) { // Try Mode.
			this->Params.Tries++; // Increase the Tries.
			this->Game->SetState(StackMem::TurnState::DrawFirst); // Set first State again.

		} else { // Normal Mode.
			this->Game->NextPlayer(); // Nah, no match. Next player!
		}
	}

	this->RefreshFrame = true; // Refresh frame.
	return GameHelper::LogicState::Nothing; // Nothing special.
};


/*
	Normal Player Logic.
*/
GameHelper::LogicState GameHelper::PlayerLogic(const uint32_t Down, const uint32_t Held, const touchPosition T) {
	Pointer::ScrollHandling(Held, true); // Scrolling handle.

	if (Down & KEY_L) this->PrevPage();
	if (Down & KEY_R) this->NextPage();

	if (Down & KEY_DDOWN) {
		if (this->Selection < 15) { // 15 --> 4th row first card.
			if ((this->Page * 20) + this->Selection + 5 < this->Game->GetPairs() * 2) { // Ensureness.
				this->Selection += 5;
				Pointer::SetPos(this->CPos[this->Selection].X + 10, this->CPos[this->Selection].Y + 10);
			}
		}
	};

	if (Down & KEY_DUP) {
		if (this->Selection > 4) { // 4 --> 1st row last card.
			this->Selection -= 5;
			Pointer::SetPos(this->CPos[this->Selection].X + 10, this->CPos[this->Selection].Y + 10);
		}
	};

	if (Down & KEY_DLEFT) {
		if (this->Selection == 0 || this->Selection == 5 || this->Selection == 10 || this->Selection == 15) {
			this->PrevPage();

		} else {
			if (this->Selection > 0) {
				this->Selection--;
				Pointer::SetPos(this->CPos[this->Selection].X + 10, this->CPos[this->Selection].Y + 10);
			}
		}
	};

	if (Down & KEY_DRIGHT) {
		if (this->Selection < 20) {
			if (this->Selection == 4 || this->Selection == 9 || this->Selection == 14 || this->Selection == 19) {
				if (this->NextPage()) {
					if ((this->Page * 20) + this->Selection + 1 > this->Game->GetPairs() * 2) { // Ensureness.
						this->Selection = ((this->Game->GetPairs() * 2) - 1) % 20;
						Pointer::SetPos(this->CPos[this->Selection].X + 10, this->CPos[this->Selection].Y + 10);
					}
				}

			} else {
				if ((this->Page * 20) + this->Selection + 1 < this->Game->GetPairs() * 2) { // Ensureness.
					this->Selection++;
					Pointer::SetPos(this->CPos[this->Selection].X + 10, this->CPos[this->Selection].Y + 10);
				}
			}
		}
	};

	if (Down & KEY_A) {
		for (auto &Position : this->CPos) {
			if (Pointer::Clicked(Position, true)) return GameHelper::LogicState::Nothing;
		}
	}

	if (Down & KEY_TOUCH) {
		for (auto &Position : this->CPos) {
			if (Touched(Position, T, true)) return GameHelper::LogicState::Nothing;
		}
	}

	return GameHelper::LogicState::Nothing;
};


/*
	Normal Mode -> AI Logic.
*/
GameHelper::LogicState GameHelper::AILogic(const uint32_t Down) {
	if (this->Params.CardDelayUsed && this->Params.CardDelay > 0) { // Play with card delay.
		if (this->RefreshFrame) {
			this->RefreshFrame = false;
			return GameHelper::LogicState::Nothing;
		};

		for (size_t Idx = 0; Idx < this->Params.CardDelay; Idx++) { gspWaitForVBlank(); }; // Delay.
		const size_t Card = this->Game->AIPlay();

		/* Switch to proper pages. */
		const size_t NewPage = (Card / 20);
		if (NewPage > this->Page) {
			while(aptMainLoop() && (this->Page < NewPage)) { this->PageAnimation(true); }; // Go forward.

		} else if (NewPage < this->Page) {
			while(aptMainLoop() && (this->Page > NewPage)) { this->PageAnimation(false); }; // Go backward.
		};

		this->PickAnimation(Card);
		this->Selection = (Card % 20);
		Pointer::SetPos(this->CPos[this->Selection].X + 10, this->CPos[this->Selection].Y + 10);

	} else { // We don't have a valid card delay set, so handle through click.
		if (Down) {
			const size_t Card = this->Game->AIPlay();

			/* Switch to proper pages. */
			const size_t NewPage = (Card / 20);
			if (NewPage > this->Page) {
				while(aptMainLoop() && (this->Page < NewPage)) { this->PageAnimation(true); }; // Go forward.

			} else if (NewPage < this->Page) {
				while(aptMainLoop() && (this->Page > NewPage)) { this->PageAnimation(false); }; // Go backward.
			};

			this->PickAnimation(Card);
			this->Selection = (Card % 20);
			Pointer::SetPos(this->CPos[this->Selection].X + 10, this->CPos[this->Selection].Y + 10);
		}
	}

	return GameHelper::LogicState::Nothing;
};


/*
	Only THIS function SHOULD be called.

	const uint32_t Down: The hidKeysDown() variable.
	const uint32_t Held: The hidKeysHeld() variable.
	const touchPosition T: The touchPosition variable.

	This returns the following states:
		P1 Wins: GameHelper::LogicState::P1Won.
		P2 Wins: GameHelper::LogicState::P2Won.
		No one Wins: GameHelper::LogicState::Tie.
		Not even over yet: GameHelper::LogicState::Nothing.
*/
GameHelper::LogicState GameHelper::Logic(const uint32_t Down, const uint32_t Held, const touchPosition T) {
	if (this->Params.GameMode == GameSettings::GameModes::TryPlay) { // Least amount of tries play mode.
		if (this->Game->GetState() != StackMem::TurnState::DoCheck) { // As long as the State is not check, we can play.
			this->PlayerLogic(Down, Held, T);

		} else { // We are in the DoCheck state, so check.
			if (this->Params.CardDelayUsed && this->Params.CardDelay > 0) { // Use the card delay.
				if (this->RefreshFrame) {
					this->RefreshFrame = false;
					return GameHelper::LogicState::Nothing;
				};

				if (this->Params.CardDelayUsed && this->Params.CardDelay > 0) {
					for (size_t Idx = 0; Idx < this->Params.CardDelay; Idx++) { gspWaitForVBlank(); }; // Do the delay, if enabled.
				}

				const GameHelper::LogicState State = this->TurnChecks();
				return State;

			} else { // We don't have a valid card delay set, so handle through click.
				if (Down) {
					const GameHelper::LogicState State = this->TurnChecks();
					return State;
				}
			}
		}

		/* The Logic of Normal Play. */
	} else {
		if (this->Game->GetState() != StackMem::TurnState::DoCheck) { // As long as the State is not check, we can play.
			/* Player 1 is ALWAYS a player. */
			if (this->Game->GetCurrentPlayer() == StackMem::Players::Player1) return this->PlayerLogic(Down, Held, T);

			/* Player 2 Logic. */
			else {
				if (this->Game->AIEnabled()) return this->AILogic(Down); // It's the AI's turn.
				else return this->PlayerLogic(Down, Held, T); // It's the second player's turn.
			}

		} else { // State -> DoCheck.
			if (this->Params.CardDelayUsed && this->Params.CardDelay > 0) { // Use the card delay.
				if (this->RefreshFrame) {
					this->RefreshFrame = false;
					return GameHelper::LogicState::Nothing;
				};

				if (this->Params.CardDelayUsed && this->Params.CardDelay > 0) {
					for (uint8_t Idx = 0; Idx < this->Params.CardDelay; Idx++) { gspWaitForVBlank(); }; // Do the delay, if enabled.
				}

				const GameHelper::LogicState State = this->TurnChecks();
				return State;

			} else { // We don't have a valid card delay set, so handle through click.
				if (Down) {
					const GameHelper::LogicState State = this->TurnChecks();
					return State;
				}
			}
		}
	}

	return GameHelper::LogicState::Nothing;
};