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

#include "config.hpp"
#include "gameScreen.hpp"

extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

GameScreen::GameScreen() {
	// Call Game Constructor here.
	this->currentGame = std::make_unique<Game>();
	this->currentGame->loadLevel("romfs:/levels/level1.lvl");
}

void GameScreen::DrawCar(int car) const {
	if (this->currentGame->getDirection(car) != Direction::None || this->currentGame->getCar(car) != Car::Lock_Invalid) {
		if (this->currentGame->getDirection(car) == Direction::Vertical) {
			Gui::Draw_Rect((30 * this->currentGame->getPosition(car)), ((30 * this->currentGame->getYRow(car)) -1), this->currentGame->getSize(car) * 30, (30), GFX::getColor(this->currentGame->getCar(car)));
		} else if (this->currentGame->getDirection(car) == Direction::Horizontal) {
			Gui::Draw_Rect(((30 * this->currentGame->getXRow(car)) - 1), (30 * this->currentGame->getPosition(car)), 30, (this->currentGame->getSize(car) * 30), GFX::getColor(this->currentGame->getCar(car)));
		}
	}
}

void GameScreen::DrawSelectedCar(int car) const {
	if (this->currentGame->getDirection(car) != Direction::None || this->currentGame->getCar(car) != Car::Lock_Invalid) {
		if (this->currentGame->getDirection(car) == Direction::Vertical) {
			Gui::drawGrid((30 * this->currentGame->getPosition(car)), ((30 * this->currentGame->getYRow(car)) -1), this->currentGame->getSize(car) * 30, (30), C2D_Color32(255, 255, 255, 255));
		} else if (this->currentGame->getDirection(car) == Direction::Horizontal) {
			Gui::drawGrid(((30 * this->currentGame->getXRow(car)) - 1), (30 * this->currentGame->getPosition(car)), 30, (this->currentGame->getSize(car) * 30), C2D_Color32(255, 255, 255, 255));
		}
	}
}

void GameScreen::DrawGameField() const {
	GFX::DrawBottom(false);

	if (this->currentGame && this->currentGame->isValid()) {
		for (int i = 0; i < this->currentGame->getCarAmount(); i++) {
			DrawCar(i);
		}
	}

	for (int i = 0; i < 36; i++) {
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++, i++) {
				Gui::drawGrid(30 + (30 * x), 30 + (30 * y), 30, (30), C2D_Color32(0, 0, 0, 255));
			}
		}
	}

	if (this->currentGame && this->currentGame->isValid()) {
		DrawSelectedCar(this->selectedCar);

		Gui::Draw_Rect(this->carList[1].x-10, this->carList[1].y, 80, this->currentGame->getCarAmount() * 20, config->barColor());

		for (int i = 0; i < this->currentGame->getCarAmount(); i++) {
			Gui::Draw_Rect(this->carList[i+1].x, this->carList[i+1].y, (this->currentGame->getSize(i) * 20), 20, GFX::getColor(this->currentGame->getCar(i)));
		}
	}
}

void GameScreen::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), "RushHour3D - GameScreen", 390);
	Gui::DrawStringCentered(0, 215, 0.8f, config->textColor(), "Current Level: " + this->levelNames[this->wins], 390);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	this->DrawGameField();
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void GameScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Checking...
	if (this->currentGame && this->currentGame->isValid()) {
		if (this->currentGame->getCar(this->selectedCar) == Car::Red) {
			if (this->currentGame->getPosition(this->selectedCar) == 5) {
				Msg::DisplayWaitMsg("You won Level " + std::to_string(this->wins +1) + "! :)");

				this->wins++;
				if (this->wins < (int)this->levelNames.size()) {
					this->currentGame->loadLevel("romfs:/levels/" + this->levelNames[this->wins]);
					this->selectedCar = 0;
					return;
				}

				Gui::screenBack(true);
				return;
			}
		}

		if (hDown & KEY_TOUCH) {
			for (int i = 0; i < this->currentGame->getCarAmount(); i++) {
				if (touching(touch, this->carList[i + 1])) {
					this->selectedCar = i;
				}
			}
		}

		// Switch cars with L/R.
		if (hDown & KEY_R) {
			if (this->selectedCar < this->currentGame->getCarAmount()-1) this->selectedCar++;
		}
		if (hDown & KEY_L) {
			if (this->selectedCar > 0) this->selectedCar--;
		}

		if (hDown & KEY_DOWN) {
			if (this->currentGame->getDirection(this->selectedCar) == Direction::Horizontal) {
				if (this->currentGame->returnIfMovable(this->selectedCar, true)) {
					this->currentGame->setPosition(this->selectedCar, this->currentGame->getPosition(this->selectedCar) + 1);
				}
			}
		}

		if (hDown & KEY_UP) {
			if (this->currentGame->getDirection(this->selectedCar) == Direction::Horizontal) {
				if (this->currentGame->returnIfMovable(this->selectedCar, false)) {
					this->currentGame->setPosition(this->selectedCar, this->currentGame->getPosition(this->selectedCar) - 1);
				}
			}
		}

		if (hDown & KEY_LEFT) {
			if (this->currentGame->getDirection(this->selectedCar) == Direction::Vertical) {
				if (this->currentGame->returnIfMovable(this->selectedCar, false)) {
					this->currentGame->setPosition(this->selectedCar, this->currentGame->getPosition(this->selectedCar) - 1);
				}
			}
		}

		if (hDown & KEY_RIGHT) {
			if (this->currentGame->getDirection(this->selectedCar) == Direction::Vertical) {
				if (this->currentGame->returnIfMovable(this->selectedCar, true)) {
					this->currentGame->setPosition(this->selectedCar, this->currentGame->getPosition(this->selectedCar) + 1);
				}
			}
		}
	}

	// Go screen back.
	if (hDown & KEY_START) {
		Gui::screenBack(true);
		return;
	}
}