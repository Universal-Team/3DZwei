/*
*   This file is part of 3DZwei
*   Copyright (C) 2020-2023 Universal-Team
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
#include "Keyboard.hpp"


/*
	Init the Keyboard.

	const int MaxLength: The max length of characters.
	const std::string &OldString: The old string which would be returned, if the input was invalid.
	const std::string &Text: The Text to display on the top screen.
*/
Keyboard::Keyboard(const int MaxLength, const std::string &OldString, const std::string &Text) {
	this->MaxLength = MaxLength;
	this->Res = OldString;
	this->Text = Text;

	swkbdInit(&this->State, SWKBD_TYPE_NORMAL, 2, this->MaxLength);
	swkbdSetHintText(&this->State, this->Text.c_str());
	swkbdSetValidation(&this->State, SWKBD_NOTBLANK_NOTEMPTY, SWKBD_FILTER_PROFANITY, 0);
}


/* The keyboard action. */
std::string Keyboard::Action() {
	/* Display one frame on top of what should be entered. */
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 3, 0.6f, TEXT_WHITE, this->Text, 395);
	Gui::DrawSprite(GFX::Sprites, sprites_logo_idx, 72, 69);
	GFX::DrawBottom();
	C3D_FrameEnd(0);

	/* Do the keyboard magic. */
	char Input[this->MaxLength + 1] = { '\0' };
	SwkbdButton Ret = swkbdInputText(&this->State, Input, sizeof(Input));
	Input[this->MaxLength] = '\0';
	return (Ret == SWKBD_BUTTON_CONFIRM ? Input : this->Res);
}