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

#include "Browser.hpp"
#include "CardSetSelector.hpp"
#include "Common.hpp"
#include "Utils.hpp"

#define SETS_PER_SCREEN 5
#define CARDS_PER_SCREEN 9


CardSetSelector::CardSetSelector() {
	const std::vector<std::string> Tmp = { "t3x" }; // Only accept t3x files.
	std::unique_ptr<Browser> B = std::make_unique<Browser>("sdmc:/3ds/ut-games/sets/3DZwei/", Tmp);
	const std::vector<std::string> S = B->GetList();

	this->CardSets.push_back("3DZwei-RomFS"); // Push back the 3DZwei-RomFS default set. :p

	for (size_t Idx = 0; Idx < S.size(); Idx++) {
		if (S[Idx].size() > 4) { // Ensure larger than 4.
			/* Make sure it has the .t3x file extension. */
			if (S[Idx].substr(S[Idx].size() - 4) == ".t3x") this->CardSets.push_back(S[Idx]);
		}
	};
};


void CardSetSelector::PreviewSelection(const size_t Idx, const bool SetSelection, const bool First) {
	/* Clicking on the same thing would switch to the preview. */
	if (SetSelection) {
		if (this->SelectedSet == (int)Idx) { // Is equal -> So it got selected.
			this->IsSelecting = false, this->ModeSwitch = true;
			return;

		} else {
			if (Idx > this->CardSets.size() - 1) return; // Too large.
			this->SelectedSet = Idx;
		}
	};

	if (Idx < this->CardSets.size()) { // Ensure it's smaller than the size.
		if (!First) this->OldCardsetOut();
		this->CardScale.clear(); this->CardFlipped.clear(); // Clear them.
		this->CardPage = 0, this->SetGood = false;
		if (this->PreviewSheet) C2D_SpriteSheetFree(this->PreviewSheet); // Free first.

		if (this->CardSets[Idx] == "3DZwei-RomFS") { // Load from the RomFS.
			this->PreviewSheet = C2D_SpriteSheetLoad("romfs:/gfx/cards.t3x");
			this->SetGood = this->PreviewSheet;

		} else { // Load from the SD otherwise.
			if (this->CardSets[Idx] != "" && Utils::CheckSetContent(this->CardSets[Idx], false)) {
				this->PreviewSheet = C2D_SpriteSheetLoad((std::string("sdmc:/3ds/ut-games/sets/3DZwei/") + this->CardSets[Idx]).c_str());
				this->SetGood = this->PreviewSheet;
			}
		}

		if (this->SetGood) {
			/* Re-Load them. */
			if (C2D_SpriteSheetCount(this->PreviewSheet) > 1) { // Make sure there are at least 2 cards.
				for (size_t Idx = 0; Idx < C2D_SpriteSheetCount(this->PreviewSheet) - 1; Idx++) {
					this->CardScale.push_back(1.0f);
					this->CardFlipped.push_back(true);
				}
			}
		};

		/* Swipe new selected cardset in. */
		this->CurCardPos = -400;
		this->CardSwipeIn = true;
	};
};


/*
	Swipe the old cardset out.
*/
void CardSetSelector::OldCardsetOut() {
	bool Done = false;
	this->CardSwipeOut = true;

	while(aptMainLoop() && !Done) {
		this->Draw();

		hidScanInput();
		const uint32_t Repeat = hidKeysDownRepeat();

		if (!_3DZwei::CFG->DoAnimation() || Repeat) {
			this->CardSwipeOut = false, this->CurCardPos = 0, this->Cubic = 0.0f;
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.3f);
			this->CurCardPos = 0 - this->Cubic;

			if (this->Cubic >= 400.0f) {
				this->Cubic = 0.0f, this->CardSwipeOut = false, this->CurCardPos = 0;
				Done = true;
			}
		}
	};
};


/* Toggle the specified indexed card. */
void CardSetSelector::ToggleCard(const uint8_t Idx) {
	if (((this->CardPage * CARDS_PER_SCREEN) + Idx) < (int)this->CardFlipped.size()) {
		this->ToFlip = (this->CardPage * CARDS_PER_SCREEN) + Idx;
		this->FlipCard = true;
	}
};


/* Go to the previous card page. */
void CardSetSelector::PrevCardPage() {
	if (this->CardPage > 0) {
		this->CardSwipeDir = true;
		this->CardSwipe = true;
	}
};


/* Go to the last cardset. */
void CardSetSelector::LastSet() {
	if (this->SelectedSet > 0) {
		this->SelectedSet--;

		this->PreviewSelection(this->SelectedSet);
	}
};


/* Go to the previous cardset page. */
void CardSetSelector::PrevSetPage() {
	if (this->SelectedSet > 0) {
		if (this->SelectedSet > SETS_PER_SCREEN)this->SelectedSet -= SETS_PER_SCREEN;
		else this->SelectedSet = 0;

		this->PreviewSelection(this->SelectedSet);
	}
};


/* Go to the next card page. */
void CardSetSelector::NextCardPage() {
	if (this->CardCanGoNext()) {
		this->CardSwipeDir = false;
		this->CardSwipe = true;
	}
};


/* Go to the next cardset. */
void CardSetSelector::NextSet() {
	if (this->SelectedSet < (int)this->CardSets.size() - 1) {
		this->SelectedSet++;

		this->PreviewSelection(this->SelectedSet);
	}
};


/* Go to the next cardset page. */
void CardSetSelector::NextSetPage() {
	if (this->SelectedSet < (int)this->CardSets.size() - 1) {
		if (this->SelectedSet + SETS_PER_SCREEN < (int)this->CardSets.size() - 1) this->SelectedSet += SETS_PER_SCREEN;
		else this->SelectedSet = this->CardSets.size() - 1;

		this->PreviewSelection(this->SelectedSet);
	}
};


/* Return, if a next card page is available. */
bool CardSetSelector::CardCanGoNext() const {
	return (((this->CardPage * CARDS_PER_SCREEN) + CARDS_PER_SCREEN) < (int)this->CardFlipped.size());
};


void CardSetSelector::Cancel() { this->IsSelecting = true, this->ModeSwitch = true; };
void CardSetSelector::Confirm() { this->Res = true, this->Done = true; };


/*
	Draws a card page with checks.

	const size_t Pg: The page of the cardset which to draw.
	const int AddOffs: The Offsets to add to the base position.
*/
void CardSetSelector::DrawCardPage(const size_t Pg, const int AddOffs) {
	if (this->SetGood) { // Ensure set is good.
		if ((Pg * CARDS_PER_SCREEN) < this->CardFlipped.size()) {
			for (size_t Idx = (Pg * CARDS_PER_SCREEN), Idx2 = 0; Idx < (Pg * CARDS_PER_SCREEN) + CARDS_PER_SCREEN && Idx < this->CardFlipped.size(); Idx++, Idx2++) {
				if (this->CardFlipped[Idx]) {
					Gui::DrawSprite(this->PreviewSheet, Idx, this->Positions[Idx2 + 2].X + (1.0f - this->CardScale[Idx]) * 55 / 2 + AddOffs, this->Positions[Idx2 + 2].Y, this->CardScale[Idx], 1.0f);

				} else {
					Gui::DrawSprite(this->PreviewSheet, this->CardFlipped.size(), this->Positions[Idx2 + 2].X + (1.0f - this->CardScale[Idx]) * 55 / 2 + AddOffs, this->Positions[Idx2 + 2].Y, this->CardScale[Idx], 1.0f);
				}
			}
		}
	}
};


/*
	Draws a cardset list.

	const int AddOffs: The Offsets to add to the base position.
*/
void CardSetSelector::DrawSetList(const int AddOffs) {
	for(size_t Idx = 0; Idx < SETS_PER_SCREEN && Idx < this->CardSets.size(); Idx++) {
		Gui::Draw_Rect(this->SetPos[1 + Idx].X + AddOffs, this->SetPos[1 + Idx].Y, this->SetPos[1 + Idx].W, this->SetPos[1 + Idx].H, BAR_BLUE);
		Gui::DrawStringCentered(0 + AddOffs, this->SetPos[1 + Idx].Y + 8, 0.45f, TEXT_WHITE, this->CardSets[this->SetListPos + Idx], 160);
		GFX::DrawCheckbox(this->SetPos[Idx + 1].X + 194 + AddOffs, this->SetPos[Idx + 1].Y + 4, (this->SetListPos + (int)Idx == this->SelectedSet), true);
	}

	GFX::DrawCornerEdge(true, this->SetPos[0].X + AddOffs, this->SetPos[0].Y, this->SetPos[0].H, this->SelectedSet > 0);
	GFX::DrawCornerEdge(false, this->SetPos[6].X + AddOffs, this->SetPos[6].Y, this->SetPos[6].H, this->SelectedSet < (int)this->CardSets.size() - 1);
};

/*
	Draws the bottom of the Card Selector.

	const int AddOffs: The Offsets to add to the base position.
*/
void CardSetSelector::DrawCardBottom(const int AddOffs) {
	Gui::DrawStringCentered(0 + AddOffs, 3, 0.6f, TEXT_WHITE, Lang::Get("AMOUNT_OF_CARDS") + std::to_string(this->CardFlipped.size()) , 310);

	/* Draw Cancel and Confirm buttons. */
	Gui::Draw_Rect(this->BottomPos[11].X + AddOffs, this->BottomPos[11].Y, this->BottomPos[11].W, this->BottomPos[11].H, BAR_BLUE);
	Gui::DrawStringCentered(-60 + AddOffs, this->BottomPos[11].Y + 3, 0.6f, TEXT_WHITE, Lang::Get("CANCEL"));
	Gui::Draw_Rect(this->BottomPos[12].X + AddOffs, this->BottomPos[12].Y, this->BottomPos[12].W, this->BottomPos[12].H, BAR_BLUE);
	Gui::DrawStringCentered(60 + AddOffs, this->BottomPos[12].Y + 3, 0.6f, TEXT_WHITE, Lang::Get("CONFIRM"));

	/* Draw the Checkboxes for toggling the back cover. */
	for (size_t Idx = (this->CardPage * CARDS_PER_SCREEN), Idx2 = 0; Idx < ((size_t)this->CardPage * CARDS_PER_SCREEN) + CARDS_PER_SCREEN && Idx < this->CardFlipped.size(); Idx++, Idx2++) {
		GFX::DrawCheckbox(this->BottomPos[Idx2 + 2].X + AddOffs, this->BottomPos[Idx2 + 2].Y, this->CardFlipped[Idx]);
	}

	GFX::DrawCornerEdge(true, this->BottomPos[0].X + AddOffs, this->BottomPos[0].Y, this->BottomPos[0].H, this->CardPage > 0);
	GFX::DrawCornerEdge(false, this->BottomPos[1].X + AddOffs, this->BottomPos[1].Y, this->BottomPos[1].H, this->CardCanGoNext());
};


/*
	The Main Draw handle.
*/
void CardSetSelector::Draw() {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	GFX::DrawTop();

	/* Top bar text. */
	if (this->IsSelecting) Gui::DrawStringCentered(0, 3, 0.6f, TEXT_WHITE, Lang::Get("CARDSET_SELECTOR_TITLE"), 395);
	else Gui::DrawStringCentered(0, 3, 0.6f, TEXT_WHITE, Lang::Get("CARDSET_SELECTOR_CONFIRM"), 395);

	/* Display Preview of the current set. */
	if (this->SetGood) {
		if (this->CardSwipeIn || this->CardSwipeOut) {
			this->DrawCardPage(this->CardPage, this->CurCardPos);
		};

		if (this->CardSwipe) { // We swipe.
			this->DrawCardPage(this->CardPage, this->CurCardPos); // Draw current page.

			if (this->CardSwipeDir) this->DrawCardPage(this->CardPage - 1, this->PrevCardPos);
			else this->DrawCardPage(this->CardPage + 1, this->NextCardPos);

		} else { // No swipe.
			if (!this->CardSwipeIn && !this->CardSwipeOut) this->DrawCardPage(this->CardPage, 0); // Draw current page only.
		}

		GFX::DrawCornerEdge(true, this->Positions[0].X, this->Positions[0].Y, this->Positions[0].H, this->CardPage > 0);
		GFX::DrawCornerEdge(false, this->Positions[1].X, this->Positions[1].Y, this->Positions[1].H, this->CardCanGoNext());
		Pointer::Draw();

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		};
	}

	GFX::DrawBottom();
	if (this->ModeSwitch) {
		if (this->IsSelecting) { // Switch over to List.
			this->DrawSetList(-320 + this->Cubic);
			this->DrawCardBottom(this->Cubic);

		} else { // Switch over to card flipper.
			this->DrawSetList(0 - this->Cubic);
			this->DrawCardBottom(320 - this->Cubic);
		}

	} else {
		if (this->IsSelecting) { // Set list.
			if (this->InitialSwipe) {
				this->DrawSetList(this->CurSetPos); // Swipe it in.

			} else {
				/* Draw Content. */
				this->DrawSetList(0); // No swipe.
			}

		} else { // Card Logic.
			if (this->SetGood) {
				this->DrawCardBottom(0);
			}
		}
	}

	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
	};

	C3D_FrameEnd(0);
};


/* Main Action. */
std::string CardSetSelector::Action() {
	this->PreviewSelection(0, false, true); // Preview 3DZwei-ROMFS Set.
	Pointer::OnTop = true;
	Pointer::SetPos(0, 0);

	while(aptMainLoop() && !this->FullDone) {
		this->Draw();
		this->Handler();
	};

	if (this->SetGood) {
		if (this->PreviewSheet) C2D_SpriteSheetFree(this->PreviewSheet); // We don't need the Preview anymore, so free the Spritesheet.
	};

	Pointer::OnTop = false;
	Pointer::SetPos(0, 0);
	if (this->Res) return this->CardSets[this->SelectedSet];
	return "";
};


/*
	Handle the Set Overlay Logic.

	const uint32_t Down: The hidKeysDown() variable.
	const uint32_t Held: The hidKeysHeld() variable.
	const touchPosition &T: A reference to the touchPosition variable.
*/
void CardSetSelector::HandleSet(const uint32_t Down, const uint32_t Held, const uint32_t Repeat, const touchPosition &T) {
	if (Down & KEY_B) this->Done = true; // Exit completely.
	Pointer::ScrollHandling(Held, true); // Only Circle-Pad.

	if (Repeat & KEY_DDOWN) {
		this->NextSet();
		if (Pointer::Show) Pointer::Show = false;
	};

	if (Repeat & KEY_DUP) {
		this->LastSet();
		if (Pointer::Show) Pointer::Show = false;
	};

	if (Down & KEY_DRIGHT) {
		if (this->SetGood) {
			this->IsSelecting = false;
			this->ModeSwitch = true;
		}
	};

	if (Down & KEY_START) {
		if (this->SetGood) this->Confirm();
	};

	if (Repeat & KEY_L) this->PrevCardPage();
	if (Repeat & KEY_R) this->NextCardPage();

	if (Repeat & KEY_A) {
		if (Pointer::Show) { // Pointer show -> Handle top.
			for (auto &Pos : this->Positions) {
				if (Pointer::Clicked(Pos, true)) break;
			}

		} else { // Since the pointer is not shown -> Straight go into preview, if good.
			if (this->SetGood) {
				this->IsSelecting = false;
				this->ModeSwitch = true;
			}
		}
	};

	if (Repeat & KEY_TOUCH) {
		for (auto &Pos : this->SetPos) {
			if (Touched(Pos, T, true)) break;
		}
	};

	/* Scroll. */
	if (this->SelectedSet < this->SetListPos) this->SetListPos = this->SelectedSet;
	else if (this->SelectedSet > this->SetListPos + SETS_PER_SCREEN - 1) this->SetListPos = this->SelectedSet - SETS_PER_SCREEN + 1;
};


/*
	Handle the Card Overlay Logic.

	const uint32_t Down: The hidKeysDown() variable.
	const uint32_t Held: The hidKeysHeld() variable.
	const touchPosition &T: A reference to the touchPosition variable.
*/
void CardSetSelector::HandleCard(const uint32_t Down, const uint32_t Held, const uint32_t Repeat, const touchPosition &T) {
	if (Down & KEY_B || Repeat & KEY_DLEFT) this->Cancel(); // Return to set selector.
	Pointer::ScrollHandling(Held, true); // Only with the Circle-Pad.

	if (Repeat & KEY_L) this->PrevCardPage();
	if (Repeat & KEY_R) this->NextCardPage();

	if (Down & KEY_START) {
		if (this->SetGood) this->Confirm();
	};

	if (Repeat & KEY_A) {
		for (auto &Pos : this->Positions) {
			if (Pointer::Clicked(Pos, true)) break;
		}
	};

	if (Repeat & KEY_TOUCH) {
		for (auto &Pos : this->BottomPos) {
			if (Touched(Pos, T, true)) break;
		}
	};
};


/* Handles Effects such as fade and that stuff. */
void CardSetSelector::Handler() {
	hidScanInput();
	touchPosition T;
	hidTouchRead(&T);
	const uint32_t Down = hidKeysDown();
	const uint32_t Held = hidKeysHeld();
	const uint32_t Repeat = hidKeysDownRepeat();

	/* Handle FADE-INs. */
	if (this->FadeIn) {
		if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade()) this->FAlpha = 0, this->FadeIn = false;

		if (this->FAlpha > 0) {
			this->FAlpha -= 5;

			if (this->FAlpha <= 0) this->FadeIn = false;
		}
	};

	/* Handle initial swipe. */
	if (this->InitialSwipe) {
		if (!_3DZwei::CFG->DoAnimation() || Down) {
			this->InitialSwipe = false, this->CurSetPos = 0, this->Cubic = 0.0f;
			return;
		}

		if (this->Cubic < 320.0f) {
			this->Cubic = std::lerp(this->Cubic, 321.0f, 0.2f);
			this->CurSetPos = -320 + this->Cubic;

			if (this->Cubic >= 320.0f) {
				this->Cubic = 0.0f, this->InitialSwipe = false, this->CurSetPos = 0;
			}
		}

		return;
	};


	if (this->CardSwipeIn) {
		if (!_3DZwei::CFG->DoAnimation() || Repeat) {
			this->CardSwipeIn = false, this->CurCardPos = 0, this->Cubic = 0.0f;
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.2f);
			this->CurCardPos = -400 + this->Cubic;

			if (this->Cubic >= 400.0f) {
				this->Cubic = 0.0f, this->CardSwipeIn = false, this->CurCardPos = 0;
			}
		}

		return;
	};

	/* Handle FADE-OUTs. */
	if (this->Done) {
		if (!_3DZwei::CFG->DoAnimation() || !_3DZwei::CFG->DoFade() || Down) this->FullDone = true;

		if (this->FAlpha < 255) {
			this->FAlpha += 5;

			if (this->FAlpha >= 255) this->FullDone = true;
		}

		return;
	};

	/* Handle Mode SWIPEs. */
	if (this->ModeSwitch) {
		if (!_3DZwei::CFG->DoAnimation() || Repeat) {
			this->Cubic = 0.0f, this->ModeSwitch = false;
			return;
		};

		if (this->Cubic < 320.0f) {
			this->Cubic = std::lerp(this->Cubic, 321.0f, 0.2f);

			if (this->Cubic >= 320.0f) this->Cubic = 0.0f, this->ModeSwitch = false;
		}

		return;
	};

	/* Handle CARD FLIPs. */
	if (this->FlipCard && (this->ToFlip != -1) && (this->ToFlip <= (int)this->CardFlipped.size())) { // Focus on card flip.
		if (!this->FirstFlipDone) { // Scale down the back cover -- first operation.
			if (this->Cubic < 1.0f) {
				this->Cubic = std::lerp(this->Cubic, 2.0f, 0.1f);
				this->CardScale[this->ToFlip] = 1.0f - this->Cubic;

				if (this->Cubic >= 1.0f) {
					this->Cubic = 0.0f, this->CardScale[this->ToFlip] = 0.0f;
					this->CardFlipped[this->ToFlip] = !this->CardFlipped[this->ToFlip]; // Set state.
					this->FirstFlipDone = true;
				}
			}

		} else { // Scale up the front cover -- second operation.
			if (this->Cubic < 1.0f) {
				this->Cubic = std::lerp(this->Cubic, 2.0f, 0.1f);
				this->CardScale[this->ToFlip] = this->Cubic;

				if (this->Cubic >= 1.0f) {
					this->Cubic = 0.0f, this->CardScale[this->ToFlip] = 1.0f;
					this->ToFlip = -1, this->FirstFlipDone = false, this->FlipCard = false;
				}
			}
		}

		return; // Do NOT do anything else.
	};

	/* Handle CARD SWIPEs. */
	if (this->CardSwipe) {
		if (!_3DZwei::CFG->DoAnimation() || Repeat) {
			this->CurCardPos = 0.0f, this->PrevCardPos = -400.0f, this->NextCardPos = 400.0f;
			this->Cubic = 0.0f, this->CardSwipe = false;
			this->CardPage = (this->CardSwipeDir ? (this->CardPage - 1) : (this->CardPage + 1));
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.2f);

			if (this->CardSwipeDir) this->CurCardPos = this->Cubic, this->PrevCardPos = -400 + this->Cubic; // -> (Last).
			else this->CurCardPos = 0 - this->Cubic, this->NextCardPos = 400 - this->Cubic; // <- (Next).

			if (this->Cubic >= 400.0f) {
				this->CurCardPos = 0.0f, this->PrevCardPos = -400.0f, this->NextCardPos = 400.0f;
				this->Cubic = 0.0f, this->CardSwipe = false;
				this->CardPage = (this->CardSwipeDir ? (this->CardPage - 1) : (this->CardPage + 1));
			}
		}

		return;
	};

	if (this->IsSelecting) this->HandleSet(Down, Held, Repeat, T);
	else this->HandleCard(Down, Held, Repeat, T);
};