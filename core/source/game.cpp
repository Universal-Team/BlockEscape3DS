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

// Your constructor here for the Game.
Game::Game() {
	this->currentLevel = std::make_unique<Level>();
}

void Game::unload() {
	this->currentLevel->unload();
}

// Load a level.
void Game::loadLevel(const std::string &file) {
	this->currentLevel->loadLevel(file);
}

// Reload a Level.
void Game::reload() {
	this->currentLevel->reload();
}

// Prepare the level here.
void Game::prepareLevel() {
	this->currentLevel->prepareLevel();
}


int Game::getXRow(int bl) {
	return this->currentLevel->getXRow(bl);
}

void Game::setXRow(int bl, int xPos, int yPos, int pos) {
	this->currentLevel->setXRow(bl, xPos, yPos, pos);
}

int Game::getYRow(int bl) {
	return this->currentLevel->getYRow(bl);
}

void Game::setYRow(int bl, int xPos, int yPos, int pos) {
	this->currentLevel->setYRow(bl, xPos, yPos, pos);
}

int Game::getSize(int bl) {
	return this->currentLevel->getSize(bl);
}

Direction Game::getDirection(int bl) {
	return this->currentLevel->getDirection(bl);
}

Blocks Game::getBlock(int bl) {
	return this->currentLevel->getBlock(bl);
}

int Game::getBlockAmount() { return this->currentLevel->getBlockAmount(); }

bool Game::isValid() { return this->currentLevel->isValid(); }

bool Game::returnIfMovable(int bl, bool mv) {
	return this->currentLevel->returnIfMovable(bl, mv);
}