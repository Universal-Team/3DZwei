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

#ifndef _3DZWEI_MSG_HPP
#define _3DZWEI_MSG_HPP

#include <string>

namespace Msg {
	/* Display a Warn Message for about 2 seconds. */
	void DisplayWarnMsg(std::string Text);

	/* Display a Warn Message for about 2 seconds. Used for more text. */
	void DisplayWarnMsg2(std::string Text);

	/* This will be used for the prompt Messages, which needs confirmation with A or cancel with B. */
	bool promptMsg(std::string promptMsg);

	/* Will Display a Message, which needs a confirmation with A. */
	void DisplayWaitMsg(std::string waitMsg, ...);

	/* Display a not Implemented Message. */
	void NotImplementedYet(void);

	/* Display just a Message. */
	void DisplayMsg(std::string Message);

	/* Helper Box. */
	void HelperBox(std::string Msg);

	/* Displays a message, which does not rely on the config. */
	void DisplayNoConfig(std::string Text);

	/* Debug Message for me, StackZ, to debug the core. */
	void DebugMessage(std::string msg);
};

#endif