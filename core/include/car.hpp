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

#ifndef _RUSH_HOUR_3D_CORE_CAR_HPP
#define _RUSH_HOUR_3D_CORE_CAR_HPP

#include <3ds.h>
#include "coreHelper.hpp"
#include <memory>

class Cars {
public:
	Cars(int xRow, int yRow, int size, Direction direction, Car index) {
		this->rowX = xRow;
		this->rowY = yRow;
		this->size = size;
		this->direction = direction;
		this->index = index;
	}

	int getX() { return this->rowX; }
	void setX(int x) { this->rowX = x; }
	int getY() { return this->rowY; }
	void setY(int y) { this->rowY = y; }
	int getSize() { return this->size; }
	void setSize(int sz) { this->size = sz; }
	Direction getDirection() { return this->direction; }
	void setDirection(Direction dr) { this->direction = dr; }
	Car getCar() { return this->index; }
private:
	int rowX, rowY, size;
	Car index;
	Direction direction;
};

#endif