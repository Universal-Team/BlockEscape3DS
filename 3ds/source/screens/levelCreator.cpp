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

#include "config.hpp"
#include "keyboard.hpp"
#include "levelCreator.hpp"
#include "overlay.hpp"

extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

LevelCreator::LevelCreator() {
	this->level = std::make_unique<Level>(false);
	this->level->createNew();
}

/* Return, what to select or edit. */
const std::string LevelCreator::getSelectMsg() const {
	switch(this->selectedMode) {
		case 0:
			return Lang::get("SELECT_CAR");
		case 1:
			return Lang::get("SELECT_DIRECTION");
		case 2:
			return Lang::get("SELECT_ROW_Y");
		case 3:
			return Lang::get("SELECT_ROW_X");
	}

	return "";
}

void LevelCreator::DrawCar(int car) const {
	if (this->level->getDirection(car) != Direction::None || this->level->getCar(car) != Car::Lock_Invalid) {
		if (this->level->getXRow(car) != 0 || this->level->getYRow(car) != 0) {
			if (this->level->getDirection(car) == Direction::Vertical) {
				Gui::Draw_Rect((30 * this->level->getXRow(car)), ((30 * this->level->getYRow(car)) -1), this->level->getSize(car) * 30, (30), GFX::getCarColor(this->level->getCar(car)));
			} else if (this->level->getDirection(car) == Direction::Horizontal) {
				Gui::Draw_Rect(((30 * this->level->getXRow(car)) - 1), (30 * this->level->getYRow(car)), 30, (this->level->getSize(car) * 30), GFX::getCarColor(this->level->getCar(car)));
			}
		}
	}
}	

void LevelCreator::DrawGameField() const {
	GFX::DrawBottom(false);

	if (this->level && this->level->isValid()) {
		for (int i = 0; i < this->level->getCarAmount(); i++) {
			this->DrawCar(i);
		}
	}

	for (int i = 0; i < 36; i++) {
		Gui::drawGrid(gridPos[i].x, gridPos[i].y, gridPos[i].w, gridPos[i].h, C2D_Color32(0, 0, 0, 255));
	}

	if (this->level && this->level->isValid()) {
		Gui::Draw_Rect(230, 0, 80, this->level->getCarAmount() * 15, config->barColor());

		for (int i = 0; i < this->level->getCarAmount(); i++) {
			Gui::Draw_Rect(this->carList[i].x, this->carList[i].y, (this->level->getSize(i) * 20), 10, GFX::getCarColor(this->level->getCar(i)));
		}
	}

	GFX::DrawSprite(sprites_save_idx, saveIcon.x, saveIcon.y);
}

void LevelCreator::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), "RushHour3D - " + Lang::get("LEVEL_CREATOR"), 390);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.7f, "You need to take care by yourself about the cars.\nThere aren't any checks added yet.\nChecks might be added with v0.2.0."))/2, 0.7f, config->textColor(), "You need to take care by yourself about the cars.\nThere aren't any checks added yet.\nChecks might be added with v0.2.0.", 390, 70);
	Gui::DrawStringCentered(0, 214, 0.8f, config->textColor(), this->getSelectMsg(), 390);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	if (this->level->isValid()) {
		this->DrawGameField();
	} else {
		GFX::DrawBottom(true);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void LevelCreator::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->level && this->level->isValid()) {

		if (this->selectedMode == 0) {
			// Car Selection.
			if (hDown & KEY_TOUCH) {
				for (int i = 0; i < 16; i++) {
					if (touching(touch, carList[i])) {
						this->selectedCar = i;
						this->selectedMode = 1;
					}
				}
			}

		} else if (this->selectedMode == 1) {
			// Select the Direction here.
			if (Msg::promptMsg(Lang::get("DIRECTION_SELECT"))) {
				this->level->setDirection(this->selectedCar, this->level->getXRow(this->selectedCar), this->level->getYRow(this->selectedCar), Direction(1));
				this->selectedMode = 2;
			} else {
				this->level->setDirection(this->selectedCar, this->level->getXRow(this->selectedCar), this->level->getYRow(this->selectedCar), Direction(2));
				this->selectedMode = 2;
			}

		} else if (this->selectedMode == 2) {
			if (hDown & KEY_TOUCH) {
				for (int i = 0; i < 36; i++) {
					if (touching(touch, gridPos[i])) {
						this->Y = (i / 6) + 1;
						this->selectedMode = 3;
					}
				}
			}
		} else if (this->selectedMode == 3) {
			if (hDown & KEY_TOUCH) {
				for (int i = 0; i < 6; i++) {
					if (touching(touch, gridPos[((this->Y * GRIDSIZE) - GRIDSIZE) + i])) {
						this->X = i + 1;
						this->selectedMode = 4;
					}
				}
			}
		} else if (this->selectedMode == 4) {
			this->level->setXRow(this->selectedCar, 0, 0, this->X);
			this->level->setYRow(this->selectedCar, 0, 0, this->Y);
			this->selectedMode = 0;
		}
	}

	// Go screen back.
	if (hDown & KEY_START) {
		Gui::screenBack(true);
		return;
	}

	// TODO: Overlay for location selection and KBD for name input.
	if (hDown & KEY_TOUCH && touching(touch, saveIcon)) {
		if (this->level->getXRow(15) != 0 || this->level->getYRow(15) != 0) {
			this->level->setCreatorStuff();
			std::string temp = Overlays::SelectDirectory();
			temp += Keyboard::setkbdString(30, Lang::get("ENTER_LEVEL_NAME"));
			temp += ".lvl";
			FILE *tmp = fopen(temp.c_str(), "w");
			fwrite(this->level->levelDt().get(), 0x34, 1, tmp);
			fclose(tmp);
		} else {
			Msg::DisplayWaitMsg(Lang::get("RED_CAR_NEEDED"));
		}
	}
}