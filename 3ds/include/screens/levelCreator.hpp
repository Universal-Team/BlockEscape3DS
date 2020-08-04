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

#ifndef _BLOCK_ESCAPE_3DS_LEVEL_CREATOR_HPP
#define _BLOCK_ESCAPE_3DS_LEVEL_CREATOR_HPP

#include "common.hpp"
#include "level.hpp"
#include "structs.hpp"

class LevelCreator : public Screen {
public:
	LevelCreator();
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	void DrawBlock(int block) const;
	std::unique_ptr<Level> level;
	void DrawGameField() const;
	const std::string getSelectMsg() const;
	int X = 0, Y = 0;
	

		/* Modes. */
	/*
		0 -> Block Select.
		1 -> Direction Select.
		2 -> YRow Select.
		3 -> XRow Select.
	*/
	int selectedMode = 0;
	int selectedBlock = 0;

	const std::vector<Structs::ButtonPos> blockListNew = {
		{currentTheme->creatorXPos + 2, currentTheme->creatorYPos, currentTheme->GridBlockSize + currentTheme->GridBlockSize, currentTheme->GridBlockSize + currentTheme->GridBlockSize },
		{currentTheme->creatorXPos + 2, currentTheme->creatorYPos + currentTheme->GridBlockSize + 5, currentTheme->GridBlockSize + currentTheme->GridBlockSize + currentTheme->GridBlockSize, currentTheme->GridBlockSize + currentTheme->GridBlockSize + currentTheme->GridBlockSize },
		{currentTheme->creatorXPos + 2, currentTheme->creatorYPos + currentTheme->GridBlockSize + currentTheme->GridBlockSize + 10, currentTheme->GridBlockSize + currentTheme->GridBlockSize, currentTheme->GridBlockSize + currentTheme->GridBlockSize }
	};

	// Block Selection.
	const std::vector<Structs::ButtonPos> blockList = {
		{240, 2, 60, 10},
		{240, 17, 60, 10},
		{240, 32, 60, 10},
		{240, 47, 60, 10},
		{240, 62, 60, 10},
		{240, 77, 60, 10},
		{240, 92, 60, 10},
		{240, 107, 60, 10},
		{240, 122, 60, 10},
		{240, 137, 60, 10},
		{240, 152, 60, 10},
		{240, 167, 60, 10},
		{240, 182, 60, 10},
		{240, 197, 60, 10},
		{240, 212, 60, 10},
		{240, 227, 60, 10}
	};

	const Structs::ButtonPos saveIcon = {currentTheme->creatorSaveX, currentTheme->creatorSaveY, 25, 25};

	const std::vector<Structs::ButtonPos> gridPos = {
		{currentTheme->creatorGridX, currentTheme->creatorGridY, currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (1 * currentTheme->GridBlockSize), currentTheme->creatorGridY, currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (2 * currentTheme->GridBlockSize), currentTheme->creatorGridY, currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (3 * currentTheme->GridBlockSize), currentTheme->creatorGridY, currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (4 * currentTheme->GridBlockSize), currentTheme->creatorGridY, currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (5 * currentTheme->GridBlockSize), currentTheme->creatorGridY, currentTheme->GridBlockSize, currentTheme->GridBlockSize},

		{currentTheme->creatorGridX, currentTheme->creatorGridY + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (1 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (2 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (3 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (4 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (5 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (1 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},

		{currentTheme->creatorGridX, currentTheme->creatorGridY + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (1 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (2 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (3 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (4 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (5 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (2 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},

		{currentTheme->creatorGridX, currentTheme->creatorGridY + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (1 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (2 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (3 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (4 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (5 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (3 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},

		{currentTheme->creatorGridX, currentTheme->creatorGridY + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (1 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (2 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (3 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (4 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (5 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (4 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},

		{currentTheme->creatorGridX, currentTheme->creatorGridY + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (1 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (2 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (3 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (4 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize},
		{currentTheme->creatorGridX + (5 * currentTheme->GridBlockSize), currentTheme->creatorGridY + (5 * currentTheme->GridBlockSize), currentTheme->GridBlockSize, currentTheme->GridBlockSize}
	};
};

#endif