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

Level::Level(bool useField) {
	this->validLevel = false;
	this->useField = useField;
}

void Level::unload() {
	// Reset cars.
	this->cars.clear();
	this->levelData = nullptr;
	this->validLevel = false;
	this->resetMovement();

	for (int i = 0; i < 36; i++) {
		this->gamefield[i] = {-1, Direction::None, -1};
	}
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
	if (this->size != 0x34) {
		Msg::DisplayWaitMsg(Lang::get("LEVEL_SIZE_INCORRECT"));
		fclose(this->levelFile);
		this->validLevel = false;
		return;
	}

	fseek(this->levelFile, 0, SEEK_SET);
	this->levelData = std::unique_ptr<u8[]>(new u8[0x34]);
	fread(this->levelData.get(), 1, 0x34, this->levelFile); // Read level data.
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

void Level::reload() {
	// Reset cars.
	this->cars.clear();
	this->resetMovement();

	for (int i = 0; i < 36; i++) {
		this->gamefield[i] = {-1, Direction::None, -1};
	}

	this->prepareLevel();
}

// Prepare the level here.
void Level::prepareLevel() {
	if (this->validLevel && this->levelData) {

		// 2 size cars.
		for (int i = 0; i < 11; i++) {
			if (this->levelPointer()[0 + (i * 0x3)] != 0) {

				if (this->levelPointer()[0 + (i * 0x3)] < STARTPOS || this->levelPointer()[0 + (i * 0x3)] > 2) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(1 + i) + " " +  Lang::get("INVALID_DIRECTION"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[1 + (i * 0x3)] < STARTPOS || this->levelPointer()[1 + (i * 0x3)] > GRIDSIZE) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(1 + i) + " " +  Lang::get("INVALID_X_ROW"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[2 + (i * 0x3)] < STARTPOS || this->levelPointer()[2 + (i * 0x3)] > GRIDSIZE) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(1 + i) + " " +  Lang::get("INVALID_Y_ROW"));
					this->validLevel = false;
					return;
				}


				// Direction check. Vertical.
				if (this->levelPointer()[0 + (i * 0x3)] == 1) {
					this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[1 + (i * 0x3)], this->levelPointer()[2 + (i * 0x3)], 2, Direction(this->levelPointer()[0 + (i * 0x3)]), Car(i + 1))});
					for (int i2 = 0; i2 < 2; i2++) {
						this->gamefield[this->levelPointer()[1 + (i * 0x3)] + (((this->levelPointer()[2 + (i * 0x3)]) * 6) - 6) + i2 - 1] = {i, Direction(this->levelPointer()[0 + (i * 0x3)]), this->getCarAmount()-1};
					}
					
					// Direction check. Horizontal.
				} else if (this->levelPointer()[0 + (i * 0x3)] == 2) {
					this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[1 + (i * 0x3)], this->levelPointer()[2 + (i * 0x3)], 2, Direction(this->levelPointer()[0 + (i * 0x3)]), Car(i + 1))});
					for (int i2 = 0; i2 < 2; i2++) {
						this->gamefield[this->levelPointer()[1 + (i * 0x3)] + (((this->levelPointer()[2 + (i * 0x3)]) * 6) - 6) + (i2 * 6) - 1] = {i, Direction(this->levelPointer()[0 + (i * 0x3)]), this->getCarAmount()-1};
					}
				}
			}
		}

		// 3 size cars.
		for (int i = 0; i < 4; i++) {
			if (this->levelPointer()[0x21 + (i * 0x3)] != 0) {

				if (this->levelPointer()[0x21 + (i * 0x3)] < STARTPOS || this->levelPointer()[0x21 + (i * 0x3)] > 2) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(12 + i) + " " +  Lang::get("INVALID_DIRECTION"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[0x22 + (i * 0x3)] < STARTPOS || this->levelPointer()[0x22 + (i * 0x3)] > GRIDSIZE) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(12 + i) + " " +  Lang::get("INVALID_X_ROW"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[0x23 + (i * 0x3)] < STARTPOS || this->levelPointer()[0x23 + (i * 0x3)] > GRIDSIZE) {
					Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(12 + i) + " " +  Lang::get("INVALID_Y_ROW"));
					this->validLevel = false;
					return;
				}

				// Direction check. Vertical.
				if (this->levelPointer()[0x21 + (i * 0x3)] == 1) {
					this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[0x22 + (i * 0x3)], this->levelPointer()[0x23 + (i * 0x3)], 3, Direction(this->levelPointer()[0x21 + (i * 0x3)]), Car(11 + i))});

					for (int i2 = 0; i2 < 3; i2++) {
						this->gamefield[this->levelPointer()[0x22 + (i * 0x3)] + (((this->levelPointer()[0x23 + (i * 0x3)]) * 6) - 6) + i2 - 1] = {11 + i, Direction(this->levelPointer()[0x21 + (i * 0x3)]), this->getCarAmount()-1};
					}

					// Direction check. Horizontal.
				} else if (this->levelPointer()[0x21 + (i * 0x3)] == 2) {
					this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[0x22 + (i * 0x3)], this->levelPointer()[0x23 + (i * 0x3)], 3, Direction(this->levelPointer()[0x21 + (i * 0x3)]), Car(11 + i))});

					for (int i2 = 0; i2 < 3; i2++) {
						this->gamefield[this->levelPointer()[0x22 + (i * 0x3)] + (((this->levelPointer()[0x23 + (i * 0x3)]) * 6) - 6) + (i2 * 6) - 1] = {11 + i, Direction(this->levelPointer()[0x21 + (i * 0x3)]), this->getCarAmount()-1};
					}

				}
			}
		}

		// the needed car.
		if (this->levelPointer()[0x2D] != 0) {

			if (this->levelPointer()[0x2D] < STARTPOS ||this->levelPointer()[0x2D] > 2) {
				Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(16) + " " +  Lang::get("INVALID_DIRECTION"));
				this->validLevel = false;
				return;
			}

			if (this->levelPointer()[0x2E] < STARTPOS || this->levelPointer()[0x2E] > GRIDSIZE) {
				Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(16) + " " +  Lang::get("INVALID_X_ROW"));
				this->validLevel = false;
				return;
			}

			if (this->levelPointer()[0x2F] < STARTPOS || this->levelPointer()[0x2F] > GRIDSIZE) {
				Msg::DisplayWaitMsg(Lang::get("CAR") + " " + std::to_string(16) + " " +  Lang::get("INVALID_Y_ROW"));
				this->validLevel = false;
				return;
			}

			// Direction check. Vertical.
			if (this->levelPointer()[0x2D] == 1) {
				this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[0x2E], this->levelPointer()[0x2F], 2, Direction(this->levelPointer()[0x2D]), Car::Red)});

				for (int i2 = 0; i2 < 2; i2++) {
					this->gamefield[this->levelPointer()[0x2E] + (((this->levelPointer()[0x2F]) * 6) - 6) + i2 - 1] = {(int)Car::Red, Direction(this->levelPointer()[0x2D]), this->getCarAmount()-1};
				}

				// Direction check. Horizontal.
			} else if (this->levelPointer()[0x2D] == 2) {
				this->cars.push_back({std::make_unique<Cars>(this->levelPointer()[0x2E], this->levelPointer()[0x2F], 2, Direction(this->levelPointer()[0x2D]), Car::Red)});

				for (int i2 = 0; i2 < 2; i2++) {
					this->gamefield[this->levelPointer()[0x2E] + (((this->levelPointer()[0x2F]) * 6) - 6) + (i2 * 6) - 1] = {(int)Car::Red, Direction(this->levelPointer()[0x2D]), this->getCarAmount()-1};
				}

			}
		}
	}
}

int Level::getXRow(int cr) {
	if (this->getCarAmount() < cr) return 0;

	return this->cars[cr]->getX();
}

void Level::setXRow(int cr, int xPos, int yPos, int pos) {
	if (this->getCarAmount() < cr) return;

	// Field stuff.
	if (this->useField) {
		for (int i = 0; i < this->cars[cr]->getSize(); i++) {
			if (this->cars[cr]->getX() >= STARTPOS || this->cars[cr]->getY() >= STARTPOS) {
				if (this->cars[cr]->getDirection() == Direction::Vertical) {
					this->gamefield[((yPos * GRIDSIZE) - GRIDSIZE) + xPos + i - 1] = {-1, Direction::None, -1};
				} else if (this->cars[cr]->getDirection() == Direction::Horizontal) {
					this->gamefield[((yPos * GRIDSIZE) - GRIDSIZE) + xPos - 1] = {-1, Direction::None, -1};
				}
			}
		}

		for (int i = 0; i < this->cars[cr]->getSize(); i++) {
			if (this->cars[cr]->getDirection() == Direction::Vertical) {
				this->gamefield[((yPos * GRIDSIZE) - GRIDSIZE) + pos + (i - 1)] = {(int)this->cars[cr]->getCar(), this->cars[cr]->getDirection(), cr};
			} else if (this->cars[cr]->getDirection() == Direction::Horizontal) {
				this->gamefield[((yPos * GRIDSIZE) - GRIDSIZE) + pos - 1] = {(int)this->cars[cr]->getCar(), this->cars[cr]->getDirection(), cr};
			}
		}
	}

	this->cars[cr]->setX(pos);
}

int Level::getYRow(int cr) {
	if (this->getCarAmount() < cr) return 0;

	return this->cars[cr]->getY();
}

void Level::setYRow(int cr, int xPos, int yPos, int pos) {
	if (this->getCarAmount() < cr) return;

	// Field stuff.
	if (this->useField) {
		for (int i = 0; i < this->cars[cr]->getSize(); i++) {
			if (this->cars[cr]->getX() >= STARTPOS || this->cars[cr]->getY() >= STARTPOS) {
				if (this->cars[cr]->getDirection() == Direction::Horizontal) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {-1, Direction::None, -1};
				} else if (this->cars[cr]->getDirection() == Direction::Vertical) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) - 1] = {-1, Direction::None, -1};
				}
			}
		}

		for (int i = 0; i < this->cars[cr]->getSize(); i++) {
			if (this->cars[cr]->getDirection() == Direction::Horizontal) {
				this->gamefield[xPos + ((pos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {(int)this->cars[cr]->getCar(), this->cars[cr]->getDirection(), cr};
			} else if (this->cars[cr]->getDirection() == Direction::Vertical) {
				this->gamefield[xPos + ((pos * GRIDSIZE) - GRIDSIZE) - 1] = {(int)this->cars[cr]->getCar(), this->cars[cr]->getDirection(), cr};
			}
		}
	}

	this->cars[cr]->setY(pos);
}

int Level::getSize(int cr) {
	if (this->getCarAmount() < cr) return 0;

	return this->cars[cr]->getSize();
}

Direction Level::getDirection(int cr) {
	if (this->getCarAmount() < cr) return Direction::None;

	return this->cars[cr]->getDirection();
}

void Level::setDirection(int cr, int xPos, int yPos, Direction dr) {
	if (this->getCarAmount() < cr) return;

	this->cars[cr]->setDirection(dr);


	// Field stuff.
	if (this->useField) {
		if (dr == Direction::Horizontal) {
			for (int i = 0; i < this->cars[cr]->getSize(); i++) {
				if (xPos != 0 || yPos != 0) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {-1, Direction::None, -1};
				}
			}

			for (int i = 0; i < this->cars[cr]->getSize(); i++) {
				if (xPos != 0 || yPos != 0) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {(int)this->cars[cr]->getCar(), dr, cr};
				}
			}


		} else if (dr == Direction::Vertical) {
			for (int i = 0; i < this->cars[cr]->getSize(); i++) {
				if (xPos != 0 || yPos != 0) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {-1, Direction::None, -1};
				}
			}

			for (int i = 0; i < this->cars[cr]->getSize(); i++) {
				if (xPos != 0 || yPos != 0) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {(int)this->cars[cr]->getCar(), dr, cr};
				}
			}
		}
	}
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
		if (this->cars[cr]->getDirection() == Direction::Vertical) {
			if (this->cars[cr]->getX() + this->cars[cr]->getSize() - 1 == GRIDSIZE) return false;
			distance = this->cars[cr]->getX() + this->cars[cr]->getSize();

		} else if (this->cars[cr]->getDirection() == Direction::Horizontal) {
			if (this->cars[cr]->getY() + this->cars[cr]->getSize() - 1 == GRIDSIZE) return false;
			distance = this->cars[cr]->getY() + this->cars[cr]->getSize();
		}

	} else {
		if (this->cars[cr]->getDirection() == Direction::Vertical) {
			if (this->cars[cr]->getX() == STARTPOS) return false;
			distance = this->cars[cr]->getX() - 1;

		} else if (this->cars[cr]->getDirection() == Direction::Horizontal) {
			if (this->cars[cr]->getY() == STARTPOS) return false;
			distance = this->cars[cr]->getY() - 1;
		}
	}

	// Up / Down.
	if (this->cars[cr]->getDirection() == Direction::Horizontal) {
		for (int i = 0; i < this->getCarAmount(); i++) {
			if (this->cars[i]->getDirection() == Direction::Vertical) {
				if (this->cars[i]->getY() == distance) {
					if (this->cars[i]->getSize() == 2) {
						if (this->cars[i]->getX() == this->cars[cr]->getX() || this->cars[i]->getX() + 1 == this->cars[cr]->getX()) {
							return false; // Blocked!
						}
					} else if (this->cars[i]->getSize() == 3) {
						if (this->cars[i]->getX() == this->cars[cr]->getX() || this->cars[i]->getX() + 1 == this->cars[cr]->getX() || this->cars[i]->getX() + 2 == this->cars[cr]->getX()) {
							return false; // Blocked!
						}
					}
				}



			} else if (this->cars[i]->getDirection() == Direction::Horizontal) {
				if (this->cars[i]->getX() == this->cars[cr]->getX()) {
					if (this->cars[i]->getSize() == 2) {
						if (this->cars[i]->getY() == distance || this->cars[i]->getY() + 1 == distance) {
							return false; // Blocked!
						}
					} else if (this->cars[i]->getSize() == 3) {
						if (this->cars[i]->getY() == distance || this->cars[i]->getY() + 1 == distance || this->cars[i]->getY() + 2 == distance) {
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
						if (this->cars[i]->getY() == this->cars[cr]->getY() || this->cars[i]->getY() + 1 == this->cars[cr]->getY()) {
							return false; // Blocked!
						}
					} else if (this->cars[i]->getSize() == 3) {
						if (this->cars[i]->getY() == this->cars[cr]->getY() || this->cars[i]->getY() + 1 == this->cars[cr]->getY() || this->cars[i]->getY() + 2 == this->cars[cr]->getY()) {
							return false; // Blocked!
						}
					}
				}
				
			} else if (this->cars[i]->getDirection() == Direction::Vertical) {
				if (this->cars[i]->getY() == this->cars[cr]->getY()) {
					if (this->cars[i]->getSize() == 2) {
						if (this->cars[i]->getX() == distance || this->cars[i]->getX() + 1 == distance) {
							return false; // Blocked!
						}

					} else if (this->cars[i]->getSize() == 3) {
						if (this->cars[i]->getX() == distance || this->cars[i]->getX() + 1 == distance || this->cars[i]->getX() + 2 == distance) {
							return false; // Blocked!
						}
					}
				}
			}
		}
	}

	return true;
}

// Create a new Level!
void Level::createNew() {
	this->unload();

	this->levelData = std::unique_ptr<u8[]>(new u8[0x34]);
	this->levelData[0] = 'R'; this->levelData[1] = 'H'; this->levelData[2] = '3'; this->levelData[3] = 'D';

	// Push all cars here! :P
	for (int i = 0; i < 11; i++) {
		this->cars.push_back({std::make_unique<Cars>(0, 0, 2, Direction(0), Car(1 + i))});
	}

	for (int i = 0; i < 4; i++) {
		this->cars.push_back({std::make_unique<Cars>(0, 0, 3, Direction(0), Car(11 + i))});
	}

	this->cars.push_back({std::make_unique<Cars>(0, 0, 2, Direction(0), Car::Red)});

	this->validLevel = true;
}

void Level::setCreatorStuff() {
	for (int i = 0; i < 16; i++) {
		this->levelData[0x4 + (i * 0x3)] = (int)this->cars[i]->getDirection();
		this->levelData[0x5 + (i * 0x3)] = this->cars[i]->getX();
		this->levelData[0x6 + (i * 0x3)] = this->cars[i]->getY();
	}
}