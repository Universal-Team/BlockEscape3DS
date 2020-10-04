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

#ifndef _BLOCK_ESCAPE_3DS_CORE_GAME_HPP
#define _BLOCK_ESCAPE_3DS_CORE_GAME_HPP

#include <3ds.h>
#include "coreHelper.hpp"
#include "level.hpp"
#include <memory>

class Level;
class Game {
public:
	Game(); // Our constructor for the game, which will be called on calling the Game's unique_ptr.
	void loadLevel(const std::string &file);
	void prepareLevel();
	void unload();
	void reload();

	int getXRow(int bl) const;
	void setXRow(int bl, int xPos, int yPos, int pos);
	int getYRow(int bl) const;
	void setYRow(int bl, int xPos, int yPos, int pos);
	int getSize(int bl) const;
	Direction getDirection(int bl) const;
	Blocks getBlock(int bl) const;
	int getBlockAmount() const;
	bool isValid() const;

	/* Movement stuff here. */
	int getMovement() const { return this->currentLevel->getMovement(); }
	void doMovement() { this->currentLevel->doMovement(); }
	void resetMovement() { this->currentLevel->resetMovement(); }
	bool returnIfMovable(int bl, bool mv); // if movement is true, do forward.

	/* Gamefield. */
	int returnField(int i) const { return this->currentLevel->returnField(i); }
	Direction returnDirection(int i) const { return this->currentLevel->returnDirection(i); }
	int returnIndex(int i) const { return this->currentLevel->returnIndex(i); }
private:
	std::unique_ptr<Level> currentLevel;
};

#endif