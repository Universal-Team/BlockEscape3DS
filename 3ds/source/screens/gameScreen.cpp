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

#include "config.hpp"
#include "gameScreen.hpp"
#include "overlay.hpp"

extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern bool btnTouch(touchPosition touch, ButtonStruct button);

GameScreen::GameScreen() {
	// Call Game Constructor here.
	this->currentGame = std::make_unique<Game>();
	const std::string path = Overlays::SelectLevel();
	if (path != "!NO_LEVEL") {
		this->currentGame->loadLevel(path);
		return;
	}
}

void GameScreen::DrawBlock(int block) const {
	if (this->currentGame->getDirection(block) != Direction::None || this->currentGame->getBlock(block) != Blocks::Lock_Invalid) {
		if (this->currentGame->getDirection(block) == Direction::Vertical) {
			Gui::Draw_Rect((30 * this->currentGame->getXRow(block)), ((30 * this->currentGame->getYRow(block)) -1), this->currentGame->getSize(block) * 30, (30), GFX::getBlockColor(this->currentGame->getBlock(block)));
		} else if (this->currentGame->getDirection(block) == Direction::Horizontal) {
			Gui::Draw_Rect(((30 * this->currentGame->getXRow(block)) - 1), (30 * this->currentGame->getYRow(block)), 30, (this->currentGame->getSize(block) * 30), GFX::getBlockColor(this->currentGame->getBlock(block)));
		}
	}
}

void GameScreen::DrawSelectedBlock(int block) const {
	if (this->currentGame->getDirection(block) != Direction::None || this->currentGame->getBlock(block) != Blocks::Lock_Invalid) {
		if (this->currentGame->getDirection(block) == Direction::Vertical) {
			Gui::drawGrid((30 * this->currentGame->getXRow(block)), ((30 * this->currentGame->getYRow(block)) -1), this->currentGame->getSize(block) * 30, (30), C2D_Color32(255, 255, 255, 255));
		} else if (this->currentGame->getDirection(block) == Direction::Horizontal) {
			Gui::drawGrid(((30 * this->currentGame->getXRow(block)) - 1), (30 * this->currentGame->getYRow(block)), 30, (this->currentGame->getSize(block) * 30), C2D_Color32(255, 255, 255, 255));
		}
	}
}

void GameScreen::DrawGameField() const {
	GFX::DrawBottom(false);

	if (this->currentGame && this->currentGame->isValid()) {
		for (int i = 0; i < this->currentGame->getBlockAmount(); i++) {
			DrawBlock(i);
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
		DrawSelectedBlock(this->selectedBlock);
	}
}

void GameScreen::Draw(void) const {
	if (this->mode == 0) {
		GFX::DrawTop();
		Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), "BlockEscape3DS - " + Lang::get("GAME_SCREEN"), 390);
	
		if (this->currentGame->isValid()) {
			Gui::DrawStringCentered(0, 214, 0.8f, config->textColor(), Lang::get("MOVEMENTS") + std::to_string(this->currentGame->getMovement()), 390);
		} else {
			Gui::DrawStringCentered(0, 214, 0.8f, config->textColor(), Lang::get("LEVEL_NOT_LOADED"), 390);
		}

		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

		if (this->currentGame->isValid()) {
			this->DrawGameField();
		} else {
			GFX::DrawBottom(true);
		}

		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	} else {
		GFX::DrawTop();
		Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), "BlockEscape3DS - " + Lang::get("GAME_SCREEN_SUB_MENU"), 390);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
		GFX::DrawBottom(true);

		for (int i = 0; i < 3; i++) {
			GFX::Button(this->subPos[i]);
		}

		GFX::DrawButtonSelector(subPos[this->subSel].X, subPos[this->subSel].Y);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	}
}


void GameScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->mode == 0) {
		// Checking...
		if (this->currentGame && this->currentGame->isValid()) {
			if (this->currentGame->getBlock(this->selectedBlock) == Blocks::Escape) {
				if (this->currentGame->getDirection(this->selectedBlock) == Direction::Vertical) {
					if (this->currentGame->getXRow(this->selectedBlock) == GRIDSIZE - 1) {
						Msg::DisplayWaitMsg(Lang::get("LEVEL_WON"));

						if (Msg::promptMsg(Lang::get("ANOTHER_LEVEL"))) {
							const std::string path = Overlays::SelectLevel();
							if (path != "!NO_LEVEL") {
								this->currentGame->loadLevel(path);
								return;
							} else {
								Msg::DisplayWaitMsg(Lang::get("LEVEL_NOT_SELECTED"));
								Gui::screenBack(true);
								return;
							}
						} else {
							Gui::screenBack(true);
							return;
						}
					}
				} else if (this->currentGame->getDirection(this->selectedBlock) == Direction::Horizontal) {
					if (this->currentGame->getYRow(this->selectedBlock) == GRIDSIZE - 1) {
						Msg::DisplayWaitMsg(Lang::get("LEVEL_WON"));

						if (Msg::promptMsg(Lang::get("ANOTHER_LEVEL"))) {
							const std::string path = Overlays::SelectLevel();
							if (path != "!NO_LEVEL") {
								this->currentGame->loadLevel(path);
								return;
							} else {
								Msg::DisplayWaitMsg(Lang::get("LEVEL_NOT_SELECTED"));
								Gui::screenBack(true);
								return;
							}
						} else {
							Gui::screenBack(true);
							return;
						}
					}
				}
			}

			// Switch blocks with L/R.
			if (hDown & KEY_R) {
				if (this->selectedBlock < this->currentGame->getBlockAmount()-1) this->selectedBlock++;
			}

			if (hDown & KEY_L) {
				if (this->selectedBlock > 0) this->selectedBlock--;
			}

			if (hDown & KEY_DOWN) {
				if (this->currentGame->getDirection(this->selectedBlock) == Direction::Horizontal) {
					if (this->currentGame->returnIfMovable(this->selectedBlock, true)) {
						this->currentGame->setYRow(this->selectedBlock, this->currentGame->getXRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock) + 1);
						this->currentGame->doMovement();
					}
				}
			}

			if (hDown & KEY_UP) {
				if (this->currentGame->getDirection(this->selectedBlock) == Direction::Horizontal) {
					if (this->currentGame->returnIfMovable(this->selectedBlock, false)) {
						this->currentGame->setYRow(this->selectedBlock, this->currentGame->getXRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock) - 1);
						this->currentGame->doMovement();
					}
				}
			}

			if (hDown & KEY_LEFT) {
				if (this->currentGame->getDirection(this->selectedBlock) == Direction::Vertical) {
					if (this->currentGame->returnIfMovable(this->selectedBlock, false)) {
						this->currentGame->setXRow(this->selectedBlock, this->currentGame->getXRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock), this->currentGame->getXRow(this->selectedBlock) - 1);
						this->currentGame->doMovement();
					}
				}
			}

			if (hDown & KEY_RIGHT) {
				if (this->currentGame->getDirection(this->selectedBlock) == Direction::Vertical) {
					if (this->currentGame->returnIfMovable(this->selectedBlock, true)) {
						this->currentGame->setXRow(this->selectedBlock, this->currentGame->getXRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock), this->currentGame->getXRow(this->selectedBlock) + 1);
						this->currentGame->doMovement();
					}
				}
			}

			if (hHeld & KEY_TOUCH) {
				for (int i = 0; i < 36; i++) {
					if (touching(touch, gridPos[i])) {
						if (this->currentGame->returnIndex(i) != -1) {
							this->selectedBlock = this->currentGame->returnIndex(i);
						}
					}
				}

				// Get available fields.
				if (this->currentGame->getDirection(this->selectedBlock) == Direction::Horizontal) {

					for (int i = 0; i < 6; i++) {
						if (this->currentGame->getYRow(this->selectedBlock) + this->currentGame->getSize(this->selectedBlock) -1 < GRIDSIZE && touching(touch, gridPos[this->currentGame->getXRow(this->selectedBlock) + (((this->currentGame->getYRow(this->selectedBlock) + this->currentGame->getSize(this->selectedBlock)) * GRIDSIZE) - GRIDSIZE) - 1])) {
							if (this->currentGame->returnIfMovable(this->selectedBlock, true)) {
								this->currentGame->setYRow(this->selectedBlock, this->currentGame->getXRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock) + 1);
								this->currentGame->doMovement();
							}

						} else if (this->currentGame->getYRow(this->selectedBlock) > STARTPOS && touching(touch, gridPos[this->currentGame->getXRow(this->selectedBlock) + (((this->currentGame->getYRow(this->selectedBlock) - 1) * GRIDSIZE) - GRIDSIZE) - 1])) {
							if (this->currentGame->returnIfMovable(this->selectedBlock, false)) {
								this->currentGame->setYRow(this->selectedBlock, this->currentGame->getXRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock) - 1);
								this->currentGame->doMovement();
							}
						}
					}

				} else if (this->currentGame->getDirection(this->selectedBlock) == Direction::Vertical) {

					for (int i = 0; i < 6; i++) {
						if (this->currentGame->getXRow(this->selectedBlock) + this->currentGame->getSize(this->selectedBlock) -1 < GRIDSIZE && touching(touch, gridPos[((this->currentGame->getYRow(this->selectedBlock) * GRIDSIZE) - GRIDSIZE) + (this->currentGame->getXRow(this->selectedBlock) + this->currentGame->getSize(this->selectedBlock)) - 1])) {
							if (this->currentGame->returnIfMovable(this->selectedBlock, true)) {
								this->currentGame->setXRow(this->selectedBlock, this->currentGame->getXRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock), this->currentGame->getXRow(this->selectedBlock) + 1);
								this->currentGame->doMovement();
							}

						} else if (this->currentGame->getXRow(this->selectedBlock) > STARTPOS && touching(touch, gridPos[((this->currentGame->getYRow(this->selectedBlock) * GRIDSIZE) - GRIDSIZE) + (this->currentGame->getXRow(this->selectedBlock) - 1) - 1])) {
							if (this->currentGame->returnIfMovable(this->selectedBlock, false)) {
								this->currentGame->setXRow(this->selectedBlock, this->currentGame->getXRow(this->selectedBlock), this->currentGame->getYRow(this->selectedBlock), this->currentGame->getXRow(this->selectedBlock) - 1);
								this->currentGame->doMovement();
							}
						}
					}
				}
			}
		}

		if (hDown & KEY_SELECT) {
			this->mode = 1; // Go submenu!
		}

	} else {
		if (hDown & KEY_DOWN) {
			if (this->subSel < 2) this->subSel++;
		}

		if (hDown & KEY_UP) {
			if (this->subSel > 0) this->subSel--;
		}

		if (hDown & KEY_A) {
			switch(this->subSel) {
				case 0:
					if (Msg::promptMsg(Lang::get("ANOTHER_LEVEL"))) {
						const std::string path = Overlays::SelectLevel();
						if (path != "!NO_LEVEL") {
							this->currentGame->loadLevel(path);
							this->mode = 0;
						}
					}
					break;
				case 1:
					if (Msg::promptMsg(Lang::get("RESTART_LEVEL"))) {
						this->currentGame->reload();
						this->selectedBlock = 0;
						this->mode = 0;
					}
					break;
				case 2:
					if (Msg::promptMsg(Lang::get("EXIT_GAME_TXT"))) {
						Gui::screenBack(true);
						return;
					}
					break;
			}
		}

		if (hDown & KEY_TOUCH) {
			if (btnTouch(touch, subPos[0])) {
				if (Msg::promptMsg(Lang::get("ANOTHER_LEVEL"))) {
					const std::string path = Overlays::SelectLevel();
					if (path != "!NO_LEVEL") {
						this->currentGame->loadLevel(path);
						this->mode = 0;
					}
				}
			} else if (btnTouch(touch, subPos[1])) {
				if (Msg::promptMsg(Lang::get("RESTART_LEVEL"))) {
					this->currentGame->reload();
					this->selectedBlock = 0;
					this->mode = 0;
				}
			} else if (btnTouch(touch, subPos[2])) {
				if (Msg::promptMsg(Lang::get("EXIT_GAME_TXT"))) {
					Gui::screenBack(true);
					return;
				}
			}
		}

		if (hDown & KEY_B) {
			this->mode = 0;
		}
	}

	// Go screen back.
	if (hDown & KEY_START) {
		if (Msg::promptMsg(Lang::get("EXIT_GAME_TXT"))) {
			Gui::screenBack(true);
			return;
		}
	}
}