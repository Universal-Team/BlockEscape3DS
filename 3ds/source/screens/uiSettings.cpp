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

#include "credits.hpp"
#include "config.hpp"
#include "overlay.hpp"
#include "uiSettings.hpp"

extern std::unique_ptr<Config> config;
extern bool btnTouch(touchPosition touch, ButtonStruct button);

void UISettings::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, currentTheme->TitleYTop, currentTheme->TitleTextSize, currentTheme->TitleTextColor, "BlockEscape3DS - " + Lang::get("UI_SETTINGS"), 390);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();

	for (int i = 0; i < 2; i++) {
		GFX::Button(mainButtons[i]);
	}

	GFX::DrawButtonSelector(mainButtons[this->Selection].X, mainButtons[this->Selection].Y);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void UISettings::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_TOUCH) {
		if (btnTouch(touch, mainButtons[0])) {
			Msg::NotImplementedYet();
			//Overlays::SelectLanguage();
		} else if (btnTouch(touch, mainButtons[1])) {
			Gui::setScreen(std::make_unique<Credits>(), true, true);
		}
	}

	if (hDown & KEY_RIGHT) {
		if (this->Selection < 1) this->Selection++;
	}

	if (hDown & KEY_LEFT) {
		if (this->Selection > 0) this->Selection--;
	}

	if (hDown & KEY_A) {
		if (this->Selection == 0) {
			Msg::NotImplementedYet();
			//Overlays::SelectLanguage();
		} else if (this->Selection == 1) {
			Gui::setScreen(std::make_unique<Credits>(), true, true);
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack(true);
		return;
	}
}