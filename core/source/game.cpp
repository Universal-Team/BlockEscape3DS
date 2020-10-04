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
#include "game.hpp"
#include "msg.hpp"

Game::Game() { this->currentLevel = std::make_unique<Level>(); }

/*
	Unload a level.
*/
void Game::unload() { this->currentLevel->unload(); }

/*
	Load a level.

	const std::string &file: path to the level file.
*/
void Game::loadLevel(const std::string &file) { this->currentLevel->loadLevel(file); }

/*
	Reload a level.
*/
void Game::reload() { this->currentLevel->reload(); }

/*
	Prepare a level.
*/
void Game::prepareLevel() { this->currentLevel->prepareLevel(); }

/*
	Return the X Row from a block.

	int bl: The block-Index.
*/
int Game::getXRow(int bl) const { return this->currentLevel->getXRow(bl); }

/*
	Set the X Row from a block.

	int bl: The block-Index.
	int xPos: The X Position of the block.
	int yPos : The Y Position of the block.
	int pos: The "index" which is forward | backward.
*/
void Game::setXRow(int bl, int xPos, int yPos, int pos) { this->currentLevel->setXRow(bl, xPos, yPos, pos); }

/*
	Return the Y Row from a block.

	int bl: The block-Index.
*/
int Game::getYRow(int bl) const { return this->currentLevel->getYRow(bl); }

/*
	Set the Y Row from a block.

	int bl: The block-Index.
	int xPos: The X Position of the block.
	int yPos : The Y Position of the block.
	int pos: The "index" which is forward | backward.
*/
void Game::setYRow(int bl, int xPos, int yPos, int pos) { this->currentLevel->setYRow(bl, xPos, yPos, pos); }

/*
	Return the size of a block.

	int bl: The block-index.
*/
int Game::getSize(int bl) const { return this->currentLevel->getSize(bl); }

/*
	Return the Direction of a block.

	int bl: The block-index.
*/
Direction Game::getDirection(int bl) const { return this->currentLevel->getDirection(bl); }

/*
	Return the Blocktype of a block.

	int bl: The block-index.
*/
Blocks Game::getBlock(int bl) const { return this->currentLevel->getBlock(bl); }

/*
	Return the used Block Amount.
*/
int Game::getBlockAmount() const { return this->currentLevel->getBlockAmount(); }

/*
	Return if the current level is valid.
*/
bool Game::isValid() const { return this->currentLevel->isValid(); }

/*
	Return if a block can move.

	int bl: The block-index.
	bool mv: forward (true) or backward (false).
*/
bool Game::returnIfMovable(int bl, bool mv) { return this->currentLevel->returnIfMovable(bl, mv); }