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
#include "keyboard.hpp"
#include "screenCommon.hpp"

std::string Keyboard::setkbdString(uint maxLength, std::string Text) {
	C3D_FrameEnd(0);
	SwkbdState state;
	swkbdInit(&state, SWKBD_TYPE_NORMAL, 2, maxLength);
	char temp[maxLength] = {0};
	swkbdSetHintText(&state, Text.c_str());
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, SWKBD_FILTER_PROFANITY, 0);
	SwkbdButton ret = swkbdInputText(&state, temp, sizeof(temp));
	temp[maxLength-1] = '\0';

	if (ret == SWKBD_BUTTON_CONFIRM) {
		return temp;
	}

	return "";
}

int Keyboard::setInt(int maxValue, std::string Text) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 255));
	Gui::ScreenDraw(Top);
	GFX::DrawThemeSprite(theme_fileBrowse_idx, 0, 0);
	GFX::DrawThemeSprite(theme_msgBox_idx, currentTheme->MsgXPos, currentTheme->MsgYPos);
	Gui::DrawStringCentered(currentTheme->MsgTextX - 200 + (currentTheme->MsgXSize / 2), (currentTheme->MsgTextY + (currentTheme->MsgYSize - (Gui::GetStringHeight(currentTheme->MsgTextSize, Text))/2)), currentTheme->MsgTextSize, currentTheme->MsgTextColor, Text, currentTheme->MsgXSize - 10, currentTheme->MsgYSize - 10);
	C3D_FrameEnd(0);
	SwkbdState state;
	swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 3);
	swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
	char input[4]   = {0};
	SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
	input[3]        = '\0';

	if (ret == SWKBD_BUTTON_CONFIRM) {
		return (int)std::min(std::stoi(input), maxValue);
	} else if (ret == SWKBD_BUTTON_NONE) {
		return -1;
	}

	return -1;
}

int Keyboard::setu8(std::string Text) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 255));
	Gui::ScreenDraw(Top);
	GFX::DrawThemeSprite(theme_fileBrowse_idx, 0, 0);
	GFX::DrawThemeSprite(theme_msgBox_idx, currentTheme->MsgXPos, currentTheme->MsgYPos);
	Gui::DrawStringCentered(currentTheme->MsgTextX - 200 + (currentTheme->MsgXSize / 2), (currentTheme->MsgTextY + (currentTheme->MsgYSize - (Gui::GetStringHeight(currentTheme->MsgTextSize, Text))/2)), currentTheme->MsgTextSize, currentTheme->MsgTextColor, Text, currentTheme->MsgXSize - 10, currentTheme->MsgYSize - 10);
	C3D_FrameEnd(0);
	SwkbdState state;
	swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 3);
	swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
	char input[4]   = {0};
	SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
	input[3]        = '\0';

	if (ret == SWKBD_BUTTON_CONFIRM) {
		return (u8)std::min(std::stoi(input), 255);
	} else if (ret == SWKBD_BUTTON_NONE) {
		return -1;
	}

	return -1;
}