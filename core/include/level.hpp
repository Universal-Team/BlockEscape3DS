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

#ifndef _BLOCK_ESCAPE_3DS_CORE_LEVEL_HPP
#define _BLOCK_ESCAPE_3DS_CORE_LEVEL_HPP

#include "block.hpp"
#include "coreHelper.hpp"
#include <3ds.h>
#include <array>
#include <memory>
#include <vector>

/* BlockEscape3DS Level Structure.
	first 4 bytes: 42 45 33 44 (Magic Header "BE3D").
	5th byte: Direction of first block. (0 = not included, 1 = Vertical, 2 = Horizontal.)
	6th byte: RowX of first block. (0 = not included. 1 - 6: Included.)
	7th byte: RowY of first block. (0 = not included. 1 - 6: Included.)
	8th byte: Repeat same steps as 6 - 8 byte until 0x33.
*/

class Block;
class Level {
public:
	Level(bool useField = true);
	void loadLevel(const std::string &file);
	void createNew();
	void setCreatorStuff();
	void prepareLevel();
	void unload();
	void reload();

	int getXRow(int bl) const;
	void setXRow(int bl, int xPos, int yPos, int pos);
	int getYRow(int bl) const;
	void setYRow(int bl, int xPos, int yPos, int pos);
	int getSize(int bl) const;
	Direction getDirection(int bl) const;
	void setDirection(int bl, int xPos, int yPos, Direction dr);
	Blocks getBlock(int bl) const;
	int getBlockAmount() const;
	bool isValid() const;

	/* Movement stuff here. */
	int getMovement() const { return this->movement; }
	void doMovement() { this->movement++; }
	void resetMovement() { this->movement = 0; }

	bool returnIfMovable(int bl, bool mv); // if movement is true, do forward.

	int returnField(int i) const { return this->gamefield[i].blocktype; }
	Direction returnDirection(int i) const { return this->gamefield[i].direction; }
	int returnIndex(int i) const { return this->gamefield[i].index; }


	std::unique_ptr<u8[]> &levelDt() { return this->levelData; }

	u8* levelPointer() const {
		if (this->levelData) return this->levelData.get() + 0x4;
		else return nullptr;
	}
private:
	bool useField = true;
	std::unique_ptr<u8[]> levelData = nullptr; // Our level buffer.
	FILE *levelFile = nullptr; // Our FILE variable.
	u32 size = 0;
	int movement = 0;
	bool validLevel = false;
	std::vector<std::unique_ptr<Block>> blocks;
	std::array<GameField, 36> gamefield = { -1, Direction::None, -1 };
};

#endif