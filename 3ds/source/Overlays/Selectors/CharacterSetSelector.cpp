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
#include "CharacterSetSelector.hpp"
#include "Common.hpp"
#include "Utils.hpp"

#define SETS_PER_SCREEN 5


CharacterSetSelector::CharacterSetSelector() {
	const std::vector<std::string> Tmp = { "t3x" }; // Only accept t3x files.
	std::unique_ptr<Browser> B = std::make_unique<Browser>("sdmc:/3ds/ut-games/sets/characters/", Tmp);
	const std::vector<std::string> S = B->GetList();

	this->CharSets.push_back("3DZwei-RomFS"); // Push back the 3DZwei-RomFS default set. :p
	for (size_t Idx = 0; Idx < S.size(); Idx++) this->CharSets.push_back(S[Idx]);
};


void CharacterSetSelector::PreviewSelection(const size_t Idx, const bool SetSelection, const bool First) {
	/* Clicking on the same thing would switch to the preview. */
	if (SetSelection) {
		if (this->SelectedSet == (int)Idx) { // Is equal -> So it got selected.
			this->IsSelecting = false, this->ModeSwitch = true;
			return;

		} else {
			if (Idx > this->CharSets.size() - 1) return; // Too large.
			this->SelectedSet = Idx;
		}
	};

	if (Idx < this->CharSets.size()) { // Ensure it's smaller than the size.
		if (!First) this->OldCharsetOut();
		this->Idx = 0;
		if (this->PreviewSheet) C2D_SpriteSheetFree(this->PreviewSheet); // Free first.

		if (this->CharSets[Idx] == "3DZwei-RomFS") { // Load from the RomFS.
			this->PreviewSheet = C2D_SpriteSheetLoad("romfs:/gfx/chars.t3x");
			this->SetGood = this->PreviewSheet;

		} else { // Load from the SD otherwise.
			if (this->CharSets[Idx] != "" && Utils::CheckSetContent(this->CharSets[Idx], true)) {
				this->PreviewSheet = C2D_SpriteSheetLoad((std::string("sdmc:/3ds/3DZwei/sets/characters/") + this->CharSets[Idx]).c_str());
				this->SetGood = this->PreviewSheet;
			}
		}

		/* Swipe new selected charset in. */
		this->CurPos = -400;
		this->CharSwipeIn = true;
	};
};


/*
	Swipe the old character set out.
*/
void CharacterSetSelector::OldCharsetOut() {
	bool Done = false;
	this->CharSwipeOut = true;

	while(aptMainLoop() && !Done) {
		this->Draw();

		hidScanInput();
		const uint32_t Down = hidKeysDown();

		if (!_3DZwei::CFG->DoAnimation() || Down) {
			this->CharSwipeOut = false, this->CurPos = 0, this->Cubic = 0.0f;
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.2f);
			this->CurPos = 0 - this->Cubic;

			if (this->Cubic >= 400.0f) {
				this->Cubic = 0.0f, this->CharSwipeOut = false, this->CurPos = 0;
				Done = true;
			}
		}
	};
};


/* Go to the previous character. */
void CharacterSetSelector::PrevChar() {
	if (this->SetGood) {
		if (this->Idx > 0) {
			this->SwipeDir = true;
			this->DoSwipe = true;
		}
	}
};


/* Go to the last characteset. */
void CharacterSetSelector::LastSet() {
	if (this->SelectedSet > 0) {
		this->SelectedSet--;

		this->PreviewSelection(this->SelectedSet);
	}
};


/* Go to the previous characteset page. */
void CharacterSetSelector::PrevSetPage() {
	if (this->SelectedSet > 0) {
		if (this->SelectedSet > SETS_PER_SCREEN)this->SelectedSet -= SETS_PER_SCREEN;
		else this->SelectedSet = 0;

		this->PreviewSelection(this->SelectedSet);
	}
};


/* Go to the next character. */
void CharacterSetSelector::NextChar() {
	if (this->SetGood) {
		if (this->Idx < (int)C2D_SpriteSheetCount(this->PreviewSheet) - 1)  {
			this->SwipeDir = false;
			this->DoSwipe = true;
		}
	}
};


/* Go to the next characteset  page. */
void CharacterSetSelector::NextSet() {
	if (this->SelectedSet < (int)this->CharSets.size() - 1) {
		this->SelectedSet++;

		this->PreviewSelection(this->SelectedSet);
	}
};


/* Go to the next character set. */
void CharacterSetSelector::NextSetPage() {
	if (this->SelectedSet < (int)this->CharSets.size() - 1) {
		if (this->SelectedSet + SETS_PER_SCREEN < (int)this->CharSets.size() - 1) this->SelectedSet += SETS_PER_SCREEN;
		else this->SelectedSet = this->CharSets.size() - 1;

		this->PreviewSelection(this->SelectedSet);
	}
};


/* Return, if a next character is available. */
bool CharacterSetSelector::CanGoNext() const {
	return ((this->Idx + 1) < (int)C2D_SpriteSheetCount(this->PreviewSheet));
};


void CharacterSetSelector::Cancel() { this->IsSelecting = true, this->ModeSwitch = true; };
void CharacterSetSelector::Confirm() { this->Res = true, this->Done = true; };


/*
	Draws a character.

	const int Char: The character index to draw.
	const int AddOffs: The Offsets to add to the base position.
*/
void CharacterSetSelector::DrawCharacter(const int Char, const int AddOffs) {
	if (this->SetGood) {
		if (Char < (int)C2D_SpriteSheetCount(this->PreviewSheet) && Char >= 0) {
			Gui::DrawSprite(this->PreviewSheet, Char, 140 + AddOffs, 72);
			Gui::DrawSprite(GFX::Sprites, sprites_outline_idx, 138 + AddOffs, 70);
		}
	}
};


/*
	Draws a characterset list.

	const int AddOffs: The Offsets to add to the base position.
*/
void CharacterSetSelector::DrawSetList(const int AddOffs) {
	for(size_t Idx = 0; Idx < SETS_PER_SCREEN && Idx < this->CharSets.size(); Idx++) {
		Gui::Draw_Rect(this->SetPos[1 + Idx].X + AddOffs, this->SetPos[1 + Idx].Y, this->SetPos[1 + Idx].W, this->SetPos[1 + Idx].H, (Idx % 2 == 0 ? KBD_KEYPRESSED : KBD_KEYUNPRESSED));
		Gui::DrawStringCentered(0 + AddOffs, this->SetPos[1 + Idx].Y + 8, 0.45f, TEXT_COLOR, this->CharSets[this->SetListPos + Idx], 160);
		GFX::DrawCheckbox(this->SetPos[Idx + 1].X + 194 + AddOffs, this->SetPos[Idx + 1].Y + 4, (this->SetListPos + (int)Idx == this->SelectedSet));
	}

	GFX::DrawCornerEdge(true, this->SetPos[0].X + AddOffs, this->SetPos[0].Y, this->SetPos[0].H, this->SelectedSet > 0);
	GFX::DrawCornerEdge(false, this->SetPos[6].X + AddOffs, this->SetPos[6].Y, this->SetPos[6].H, this->SelectedSet < (int)this->CharSets.size() - 1);
};


/*
	Draws the bottom of the Character Selector.

	const int AddOffs: The Offsets to add to the base position.
*/
void CharacterSetSelector::DrawCharBottom(const int AddOffs) {
	/* Draw Cancel and Confirm buttons. */
	Gui::Draw_Rect(this->BottomPos[2].X + AddOffs, this->BottomPos[2].Y, this->BottomPos[2].W, this->BottomPos[2].H, KBD_KEYPRESSED);
	Gui::DrawStringCentered(-60 + AddOffs, this->BottomPos[2].Y + 15, 0.6f, TEXT_COLOR, Lang::Get("CANCEL"));
	Gui::Draw_Rect(this->BottomPos[3].X + AddOffs, this->BottomPos[3].Y, this->BottomPos[3].W, this->BottomPos[3].H, KBD_KEYPRESSED);
	Gui::DrawStringCentered(60 + AddOffs, this->BottomPos[3].Y + 15, 0.6f, TEXT_COLOR, Lang::Get("CONFIRM"));

	GFX::DrawCornerEdge(true, this->BottomPos[0].X + AddOffs, this->BottomPos[0].Y, this->BottomPos[0].H, this->Idx >= 1);
	GFX::DrawCornerEdge(false, this->BottomPos[1].X + AddOffs, this->BottomPos[1].Y, this->BottomPos[1].H, this->Idx + 1 < (int)C2D_SpriteSheetCount(this->PreviewSheet));
};


void CharacterSetSelector::Draw() {
	Gui::clearTextBufs();
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	GFX::DrawTop();

	if (this->IsSelecting) Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("CHARSET_SELECTOR_TITLE"), 395);
	else Gui::DrawStringCentered(0, 3, 0.6f, TEXT_COLOR, Lang::Get("CHARSET_SELECTOR_CONFIRM"), 395);


	/* Display Preview of the current set. */
	if (this->SetGood) {
		if (this->CharSwipeIn || this->CharSwipeOut) {
			this->DrawCharacter(this->Idx, this->CurPos);
		};

		if (this->DoSwipe) { // We swipe.
			this->DrawCharacter(this->Idx, this->CurPos);
			this->DrawCharacter(this->Idx - 1, this->PrevPos);
			this->DrawCharacter(this->Idx + 1, this->NextPos);

		} else { // No swipe.
			if (!this->CharSwipeIn && !this->CharSwipeOut) this->DrawCharacter(this->Idx, 0); // Draw current page only.
		}

		if (this->PreviewSheet) Gui::DrawStringCentered(0, 200, 0.6f, TEXT_COLOR, Lang::Get("AMOUNT_OF_CHARACTERS") + std::to_string(C2D_SpriteSheetCount(this->PreviewSheet)), 395);
		GFX::DrawCornerEdge(true, this->Positions[0].X, this->Positions[0].Y, this->Positions[0].H, this->Idx > 0);
		GFX::DrawCornerEdge(false, this->Positions[1].X, this->Positions[1].Y, this->Positions[1].H, this->CanGoNext());
		Pointer::Draw();

		if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
			if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, this->FAlpha));
		};
	}

	GFX::DrawBottom();
	if (this->ModeSwitch) {
		if (this->IsSelecting) { // Switch over to List.
			this->DrawSetList(-320 + this->Cubic);
			this->DrawCharBottom(this->Cubic);

		} else {
			this->DrawSetList(0 - this->Cubic);
			this->DrawCharBottom(320 - this->Cubic);
		}

	} else {
		if (this->IsSelecting) { // Set list.
			/* Draw Content. */
			if (this->InitialSwipe) this->DrawSetList(this->CurSetPos); // Swipe it in.
			else this->DrawSetList(0); // No Swipe.

		} else { // Character Logic.
			if (this->SetGood) {
				this->DrawCharBottom(0);
			}
		}
	}

	if (_3DZwei::CFG->DoAnimation() && _3DZwei::CFG->DoFade()) {
		if (this->FAlpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, this->FAlpha));
	};

	C3D_FrameEnd(0);
};


std::string CharacterSetSelector::Action() {
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
	if (this->Res) return this->CharSets[this->SelectedSet];
	return "";
};


void CharacterSetSelector::HandleSet(const uint32_t Down, const uint32_t Held, const touchPosition &T) {
	if (Down & KEY_B) this->Done = true; // Exit completely.
	Pointer::ScrollHandling(Held, true); // Only Circle-Pad.

	if (Down & KEY_DDOWN) this->NextSet();
	if (Down & KEY_DUP) this->LastSet();

	if (Down & KEY_DRIGHT) {
		if (this->SetGood) {
			this->IsSelecting = false, this->ModeSwitch = true;
		}
	};

	if (Down & KEY_START) {
		if (this->SetGood) this->Confirm();
	};

	if (Down & KEY_L) this->PrevChar();
	if (Down & KEY_R) this->NextChar();

	if (Down & KEY_A) {
		for (auto &Pos : this->Positions) {
			if (Pointer::Clicked(Pos, true)) break;
		}
	};

	if (Down & KEY_TOUCH) {
		for (auto &Pos : this->SetPos) {
			if (Touched(Pos, T, true)) break;
		}
	};

	/* Scroll. */
	if (this->SelectedSet < this->SetListPos) this->SetListPos = this->SelectedSet;
	else if (this->SelectedSet > this->SetListPos + SETS_PER_SCREEN - 1) this->SetListPos = this->SelectedSet - SETS_PER_SCREEN + 1;
};


void CharacterSetSelector::HandleChar(const uint32_t Down, const uint32_t Held, const touchPosition &T) {
	if (Down & KEY_B || Down & KEY_DLEFT) this->Cancel(); // Return to set selector.
	Pointer::ScrollHandling(Held, true); // Only with the Circle-Pad.

	if (Down & KEY_L) this->PrevChar();
	if (Down & KEY_R) this->NextChar();

	if (Down & KEY_START) {
		if (this->SetGood) this->Confirm();
	};

	if (Down & KEY_A) {
		for (auto &Pos : this->Positions) {
			if (Pointer::Clicked(Pos, true)) break;
		}
	};

	if (Down & KEY_TOUCH) {
		for (auto &Pos : this->BottomPos) {
			if (Touched(Pos, T, true)) break;
		}
	};
};


/* Main HANDLER. */
void CharacterSetSelector::Handler() {
	hidScanInput();
	touchPosition T;
	hidTouchRead(&T);
	const uint32_t Down = hidKeysDown();
	const uint32_t Held = hidKeysHeld();

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

	if (this->CharSwipeIn) {
		if (!_3DZwei::CFG->DoAnimation() || Down) {
			this->CharSwipeIn = false, this->CurPos = 0, this->Cubic = 0.0f;
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.2f);
			this->CurPos = -400 + this->Cubic;

			if (this->Cubic >= 400.0f) {
				this->Cubic = 0.0f, this->CharSwipeIn = false, this->CurPos = 0;
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
		if (!_3DZwei::CFG->DoAnimation() || Down) {
			this->Cubic = 0.0f, this->ModeSwitch = false;

			return;
		};

		if (this->Cubic < 320.0f) {
			this->Cubic = std::lerp(this->Cubic, 321.0f, 0.2f);

			if (this->Cubic >= 320.0f) this->Cubic = 0.0f, this->ModeSwitch = false;
		}

		return;
	};

	/* Swipe Handler. */
	if (this->DoSwipe) {
		if (!_3DZwei::CFG->DoAnimation() || Down) {
			this->CurPos = 0.0f, this->PrevPos = -400.0f, this->NextPos = 400.0f;
			this->Cubic = 0.0f, this->DoSwipe = false;
			this->Idx = (this->SwipeDir ? (this->Idx - 1) : (this->Idx + 1));
			return;
		}

		if (this->Cubic < 400.0f) {
			this->Cubic = std::lerp(this->Cubic, 401.0f, 0.2f);

			if (this->SwipeDir) this->CurPos = this->Cubic, this->PrevPos = -400 + this->Cubic; // ->.
			else this->CurPos = 0 - this->Cubic, this->NextPos = 400 - this->Cubic; // <-.

			if (this->Cubic >= 400.0f) {
				this->CurPos = 0.0f, this->PrevPos = -400.0f, this->NextPos = 400.0f;
				this->Cubic = 0.0f, this->DoSwipe = false;

				this->Idx = (this->SwipeDir ? (this->Idx - 1) : (this->Idx + 1));
			}
		}

		return;
	};

	if (this->IsSelecting) this->HandleSet(Down, Held, T);
	else this->HandleChar(Down, Held, T);
};