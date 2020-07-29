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

#include "coreHelper.hpp"
#include "lang.hpp"
#include "level.hpp"
#include "msg.hpp"

#include <cstring>

Level::Level() { this->validLevel = false; }

void Level::unload() {
	// Reset cars.
	this->cars.clear();
	this->levelData = nullptr;
	this->validLevel = false;
	this->resetMovement();
}

// Load a level.
void Level::loadLevel(const std::string &file) {
	this->unload(); // Unload before loading.

	this->levelFile = fopen(file.c_str(), "r");

	if (!this->levelFile) {
		Msg::DisplayWaitMsg(Lang::get("FILE_NOT_OPEN"));
		this->validLevel = false;
		return;
	}

	fseek(this->levelFile, 0, SEEK_END);
	this->size = ftell(this->levelFile);
	if (this->size != 0x43) {
		Msg::DisplayWaitMsg(Lang::get("LEVEL_SIZE_INCORRECT"));
		fclose(this->levelFile);
		this->validLevel = false;
		return;
	}

	fseek(this->levelFile, 0, SEEK_SET);
	this->levelData = std::unique_ptr<u8[]>(new u8[0x43]);
	fread(this->levelData.get(), 1, 0x43, this->levelFile); // Read level data.
	fclose(this->levelFile); // Close cause unneeded.

	static const char MAGIC[4] = {'R','H','3','D'};

	// Verify Magic header.
	if (memcmp(MAGIC, this->levelData.get(), 0x4) != 0) {
		Msg::DisplayWaitMsg(Lang::get("LEVEL_INVALID_MAGIC"));
		this->validLevel = false;
		return;
	}

	this->validLevel = true;

	this->prepareLevel(); // Also do prepare!
}

// Prepare the level here.
void Level::prepareLevel() {
	if (this->validLevel && this->levelData) {

		// 2 size cars.
		for (int i = 0; i < 11; i++) {
			if (this->levelPointer()[0 + (i * 0x4)] != 0) {

				if (this->levelPointer()[0 + (i * 0x4)] < 1 || this->levelPointer()[0 + (i * 0x4)] > 2) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(1 + i) + " " +  Lang::get("INVALID_DIRECTION"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[1 + (i * 0x4)] < 1 || this->levelPointer()[1 + (i * 0x4)] > 6) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(1 + i) + " " +  Lang::get("INVALID_X_ROW"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[2 + (i * 0x4)] < 1 || this->levelPointer()[2 + (i * 0x4)] > 6) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(1 + i) + " " +  Lang::get("INVALID_Y_ROW"));
					this->validLevel = false;
					return;
				}


				// Direction check. Vertical.
				if (this->levelPointer()[0 + (i * 0x4)] == 1) {
					this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[1 + (i * 0x4)], this->levelPointer()[2 + (i * 0x4)], 2, Direction(this->levelPointer()[0 + (i * 0x4)]), Car(i + 1), this->levelPointer()[1 + (i * 0x4)])});
					// Direction check. Horizontal.
				} else if (this->levelPointer()[0 + (i * 0x4)] == 2) {
					this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[1 + (i * 0x4)], this->levelPointer()[2 + (i * 0x4)], 2, Direction(this->levelPointer()[0 + (i * 0x4)]), Car(i + 1), this->levelPointer()[2 + (i * 0x4)])});
				}
			}
		}

		// 3 size cars.
		for (int i = 0; i < 4; i++) {
			if (this->levelPointer()[0x2C + (i * 0x4)] != 0) {

				if (this->levelPointer()[0x2C + (i * 0x4)] < 1 || this->levelPointer()[0x2C + (i * 0x4)] > 2) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(12 + i) + " " +  Lang::get("INVALID_DIRECTION"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[0x2D + (i * 0x4)] < 1 || this->levelPointer()[0x2D + (i * 0x4)] > 6) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(12 + i) + " " +  Lang::get("INVALID_X_ROW"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[0x2E + (i * 0x4)] < 1 || this->levelPointer()[0x2E + (i * 0x4)] > 6) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(12 + i) + " " +  Lang::get("INVALID_Y_ROW"));
					this->validLevel = false;
					return;
				}

				// Direction check. Vertical.
				if (this->levelPointer()[0x2C + (i * 0x4)] == 1) {
					this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[0x2D + (i * 0x4)], this->levelPointer()[0x2E + (i * 0x4)], 3, Direction(this->levelPointer()[0x2C + (i * 0x4)]), Car(11 + i), this->levelPointer()[0x2D + (i * 0x4)])});
					// Direction check. Horizontal.
				} else if (this->levelPointer()[0x2C + (i * 0x4)] == 2) {
					this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[0x2D + (i * 0x4)], this->levelPointer()[0x2E + (i * 0x4)], 3, Direction(this->levelPointer()[0x2C + (i * 0x4)]), Car(11 + i), this->levelPointer()[0x2E + (i * 0x4)])});
				}
			}
		}

		// the needed car.
		if (this->levelPointer()[0x3C] != 0) {

			if (this->levelPointer()[0x3C] < 1 ||this->levelPointer()[0x3C] > 2) {
				Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(16) + " " +  Lang::get("INVALID_DIRECTION"));
				this->validLevel = false;
				return;
			}

			if (this->levelPointer()[0x3D] < 1 || this->levelPointer()[0x3D] > 6) {
				Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(16) + " " +  Lang::get("INVALID_X_ROW"));
				this->validLevel = false;
				return;
			}

			if (this->levelPointer()[0x3E] < 1 || this->levelPointer()[0x3E] > 6) {
				Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(16) + " " +  Lang::get("INVALID_Y_ROW"));
				this->validLevel = false;
				return;
			}

			// Direction check. Vertical.
			if (this->levelPointer()[0x3C] == 1) {
				this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[0x3D], this->levelPointer()[0x3E], 2, Direction(this->levelPointer()[0x3C]), Car::Red, this->levelPointer()[0x3D])});
				// Direction check. Horizontal.
			} else if (this->levelPointer()[0x3C] == 2) {
				this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[0x3D], this->levelPointer()[0x3E], 2, Direction(this->levelPointer()[0x3C]), Car::Red, this->levelPointer()[0x3E])});
			}
		}
	}
}

int Level::getXRow(int cr) {
	if (this->getCarAmount() < cr) return 0;

	return this->cars[cr]->getX();
}

int Level::getYRow(int cr) {
	if (this->getCarAmount() < cr) return 0;

	return this->cars[cr]->getY();
}

int Level::getSize(int cr) {
	if (this->getCarAmount() < cr) return 0;

	return this->cars[cr]->getSize();
}

int Level::getPosition(int cr) {
	if (this->getCarAmount() < cr) return 0;

	return this->cars[cr]->getPosition();
}

void Level::setPosition(int cr, int pos) {
	if (this->getCarAmount() < cr) return;

	if (this->cars[cr]->getSize() == 2) {
		if (pos > 6 || pos < 1) return;
		this->cars[cr]->setPosition(pos);
	} else if (this->cars[cr]->getSize() == 3) {
		if (pos > 4 || pos < 1) return;
		this->cars[cr]->setPosition(pos);
	}
}

Direction Level::getDirection(int cr) {
	if (this->getCarAmount() < cr) return Direction::None;

	return this->cars[cr]->getDirection();
}

Car Level::getCar(int cr) {
	if (this->getCarAmount() < cr) return Car::Lock_Invalid;

	return this->cars[cr]->getCar();
}

int Level::getCarAmount() { return (int)this->cars.size(); }

bool Level::isValid() { return this->validLevel; }

bool Level::returnIfMovable(int cr, bool mv) {
	if (this->getCarAmount() < cr) return false; // No No.

	int distance = 0;

	// Do not do anything, if 6 / 1.
	if (mv) {
		if (this->cars[cr]->getPosition() + this->cars[cr]->getSize() - 1 == 6) return false;
		distance = this->cars[cr]->getPosition() + this->cars[cr]->getSize();
	} else {
		if (this->cars[cr]->getPosition() == 1) return false;
		distance = this->cars[cr]->getPosition() - 1;
	}

	// Up / Down.
	if (this->cars[cr]->getDirection() == Direction::Horizontal) {
		for (int i = 0; i < this->getCarAmount(); i++) {
			if (this->cars[i]->getDirection() == Direction::Vertical) {
				if (this->cars[i]->getY() == distance) {
					if (this->cars[i]->getSize() == 2) {
						if (this->cars[i]->getPosition() == this->cars[cr]->getX() || this->cars[i]->getPosition() + 1 == this->cars[cr]->getX()) {
							return false; // Blocked!
						}
					} else if (this->cars[i]->getSize() == 3) {
						if (this->cars[i]->getPosition() == this->cars[cr]->getX() || this->cars[i]->getPosition() + 1 == this->cars[cr]->getX() || this->cars[i]->getPosition() + 2 == this->cars[cr]->getX()) {
							return false; // Blocked!
						}
					}
				}

			} else if (this->cars[i]->getDirection() == Direction::Horizontal) {
				if (this->cars[i]->getX() == this->cars[cr]->getX()) {
					if (this->cars[i]->getSize() == 2) {
						if (this->cars[i]->getPosition() == distance || this->cars[i]->getPosition() + 1 == distance) {
							return false; // Blocked!
						}
					} else if (this->cars[i]->getSize() == 3) {
						if (this->cars[i]->getPosition() == distance || this->cars[i]->getPosition() + 1 == distance || this->cars[i]->getPosition() + 2 == distance) {
							return false; // Blocked!
						}
					}
				}
			}
		}

		// Right / Left.
	} else if (this->cars[cr]->getDirection() == Direction::Vertical) {
		for (int i = 0; i < this->getCarAmount(); i++) {
			if (this->cars[i]->getDirection() == Direction::Horizontal) {
				if (this->cars[i]->getX() == distance) {
					if (this->cars[i]->getSize() == 2) {
						if (this->cars[i]->getPosition() == this->cars[cr]->getY() || this->cars[i]->getPosition() + 1 == this->cars[cr]->getY()) {
							return false; // Blocked!
						}
					} else if (this->cars[i]->getSize() == 3) {
						if (this->cars[i]->getPosition() == this->cars[cr]->getY() || this->cars[i]->getPosition() + 1 == this->cars[cr]->getY() || this->cars[i]->getPosition() + 2 == this->cars[cr]->getY()) {
							return false; // Blocked!
						}
					}
				}
				
			} else if (this->cars[i]->getDirection() == Direction::Vertical) {
				if (this->cars[i]->getY() == this->cars[cr]->getY()) {
					if (this->cars[i]->getSize() == 2) {
						if (this->cars[i]->getPosition() == distance || this->cars[i]->getPosition() + 1 == distance) {
							return false; // Blocked!
						}

					} else if (this->cars[i]->getSize() == 3) {
						if (this->cars[i]->getPosition() == distance || this->cars[i]->getPosition() + 1 == distance || this->cars[i]->getPosition() + 2 == distance) {
							return false; // Blocked!
						}
					}
				}
			}
		}
	}

	return true;
}