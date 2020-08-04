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

#include "common.hpp"
#include "config.hpp"

extern std::unique_ptr<Config> config;
extern C2D_SpriteSheet sprites, theme;

void GFX::DrawTop(bool useBars) {
	Gui::ScreenDraw(Top);
	Gui::DrawSprite(theme, theme_top_bg_idx, 0, 0);
}

void GFX::DrawBottom(bool useBars) {
	Gui::ScreenDraw(Bottom);
	Gui::DrawSprite(theme, theme_bg_bottom_idx, 0, 0);
}

void GFX::DrawSprite(int index, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(sprites, index, x, y, ScaleX, ScaleY);
}

void GFX::DrawThemeSprite(int index, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(theme, index, x, y, ScaleX, ScaleY);
}

// TODO.
void GFX::DrawFileBrowseBG(bool isTop) {
	Gui::ScreenDraw(Top);
	GFX::DrawThemeSprite(theme_filebrowse_idx, 0, 0);
}

void GFX::DrawButtonSelector(int x, int y, float ScaleX, float ScaleY) {
	GFX::DrawThemeSprite(theme_selector_idx, x, y);
}

void GFX::Button(const ButtonStruct btn) {
	GFX::DrawThemeSprite(theme_button_idx, btn.X, btn.Y);
	Gui::DrawStringCentered(btn.X - 160 + (btn.xSize/2), btn.Y + (btn.ySize/2) - 10, 0.6f, currentTheme->ButtonTextColor, btn.Text, btn.X-10, btn.Y-5);
}

void GFX::DrawBox(Direction dr, Blocks bl, int x, int y) {
	if (dr == Direction::Horizontal) {
		switch(bl) {
			case Blocks::Block_Invalid:
				break;
			case Blocks::Block2:
				GFX::DrawThemeSprite(theme_block_2_hz_idx, x, y);
				break;
			case Blocks::Block3:
				GFX::DrawThemeSprite(theme_block_3_hz_idx, x, y);
				break;
			case Blocks::Block_Escape:
				GFX::DrawThemeSprite(theme_keyblock_hz_idx, x, y);
				break;

		}
	} else if (dr == Direction::Vertical) {
		switch(bl) {
			case Blocks::Block_Invalid:
				break;
			case Blocks::Block2:
				GFX::DrawThemeSprite(theme_block_2_vc_idx, x, y);
				break;
			case Blocks::Block3:
				GFX::DrawThemeSprite(theme_block_3_vc_idx, x, y);
				break;
			case Blocks::Block_Escape:
				GFX::DrawThemeSprite(theme_keyblock_vc_idx, x, y);
				break;
		}
	}
}

void GFX::DrawBoxSelected(Direction dr, Blocks bl, int x, int y) {
	if (dr == Direction::Horizontal) {
		switch(bl) {
			case Blocks::Block_Invalid:
				break;
			case Blocks::Block2:
				GFX::DrawThemeSprite(theme_block_2_hz_selected_idx, x, y);
				break;
			case Blocks::Block3:
				GFX::DrawThemeSprite(theme_block_3_hz_selected_idx, x, y);
				break;
			case Blocks::Block_Escape:
				GFX::DrawThemeSprite(theme_keyblock_hz_selected_idx, x, y);
				break;

		}
	} else if (dr == Direction::Vertical) {
		switch(bl) {
			case Blocks::Block_Invalid:
				break;
			case Blocks::Block2:
				GFX::DrawThemeSprite(theme_block_2_vc_selected_idx, x, y);
				break;
			case Blocks::Block3:
				GFX::DrawThemeSprite(theme_block_3_vc_selected_idx, x, y);
				break;
			case Blocks::Block_Escape:
				GFX::DrawThemeSprite(theme_keyblock_vc_selected_idx, x, y);
				break;
		}
	}
}