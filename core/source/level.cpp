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
	// Reset blocks.
	this->blocks.clear();
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

	static const char MAGIC[4] = {'B','E','3','D'};

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
	// Reset blocks.
	this->blocks.clear();
	this->resetMovement();

	for (int i = 0; i < 36; i++) {
		this->gamefield[i] = {-1, Direction::None, -1};
	}

	this->prepareLevel();
}

// Prepare the level here.
void Level::prepareLevel() {
	if (this->validLevel && this->levelData) {

		// 2 size blocks.
		for (int i = 0; i < 11; i++) {
			if (this->levelPointer()[0 + (i * 0x3)] != 0) {

				if (this->levelPointer()[0 + (i * 0x3)] < STARTPOS || this->levelPointer()[0 + (i * 0x3)] > 2) {
					Msg::DisplayWaitMsg(Lang::get("BLOCK") + " " + std::to_string(1 + i) + " " +  Lang::get("INVALID_DIRECTION"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[1 + (i * 0x3)] < STARTPOS || this->levelPointer()[1 + (i * 0x3)] > GRIDSIZE) {
					Msg::DisplayWaitMsg(Lang::get("BLOCK") + " " + std::to_string(1 + i) + " " +  Lang::get("INVALID_X_ROW"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[2 + (i * 0x3)] < STARTPOS || this->levelPointer()[2 + (i * 0x3)] > GRIDSIZE) {
					Msg::DisplayWaitMsg(Lang::get("BLOCK") + " " + std::to_string(1 + i) + " " +  Lang::get("INVALID_Y_ROW"));
					this->validLevel = false;
					return;
				}


				// Direction check. Vertical.
				if (this->levelPointer()[0 + (i * 0x3)] == 1) {
					this->blocks.push_back({std::make_unique<Block>(this->levelPointer()[1 + (i * 0x3)], this->levelPointer()[2 + (i * 0x3)], 2, Direction(this->levelPointer()[0 + (i * 0x3)]), Blocks(i + 1))});
					for (int i2 = 0; i2 < 2; i2++) {
						this->gamefield[this->levelPointer()[1 + (i * 0x3)] + (((this->levelPointer()[2 + (i * 0x3)]) * 6) - 6) + i2 - 1] = {i, Direction(this->levelPointer()[0 + (i * 0x3)]), this->getBlockAmount()-1};
					}
					
					// Direction check. Horizontal.
				} else if (this->levelPointer()[0 + (i * 0x3)] == 2) {
					this->blocks.push_back({std::make_unique<Block>(this->levelPointer()[1 + (i * 0x3)], this->levelPointer()[2 + (i * 0x3)], 2, Direction(this->levelPointer()[0 + (i * 0x3)]), Blocks(i + 1))});
					for (int i2 = 0; i2 < 2; i2++) {
						this->gamefield[this->levelPointer()[1 + (i * 0x3)] + (((this->levelPointer()[2 + (i * 0x3)]) * 6) - 6) + (i2 * 6) - 1] = {i, Direction(this->levelPointer()[0 + (i * 0x3)]), this->getBlockAmount()-1};
					}
				}
			}
		}

		// 3 size blocks.
		for (int i = 0; i < 4; i++) {
			if (this->levelPointer()[0x21 + (i * 0x3)] != 0) {

				if (this->levelPointer()[0x21 + (i * 0x3)] < STARTPOS || this->levelPointer()[0x21 + (i * 0x3)] > 2) {
					Msg::DisplayWaitMsg(Lang::get("BLOCK") + " " + std::to_string(12 + i) + " " +  Lang::get("INVALID_DIRECTION"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[0x22 + (i * 0x3)] < STARTPOS || this->levelPointer()[0x22 + (i * 0x3)] > GRIDSIZE) {
					Msg::DisplayWaitMsg(Lang::get("BLOCK") + " " + std::to_string(12 + i) + " " +  Lang::get("INVALID_X_ROW"));
					this->validLevel = false;
					return;
				}

				if (this->levelPointer()[0x23 + (i * 0x3)] < STARTPOS || this->levelPointer()[0x23 + (i * 0x3)] > GRIDSIZE) {
					Msg::DisplayWaitMsg(Lang::get("BLOCK") + " " + std::to_string(12 + i) + " " +  Lang::get("INVALID_Y_ROW"));
					this->validLevel = false;
					return;
				}

				// Direction check. Vertical.
				if (this->levelPointer()[0x21 + (i * 0x3)] == 1) {
					this->blocks.push_back({std::make_unique<Block>(this->levelPointer()[0x22 + (i * 0x3)], this->levelPointer()[0x23 + (i * 0x3)], 3, Direction(this->levelPointer()[0x21 + (i * 0x3)]), Blocks(11 + i))});

					for (int i2 = 0; i2 < 3; i2++) {
						this->gamefield[this->levelPointer()[0x22 + (i * 0x3)] + (((this->levelPointer()[0x23 + (i * 0x3)]) * 6) - 6) + i2 - 1] = {11 + i, Direction(this->levelPointer()[0x21 + (i * 0x3)]), this->getBlockAmount()-1};
					}

					// Direction check. Horizontal.
				} else if (this->levelPointer()[0x21 + (i * 0x3)] == 2) {
					this->blocks.push_back({std::make_unique<Block>(this->levelPointer()[0x22 + (i * 0x3)], this->levelPointer()[0x23 + (i * 0x3)], 3, Direction(this->levelPointer()[0x21 + (i * 0x3)]), Blocks(11 + i))});

					for (int i2 = 0; i2 < 3; i2++) {
						this->gamefield[this->levelPointer()[0x22 + (i * 0x3)] + (((this->levelPointer()[0x23 + (i * 0x3)]) * 6) - 6) + (i2 * 6) - 1] = {11 + i, Direction(this->levelPointer()[0x21 + (i * 0x3)]), this->getBlockAmount()-1};
					}

				}
			}
		}

		// the needed block.
		if (this->levelPointer()[0x2D] != 0) {

			if (this->levelPointer()[0x2D] < STARTPOS ||this->levelPointer()[0x2D] > 2) {
				Msg::DisplayWaitMsg(Lang::get("BLOCK") + " " + std::to_string(16) + " " +  Lang::get("INVALID_DIRECTION"));
				this->validLevel = false;
				return;
			}

			if (this->levelPointer()[0x2E] < STARTPOS || this->levelPointer()[0x2E] > GRIDSIZE) {
				Msg::DisplayWaitMsg(Lang::get("BLOCK") + " " + std::to_string(16) + " " +  Lang::get("INVALID_X_ROW"));
				this->validLevel = false;
				return;
			}

			if (this->levelPointer()[0x2F] < STARTPOS || this->levelPointer()[0x2F] > GRIDSIZE) {
				Msg::DisplayWaitMsg(Lang::get("BLOCK") + " " + std::to_string(16) + " " +  Lang::get("INVALID_Y_ROW"));
				this->validLevel = false;
				return;
			}

			// Direction check. Vertical.
			if (this->levelPointer()[0x2D] == 1) {
				this->blocks.push_back({std::make_unique<Block>(this->levelPointer()[0x2E], this->levelPointer()[0x2F], 2, Direction(this->levelPointer()[0x2D]), Blocks::Escape)});

				for (int i2 = 0; i2 < 2; i2++) {
					this->gamefield[this->levelPointer()[0x2E] + (((this->levelPointer()[0x2F]) * 6) - 6) + i2 - 1] = {(int)Blocks::Escape, Direction(this->levelPointer()[0x2D]), this->getBlockAmount()-1};
				}

				// Direction check. Horizontal.
			} else if (this->levelPointer()[0x2D] == 2) {
				this->blocks.push_back({std::make_unique<Block>(this->levelPointer()[0x2E], this->levelPointer()[0x2F], 2, Direction(this->levelPointer()[0x2D]), Blocks::Escape)});

				for (int i2 = 0; i2 < 2; i2++) {
					this->gamefield[this->levelPointer()[0x2E] + (((this->levelPointer()[0x2F]) * 6) - 6) + (i2 * 6) - 1] = {(int)Blocks::Escape, Direction(this->levelPointer()[0x2D]), this->getBlockAmount()-1};
				}

			}
		}
	}
}

int Level::getXRow(int bl) {
	if (this->getBlockAmount() < bl) return 0;

	return this->blocks[bl]->getX();
}

void Level::setXRow(int bl, int xPos, int yPos, int pos) {
	if (this->getBlockAmount() < bl) return;

	// Field stuff.
	if (this->useField) {
		for (int i = 0; i < this->blocks[bl]->getSize(); i++) {
			if (this->blocks[bl]->getX() >= STARTPOS || this->blocks[bl]->getY() >= STARTPOS) {
				if (this->blocks[bl]->getDirection() == Direction::Vertical) {
					this->gamefield[((yPos * GRIDSIZE) - GRIDSIZE) + xPos + i - 1] = {-1, Direction::None, -1};
				} else if (this->blocks[bl]->getDirection() == Direction::Horizontal) {
					this->gamefield[((yPos * GRIDSIZE) - GRIDSIZE) + xPos - 1] = {-1, Direction::None, -1};
				}
			}
		}

		for (int i = 0; i < this->blocks[bl]->getSize(); i++) {
			if (this->blocks[bl]->getDirection() == Direction::Vertical) {
				this->gamefield[((yPos * GRIDSIZE) - GRIDSIZE) + pos + (i - 1)] = {(int)this->blocks[bl]->getBlock(), this->blocks[bl]->getDirection(), bl};
			} else if (this->blocks[bl]->getDirection() == Direction::Horizontal) {
				this->gamefield[((yPos * GRIDSIZE) - GRIDSIZE) + pos - 1] = {(int)this->blocks[bl]->getBlock(), this->blocks[bl]->getDirection(), bl};
			}
		}
	}

	this->blocks[bl]->setX(pos);
}

int Level::getYRow(int bl) {
	if (this->getBlockAmount() < bl) return 0;

	return this->blocks[bl]->getY();
}

void Level::setYRow(int bl, int xPos, int yPos, int pos) {
	if (this->getBlockAmount() < bl) return;

	// Field stuff.
	if (this->useField) {
		for (int i = 0; i < this->blocks[bl]->getSize(); i++) {
			if (this->blocks[bl]->getX() >= STARTPOS || this->blocks[bl]->getY() >= STARTPOS) {
				if (this->blocks[bl]->getDirection() == Direction::Horizontal) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {-1, Direction::None, -1};
				} else if (this->blocks[bl]->getDirection() == Direction::Vertical) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) - 1] = {-1, Direction::None, -1};
				}
			}
		}

		for (int i = 0; i < this->blocks[bl]->getSize(); i++) {
			if (this->blocks[bl]->getDirection() == Direction::Horizontal) {
				this->gamefield[xPos + ((pos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {(int)this->blocks[bl]->getBlock(), this->blocks[bl]->getDirection(), bl};
			} else if (this->blocks[bl]->getDirection() == Direction::Vertical) {
				this->gamefield[xPos + ((pos * GRIDSIZE) - GRIDSIZE) - 1] = {(int)this->blocks[bl]->getBlock(), this->blocks[bl]->getDirection(), bl};
			}
		}
	}

	this->blocks[bl]->setY(pos);
}

int Level::getSize(int bl) {
	if (this->getBlockAmount() < bl) return 0;

	return this->blocks[bl]->getSize();
}

Direction Level::getDirection(int bl) {
	if (this->getBlockAmount() < bl) return Direction::None;

	return this->blocks[bl]->getDirection();
}

void Level::setDirection(int bl, int xPos, int yPos, Direction dr) {
	if (this->getBlockAmount() < bl) return;

	this->blocks[bl]->setDirection(dr);


	// Field stuff.
	if (this->useField) {
		if (dr == Direction::Horizontal) {
			for (int i = 0; i < this->blocks[bl]->getSize(); i++) {
				if (xPos != 0 || yPos != 0) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {-1, Direction::None, -1};
				}
			}

			for (int i = 0; i < this->blocks[bl]->getSize(); i++) {
				if (xPos != 0 || yPos != 0) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {(int)this->blocks[bl]->getBlock(), dr, bl};
				}
			}


		} else if (dr == Direction::Vertical) {
			for (int i = 0; i < this->blocks[bl]->getSize(); i++) {
				if (xPos != 0 || yPos != 0) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {-1, Direction::None, -1};
				}
			}

			for (int i = 0; i < this->blocks[bl]->getSize(); i++) {
				if (xPos != 0 || yPos != 0) {
					this->gamefield[xPos + ((yPos * GRIDSIZE) - GRIDSIZE) + (i * GRIDSIZE) - 1] = {(int)this->blocks[bl]->getBlock(), dr, bl};
				}
			}
		}
	}
}

Blocks Level::getBlock(int bl) {
	if (this->getBlockAmount() < bl) return Blocks::Lock_Invalid;

	return this->blocks[bl]->getBlock();
}

int Level::getBlockAmount() { return (int)this->blocks.size(); }

bool Level::isValid() { return this->validLevel; }

bool Level::returnIfMovable(int bl, bool mv) {
	if (this->getBlockAmount() < bl) return false; // No No.

	int distance = 0;


	// Do not do anything, if 6 / 1.
	if (mv) {
		if (this->blocks[bl]->getDirection() == Direction::Vertical) {
			if (this->blocks[bl]->getX() + this->blocks[bl]->getSize() - 1 == GRIDSIZE) return false;
			distance = this->blocks[bl]->getX() + this->blocks[bl]->getSize();

		} else if (this->blocks[bl]->getDirection() == Direction::Horizontal) {
			if (this->blocks[bl]->getY() + this->blocks[bl]->getSize() - 1 == GRIDSIZE) return false;
			distance = this->blocks[bl]->getY() + this->blocks[bl]->getSize();
		}

	} else {
		if (this->blocks[bl]->getDirection() == Direction::Vertical) {
			if (this->blocks[bl]->getX() == STARTPOS) return false;
			distance = this->blocks[bl]->getX() - 1;

		} else if (this->blocks[bl]->getDirection() == Direction::Horizontal) {
			if (this->blocks[bl]->getY() == STARTPOS) return false;
			distance = this->blocks[bl]->getY() - 1;
		}
	}

	// Up / Down.
	if (this->blocks[bl]->getDirection() == Direction::Horizontal) {
		for (int i = 0; i < this->getBlockAmount(); i++) {
			if (this->blocks[i]->getDirection() == Direction::Vertical) {
				if (this->blocks[i]->getY() == distance) {
					if (this->blocks[i]->getSize() == 2) {
						if (this->blocks[i]->getX() == this->blocks[bl]->getX() || this->blocks[i]->getX() + 1 == this->blocks[bl]->getX()) {
							return false; // Blocked!
						}
					} else if (this->blocks[i]->getSize() == 3) {
						if (this->blocks[i]->getX() == this->blocks[bl]->getX() || this->blocks[i]->getX() + 1 == this->blocks[bl]->getX() || this->blocks[i]->getX() + 2 == this->blocks[bl]->getX()) {
							return false; // Blocked!
						}
					}
				}



			} else if (this->blocks[i]->getDirection() == Direction::Horizontal) {
				if (this->blocks[i]->getX() == this->blocks[bl]->getX()) {
					if (this->blocks[i]->getSize() == 2) {
						if (this->blocks[i]->getY() == distance || this->blocks[i]->getY() + 1 == distance) {
							return false; // Blocked!
						}
					} else if (this->blocks[i]->getSize() == 3) {
						if (this->blocks[i]->getY() == distance || this->blocks[i]->getY() + 1 == distance || this->blocks[i]->getY() + 2 == distance) {
							return false; // Blocked!
						}
					}
				}
			}
		}

		// Right / Left.
	} else if (this->blocks[bl]->getDirection() == Direction::Vertical) {
		for (int i = 0; i < this->getBlockAmount(); i++) {
			if (this->blocks[i]->getDirection() == Direction::Horizontal) {
				if (this->blocks[i]->getX() == distance) {
					if (this->blocks[i]->getSize() == 2) {
						if (this->blocks[i]->getY() == this->blocks[bl]->getY() || this->blocks[i]->getY() + 1 == this->blocks[bl]->getY()) {
							return false; // Blocked!
						}
					} else if (this->blocks[i]->getSize() == 3) {
						if (this->blocks[i]->getY() == this->blocks[bl]->getY() || this->blocks[i]->getY() + 1 == this->blocks[bl]->getY() || this->blocks[i]->getY() + 2 == this->blocks[bl]->getY()) {
							return false; // Blocked!
						}
					}
				}
				
			} else if (this->blocks[i]->getDirection() == Direction::Vertical) {
				if (this->blocks[i]->getY() == this->blocks[bl]->getY()) {
					if (this->blocks[i]->getSize() == 2) {
						if (this->blocks[i]->getX() == distance || this->blocks[i]->getX() + 1 == distance) {
							return false; // Blocked!
						}

					} else if (this->blocks[i]->getSize() == 3) {
						if (this->blocks[i]->getX() == distance || this->blocks[i]->getX() + 1 == distance || this->blocks[i]->getX() + 2 == distance) {
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
	this->levelData[0] = 'B'; this->levelData[1] = 'E'; this->levelData[2] = '3'; this->levelData[3] = 'D';

	// Push all blocks here! :P
	for (int i = 0; i < 11; i++) {
		this->blocks.push_back({std::make_unique<Block>(0, 0, 2, Direction(0), Blocks(1 + i))});
	}

	for (int i = 0; i < 4; i++) {
		this->blocks.push_back({std::make_unique<Block>(0, 0, 3, Direction(0), Blocks(11 + i))});
	}

	this->blocks.push_back({std::make_unique<Block>(0, 0, 2, Direction(0), Blocks::Escape)});

	this->validLevel = true;
}

void Level::setCreatorStuff() {
	for (int i = 0; i < 16; i++) {
		this->levelData[0x4 + (i * 0x3)] = (int)this->blocks[i]->getDirection();
		this->levelData[0x5 + (i * 0x3)] = this->blocks[i]->getX();
		this->levelData[0x6 + (i * 0x3)] = this->blocks[i]->getY();
	}
}