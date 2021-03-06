/*
*   This file is part of BlockEscape3DS
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

#ifndef _BLOCK_ESCAPE_3DS_MAINMENU_HPP
#define _BLOCK_ESCAPE_3DS_MAINMENU_HPP

#include "common.hpp"
#include "structs.hpp"

#include <vector>

class MainMenu : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	int Selection = 0;

	const std::vector<ButtonStruct> mainButtons = {
		{10, 70, 140, 40, "NEW_GAME"}, // New Game.
		{170, 70, 140, 40, "LEVEL_CREATOR"}, // Level Creator.
		{10, 145, 140, 40, "UI_SETTINGS"}, // UI Settings.
		{170, 145, 140, 40, "?"} // ?.
	};
};

#endif