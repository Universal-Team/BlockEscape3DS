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

// Prepare the level here.
void Game::prepareLevel() {
	this->currentLevel->prepareLevel();
}


int Game::getXRow(int cr) {
	return this->currentLevel->getXRow(cr);
}

int Game::getYRow(int cr) {
	return this->currentLevel->getYRow(cr);
}

int Game::getSize(int cr) {
	return this->currentLevel->getSize(cr);
}

int Game::getPosition(int cr) {
	return this->currentLevel->getPosition(cr);
}

void Game::setPosition(int cr, int pos) {
	this->currentLevel->setPosition(cr, pos);
}

Direction Game::getDirection(int cr) {
	return this->currentLevel->getDirection(cr);
}

Car Game::getCar(int cr) {
	return this->currentLevel->getCar(cr);
}

int Game::getCarAmount() { return this->currentLevel->getCarAmount(); }

bool Game::isValid() { return this->currentLevel->isValid(); }

bool Game::returnIfMovable(int cr, bool mv) {
	return this->currentLevel->returnIfMovable(cr, mv);
}