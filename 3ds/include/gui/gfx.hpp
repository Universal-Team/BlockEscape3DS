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

#ifndef _BLOCK_ESCAPE_3DS_GFX_HPP
#define _BLOCK_ESCAPE_3DS_GFX_HPP

#include "game.hpp"
#include "sprites.h"
#include "theme.h"

#include <string>

struct ButtonStruct {
	int X;
	int Y;
	float xSize;
	float ySize;
	std::string Text;
};

namespace GFX {
	// Basic GUI.
	void DrawTop(bool useBars = true);
	void DrawBottom(bool useBars = true);
	void DrawFileBrowseBG(bool isTop = true);

	void DrawSprite(int index, int x, int y, float ScaleX = 1, float ScaleY = 1);
	void DrawThemeSprite(int index, int x, int y, float ScaleX = 1, float ScaleY = 1);
	void DrawButtonSelector(int x, int y, float ScaleX = 1, float ScaleY = 1);

	void DrawBox(Direction dr, Blocks bl, int x, int y);

	// Buttons.
	void Button(const ButtonStruct btn);

	void DrawBoxSelected(Direction dr, Blocks bl, int x, int y);
}

#endif