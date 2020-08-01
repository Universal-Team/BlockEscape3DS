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

extern std::unique_ptr<Config> config;

void Credits::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.8f, config->textColor(), "BlockEscape3DS - " + Lang::get("CREDITS"), 390);
	GFX::DrawSprite(sprites_stackz_idx, -2, 70);
	//GFX::DrawSprite(sprites_banner_idx, 137, 65);
	Gui::DrawStringCentered(0, 30, 0.6f, config->textColor(), Lang::get("DEVELOPED_BY"), 390);
	Gui::DrawStringCentered(0, 45, 0.6f, config->textColor(), Lang::get("MAIN_DEV"), 390);
	Gui::DrawString(395-Gui::GetStringWidth(0.6f, (Lang::get("CURRENT_VERSION") + V_STRING)), 219, 0.6f, config->textColor(), (Lang::get("CURRENT_VERSION") + V_STRING), 390);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();

	Gui::DrawStringCentered(0, -2, 0.8f, config->textColor(), Lang::get("GENERAL_CREDITS"), 310);
	Gui::DrawStringCentered(0, 30, 0.7f, config->textColor(), "SuperSaiyajinStackZ", 310);
	Gui::DrawStringCentered(0, 60, 0.6f, config->textColor(), Lang::get("DEVELOPING_CORE"), 310);
	Gui::DrawStringCentered(0, 90, 0.7f, config->textColor(), "Universal-Team", 310);
	Gui::DrawStringCentered(0, 120, 0.6f, config->textColor(), Lang::get("UNIVERSAL_CORE"), 310);
	Gui::DrawStringCentered(0, 150, 0.7f, config->textColor(), "devkitPro", 310);
	Gui::DrawStringCentered(0, 180, 0.6f, config->textColor(), Lang::get("DEVKIT"), 310);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void Credits::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack(true);
		return;
	}
}