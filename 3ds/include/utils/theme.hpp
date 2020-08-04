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

#ifndef _BLOCK_ESCAPE_3DS_THEME_HPP
#define _BLOCK_ESCAPE_3DS_THEME_HPP

#include "json.hpp"

#include <3ds.h>
#include <string>

class Theme {
public:
	Theme(const std::string &path);
	void initialize();
	bool isValid();

	// Button.
	float ButtonTextSize = 0.6; int ButtonXSize = 140, ButtonYSize = 40; u32 ButtonTextColor = C2D_Color32(255, 255, 255, 255);
	// Title.
	float TitleTextSize = 0.6; int TitleYTop = -1, TitleYBottom = 221; u32 TitleTextColor = C2D_Color32(255, 255, 255, 255); std::string TitleName = "BlockEscape3DS";
	// Grid.
	int GridXPos = 69, GridYPos = 38, GridBlockSize = 30;
	// MSG.
	int MsgXPos = 0, MsgYPos = 46, MsgTextX = 19, MsgTextY = 16, MsgXSize = 365, MsgYSize = 104; float MsgTextSize = 0.6; u32 MsgTextColor = C2D_Color32(255, 255, 255, 255);
	// Credits.
	int stackX = 2, stackY = 83, dev_by = 30, main_dev = 50, SSSZ = 30, Dev_BE = 50, UT = 80, UC = 100, DKP = 130, DKA = 150; float CreditsTitleSize = 0.7, CreditsDescSize = 0.6;
	// MainMenu.
	int mmButton1X = 10, mmButton1Y = 70, mmButton2X = 170, mmButton2Y = 70, mmButton3X = 10, mmButton3Y = 145, mmButton4X = 170, mmButton4Y = 145;
	// UI Settings.
	int uiButton1X = 10, uiButton1Y = 70, uiButton2X = 170, uiButton2Y = 70;
	// Browse.
	int browseTitle1 = -1, browseTitle2 = 221, browseTitle3 = -1, browseTitlePrev = 221, browseListBeginX = 26, browseListBeginY = 32, browseGridX = 69, browseGridY = 29, browseSelectorX = 3, browseSelectorY = 31, browseMultiply = 32; float browseListSize = 0.53; u32 browseTextColor = C2D_Color32(255, 255, 255, 255);
	// Level Creator.
	int creatorGridX = 10, creatorGridY = 38, creatorXPos = 215, creatorYPos = 68, creatorSaveX = 290, creatorSaveY = 210;
private:
	// Mainly helper.
	bool getBool(const std::string &key, const std::string &key2);
	int getInt(const std::string &key, const std::string &key2);
	std::string getString(const std::string &key, const std::string &key2);
	float getFloat(const std::string &key, const std::string &key2);
	nlohmann::json themeJSON; // Our private JSON file.
	bool valid = false;
};

#endif