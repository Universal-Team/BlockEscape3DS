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
	Gui::ScreenDraw(Top);
	GFX::DrawSprite(sprites_top_browse_idx, 0, 0);
	Gui::DrawStringCentered(0, -1, 0.6, C2D_Color32(255, 255, 255, 255), "BlockEscape3DS - " + Lang::get("CREDITS"), 390);
	GFX::DrawSprite(sprites_stackz_idx, 115, 83);
	Gui::DrawStringCentered(0, 30, 0.7, C2D_Color32(255, 255, 255, 255), Lang::get("DEVELOPED_BY"), 390);
	Gui::DrawStringCentered(0, 50, 0.7, C2D_Color32(255, 255, 255, 255), Lang::get("MAIN_DEV"), 390);
	Gui::DrawStringCentered(0, 221, 0.6, C2D_Color32(255, 255, 255, 255), (Lang::get("CURRENT_VERSION") + V_STRING), 390);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	Gui::ScreenDraw(Bottom);
	GFX::DrawSprite(sprites_bottom_browse_idx, 0, 0);

	Gui::DrawStringCentered(0, -1, 0.6, C2D_Color32(255, 255, 255, 255), Lang::get("GENERAL_CREDITS"), 310);
	Gui::DrawStringCentered(0, 30, 0.7, C2D_Color32(255, 255, 255, 255), "SuperSaiyajinStackZ", 310);
	Gui::DrawStringCentered(0, 50, 0.6, C2D_Color32(255, 255, 255, 255), Lang::get("DEVELOPING_CORE"), 310);
	Gui::DrawStringCentered(0, 80, 0.7, C2D_Color32(255, 255, 255, 255), "Universal-Team", 310);
	Gui::DrawStringCentered(0, 100, 0.6, C2D_Color32(255, 255, 255, 255), Lang::get("UNIVERSAL_CORE"), 310);
	Gui::DrawStringCentered(0, 130, 0.7, C2D_Color32(255, 255, 255, 255), "devkitPro", 310);
	Gui::DrawStringCentered(0, 150, 0.6, C2D_Color32(255, 255, 255, 255), Lang::get("DEVKIT"), 310);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void Credits::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack(true);
		return;
	}
}