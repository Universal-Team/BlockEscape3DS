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

#include "common.hpp"
#include "config.hpp"

extern std::unique_ptr<Config> config;
extern C2D_SpriteSheet sprites;

void GFX::DrawTop(bool useBars) {
	Gui::ScreenDraw(Top);
	if (useBars) {
		Gui::Draw_Rect(0, 0, 400, 30, config->barColor());
		Gui::Draw_Rect(0, 30, 400, 180, config->bgColor());
		Gui::Draw_Rect(0, 210, 400, 30, config->barColor());
		GFX::DrawSprite(sprites_top_screen_top_idx, 0, 0);
		GFX::DrawSprite(sprites_top_screen_bot_idx, 0, 215);
	} else {
		Gui::Draw_Rect(0, 0, 400, 240, config->bgColor());
	}
}

void GFX::DrawBottom(bool useBars) {
	Gui::ScreenDraw(Bottom);
	if (useBars) {
		Gui::Draw_Rect(0, 0, 320, 30, config->barColor());
		Gui::Draw_Rect(0, 30, 320, 180, config->bgColor());
		Gui::Draw_Rect(0, 210, 320, 30, config->barColor());
		GFX::DrawSprite(sprites_bottom_screen_top_idx, 0, 0);
		GFX::DrawSprite(sprites_bottom_screen_bot_idx, 0, 215);
	} else {
		Gui::Draw_Rect(0, 0, 320, 240, config->bgColor());
	}
}

void GFX::DrawSprite(int index, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(sprites, index, x, y, ScaleX, ScaleY);
}

void GFX::DrawFileBrowseBG(bool isTop) {
	isTop ? Gui::ScreenDraw(Top) : Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, isTop ? 400 : 320, 27, config->barColor());
	Gui::Draw_Rect(0, 27, isTop ? 400 : 320, 31, config->bgColor());
	Gui::Draw_Rect(0, 58, isTop ? 400 : 320, 31, config->bgColor() & C2D_Color32(255, 255, 255, 200));
	Gui::Draw_Rect(0, 89, isTop ? 400 : 320, 31, config->bgColor());
	Gui::Draw_Rect(0, 120, isTop ? 400 : 320, 31, config->bgColor() & C2D_Color32(255, 255, 255, 200));
	Gui::Draw_Rect(0, 151, isTop ? 400 : 320, 31, config->bgColor());
	Gui::Draw_Rect(0, 182, isTop ? 400 : 320, 31, config->bgColor() & C2D_Color32(255, 255, 255, 200));
	Gui::Draw_Rect(0, 213, isTop ? 400 : 320, 27, config->barColor());

	// Bars here.
	isTop ? GFX::DrawSprite(sprites_top_screen_top_idx, 0, 0) : GFX::DrawSprite(sprites_bottom_screen_top_idx, 0, 0);
	isTop ? GFX::DrawSprite(sprites_top_screen_bot_idx, 0, 215) : GFX::DrawSprite(sprites_bottom_screen_bot_idx, 0, 215);
}

void GFX::DrawButtonSelector(int x, int y, float ScaleX, float ScaleY, bool useSmall) {
	static float timer			= 0.0f;
	float highlight_multiplier	= fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
	u8 r						= config->selectorColor() & 0xFF;
	u8 g						= (config->selectorColor() >> 8) & 0xFF;
	u8 b						= (config->selectorColor() >> 16) & 0xFF;
	u32 color = C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);

	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, useSmall ? sprites_btnSelector2_idx : sprites_btnSelector_idx), x, y, 0.5f, &tint, ScaleX, ScaleY);

	timer += .030;
}

void GFX::Button(const ButtonStruct btn) {
	Gui::Draw_Rect(btn.X, btn.Y, btn.xSize, btn.ySize, config->buttonColor());
	Gui::DrawStringCentered(btn.X - 160 + (btn.xSize/2), btn.Y + (btn.ySize/2) - 10, 0.6f, config->textColor(), btn.Text, btn.X-10, btn.Y-5);
}

// Colors for cars.
u32 GFX::getCarColor(Car cr) {
	switch(cr) {
		case Car::Lock_Invalid:
			return 0;
		case Car::Lock1:
			return config->carColor(0);
		case Car::Lock2:
			return config->carColor(1);
		case Car::Lock3:
			return config->carColor(2);
		case Car::Lock4:
			return config->carColor(3);
		case Car::Lock5:
			return config->carColor(4);
		case Car::Lock6:
			return config->carColor(5);
		case Car::Lock7:
			return config->carColor(6);
		case Car::Lock8:
			return config->carColor(7);
		case Car::Lock9:
			return config->carColor(8);
		case Car::Lock10:
			return config->carColor(9);
		case Car::Lock11:
			return config->carColor(10);
		case Car::Lock12:
			return config->carColor(11);
		case Car::Lock13:
			return config->carColor(12);
		case Car::Lock14:
			return config->carColor(13);
		case Car::Lock15:
			return config->carColor(14);
		case Car::Red:
			return config->carColor(15);
	}

	return 0;
}