/*
*   This file is part of RushHour3D
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

#ifndef _RUSH_HOUR_3D_CORE_LEVEL_HPP
#define _RUSH_HOUR_3D_CORE_LEVEL_HPP

#include <3ds.h>
#include "car.hpp"
#include "coreHelper.hpp"
#include <memory>
#include <vector>

/* RushHour3D Level Structure.
	first 4 bytes: 52 48 33 44 (Magic Header "RH3D").
	5th byte: Direction of first car. (0 = not included, 1 = Vertical, 2 = Horizontal.)
	6th byte: RowX of first car. (0 = not included. 1 - 6: Included.)
	7th byte: RowY of first car. (0 = not included. 1 - 6: Included.)
	8th byte: Repeat same steps as 6 - 8 byte until 0x33.
*/

class Cars;
class Level {
public:
	Level();
	void loadLevel(const std::string &file);
	void prepareLevel();
	void unload();
	void reload();

	int getXRow(int cr);
	void setXRow(int cr, int xPos, int yPos, int pos);
	int getYRow(int cr);
	void setYRow(int cr, int xPos, int yPos, int pos);
	int getSize(int cr);
	Direction getDirection(int cr);
	Car getCar(int cr);
	int getCarAmount();
	bool isValid();

	// Movement stuff here.
	int getMovement() { return this->movement; }
	void doMovement() { this->movement++; }
	void resetMovement() { this->movement = 0; }
	
	bool returnIfMovable(int cr, bool mv); // if movement is true, do forward.

	int returnField(int i) { return this->gamefield[i].cartype; }
	Direction returnDirection(int i) { return this->gamefield[i].direction; }
	int returnIndex(int i) { return this->gamefield[i].index; }
private:
	const u8* levelPointer() {
		if (this->levelData) return this->levelData.get() + 0x4;
		else return nullptr;
	}

	std::unique_ptr<u8[]> levelData = nullptr; // Our level buffer.
	FILE *levelFile = nullptr; // Our FILE variable.
	u32 size = 0;
	int movement = 0;
	bool validLevel = false;
	std::vector<std::unique_ptr<Cars>> cars;
	std::array<GameField, 36> gamefield = {-1, Direction::None, -1};
};

#endif