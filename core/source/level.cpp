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

#include "coreHelper.hpp"
#include "level.hpp"
#include "msg.hpp"

Level::Level(const std::string &file) { this->loadLevel(file); }

void Level::unload() {
	// Reset cars.
	this->cars.clear();
	if (this->levelData) delete levelData; // Delete Level data.
	this->validLevel = false;
}

// Load a level.
void Level::loadLevel(const std::string &file) {
	this->unload(); // Unload before loading.

	this->levelFile = fopen(file.c_str(), "r");

	if (!this->levelFile) {
		printf("Level file could not be opened.");
		this->validLevel = false;
		return;
	}

	fseek(this->levelFile, 0, SEEK_END);
	this->size = ftell(this->levelFile);
	if (this->size != 0x53) {
		printf("Level size is incorrect.");
		fclose(this->levelFile);
		this->validLevel = false;
		return;
	}

	fseek(this->levelFile, 4, SEEK_SET); // skip magic header.
	this->levelData = new u8[0x4F];
	fread(this->levelData, 1, 0x4F, this->levelFile); // Read level data.
	fclose(this->levelFile); // Close cause unneeded.

	this->validLevel = true;

	this->prepareLevel(); // Also do prepare!
}

// Prepare the level here.
void Level::prepareLevel() {
	if (this->validLevel && this->levelData) {
		// 2 size cars.
		for (int i = 0; i < 11; i++) {
			if (this->levelData[0 + (i * 0x5)] != 0) {
				this->cars.push_back({std::make_unique<Cars>(this->levelData[1 + (i * 0x5)], this->levelData[2 + (i * 0x5)], 2, Direction(this->levelData[0 + (i * 0x5)]), Car(i + 1), this->levelData[3 + (i * 0x5)])});
			}
		}

		// 3 size cars.
		for (int i = 0; i < 4; i++) {
			if (this->levelData[0x37 + (i * 0x5)] != 0) {
				this->cars.push_back({std::make_unique<Cars>(this->levelData[0x38 + (i * 0x5)], this->levelData[0x39 + (i * 0x5)], 3, Direction(this->levelData[0x37 + (i * 0x5)]), Car(11 + i), this->levelData[0x3A + (i * 0x5)])});
			}
		}

		// the needed car.
		this->cars.push_back({std::make_unique<Cars>(this->levelData[0x4C], this->levelData[0x4D], 2, Direction(this->levelData[0x4B]), Car::Red, this->levelData[0x4E])});
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

	// Do not do anything, if 6 / 1.
	if (mv) {
		if (this->cars[cr]->getPosition() + this->cars[cr]->getSize() - 1 == 6) return false;
	} else {
		if (this->cars[cr]->getPosition() == 1) return false;
	}

	int distance = 0;

	if (mv) {
		distance = this->cars[cr]->getPosition() + this->cars[cr]->getSize();
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

			// Right.
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

		// Backwards.
	} else {
		distance = this->cars[cr]->getPosition() - 1;
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
	}

	return true;
}