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

#ifndef _BLOCK_ESCAPE_3DS_GAME_SCREEN_HPP
#define _BLOCK_ESCAPE_3DS_GAME_SCREEN_HPP

#include "common.hpp"
#include "game.hpp"
#include "structs.hpp"

class GameScreen : public Screen {
public:
	GameScreen();
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	void DrawBlock(int block) const;
	void DrawSelectedBlock(int block) const;
	std::unique_ptr<Game> currentGame; // Our current game.
	void DrawGameField() const;
	int selectedBlock = 0, mode = 0, subSel = 0;
	int selectedField = 0;
	const std::vector<ButtonStruct> subPos = {
		{90, 40, 140, 40, Lang::get("LOAD_LEVEL")},
		{90, 100, 140, 40, Lang::get("RESTART")},
		{90, 160, 140, 40, Lang::get("EXIT_GAME")}
	};

	const std::vector<Structs::ButtonPos> gridPos = {
		{currentTheme->GridXPos, currentTheme->GridYPos, currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (1 * currentTheme->GridBlockSize), currentTheme->GridYPos, currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (2 * currentTheme->GridBlockSize), currentTheme->GridYPos, currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (3 * currentTheme->GridBlockSize), currentTheme->GridYPos, currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (4 * currentTheme->GridBlockSize), currentTheme->GridYPos, currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (5 * currentTheme->GridBlockSize), currentTheme->GridYPos, currentTheme->GridBlockSize, currentTheme->GridBlockSize},

		{currentTheme->GridXPos, currentTheme->GridYPos + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (1 * currentTheme->GridBlockSize), currentTheme->GridYPos + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (2 * currentTheme->GridBlockSize), currentTheme->GridYPos + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (3 * currentTheme->GridBlockSize), currentTheme->GridYPos + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (4 * currentTheme->GridBlockSize), currentTheme->GridYPos + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (5 * currentTheme->GridBlockSize), currentTheme->GridYPos + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},

		{currentTheme->GridXPos, currentTheme->GridYPos + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (1 * currentTheme->GridBlockSize), currentTheme->GridYPos + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (2 * currentTheme->GridBlockSize), currentTheme->GridYPos + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (3 * currentTheme->GridBlockSize), currentTheme->GridYPos + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (4 * currentTheme->GridBlockSize), currentTheme->GridYPos + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (5 * currentTheme->GridBlockSize), currentTheme->GridYPos + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},

		{currentTheme->GridXPos, currentTheme->GridYPos + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (1 * currentTheme->GridBlockSize), currentTheme->GridYPos + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (2 * currentTheme->GridBlockSize), currentTheme->GridYPos + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (3 * currentTheme->GridBlockSize), currentTheme->GridYPos + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (4 * currentTheme->GridBlockSize), currentTheme->GridYPos + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (5 * currentTheme->GridBlockSize), currentTheme->GridYPos + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},

		{currentTheme->GridXPos, currentTheme->GridYPos + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (1 * currentTheme->GridBlockSize), currentTheme->GridYPos + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (2 * currentTheme->GridBlockSize), currentTheme->GridYPos + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (3 * currentTheme->GridBlockSize), currentTheme->GridYPos + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (4 * currentTheme->GridBlockSize), currentTheme->GridYPos + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (5 * currentTheme->GridBlockSize), currentTheme->GridYPos + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},

		{currentTheme->GridXPos, currentTheme->GridYPos + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (1 * currentTheme->GridBlockSize), currentTheme->GridYPos + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (2 * currentTheme->GridBlockSize), currentTheme->GridYPos + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (3 * currentTheme->GridBlockSize), currentTheme->GridYPos + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (4 * currentTheme->GridBlockSize), currentTheme->GridYPos + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->GridXPos + (5 * currentTheme->GridBlockSize), currentTheme->GridYPos + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize}
	};
};

#endif