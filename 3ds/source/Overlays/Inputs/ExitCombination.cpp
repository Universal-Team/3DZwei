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
#include "ExitCombination.hpp"
#include "Utils.hpp"

uint32_t ExitCombination::Action() {
	while(aptMainLoop() && !Done) {
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		GFX::DrawTop();
		Gui::DrawStringCentered(0, 3, 0.6f, TEXT_WHITE, Lang::Get("EXIT_COMBINATION_TITLE"), 395);
		Gui::Draw_Rect(0, 215, 400, 25, BAR_BLUE);
		Gui::DrawStringCentered(0, 218, 0.6f, TEXT_WHITE, Lang::Get("EXIT_COMBINATION_TEXT") + Utils::GetCombiString(this->Res), 395);
		Gui::DrawSprite(GFX::Sprites, sprites_logo_idx, 72, 57);
		GFX::DrawBottom();
		C3D_FrameEnd(0);

		hidScanInput();
		this->Res = hidKeysHeld();
		this->Delay--;
		if (this->Delay == 0) this->Done = true;
	}

	return this->Res;
};