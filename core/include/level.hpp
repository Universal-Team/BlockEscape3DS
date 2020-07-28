/*
*   This file is part of RushHour3D
*   Copyright (C) 2020 SuperSaiyajinStackZ
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

class Cars;
class Level {
public:
	Level();
	void loadLevel(const std::string &file);
	void prepareLevel();
	void unload();

	int getXRow(int cr);
	int getYRow(int cr);
	int getSize(int cr);
	int getPosition(int cr);
	void setPosition(int cr, int pos);
	Direction getDirection(int cr);
	Car getCar(int cr);
	int getCarAmount();
	bool isValid();

	bool returnIfMovable(int cr, bool mv); // if movement is true, do forward.
private:
	const u8* levelPointer() {
		if (this->levelData) return this->levelData.get() + 0x4;
		else return nullptr;
	}

	std::unique_ptr<u8[]> levelData = nullptr; // Our level buffer.
	FILE *levelFile = nullptr; // Our FILE variable.
	u32 size = 0;

	bool validLevel = false;

	std::vector<std::unique_ptr<Cars>> cars;
};

#endif