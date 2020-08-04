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
#include "colorHelper.hpp"
#include "theme.hpp"

#include <3ds.h>
#include <regex>
#include <string>
#include <unistd.h>

#define DEFAULT_THEME "romfs:/gfx/theme.json"

// Get a uint32_t color from a hex string.
static u32 getHexColor(std::string colorString) {
	if (colorString.length() < 7 || std::regex_search(colorString.substr(1), std::regex("[^0-9a-f]"))) { // invalid color
		return 0;
	}

	int r = std::stoi(colorString.substr(1, 2), nullptr, 16);
	int g = std::stoi(colorString.substr(3, 2), nullptr, 16);
	int b = std::stoi(colorString.substr(5, 2), nullptr, 16);
	return RGBA8(r, g, b, 0xFF);
}

Theme::Theme(const std::string &path) {
	//this->path = ThemePath;
	this->initialize();
}

bool Theme::isValid() { return this->valid; }

void Theme::initialize() {
	if (access(DEFAULT_THEME, F_OK) != 0 ) {
		this->valid = false;
	}

	FILE* file = fopen(DEFAULT_THEME, "r");
	this->themeJSON = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	// Button.
	if (this->themeJSON["Button"].contains("Text_Size")) this->ButtonTextSize = this->getFloat("Button", "Text_Size");
	if (this->themeJSON["Button"].contains("Button_XSize")) this->ButtonXSize = this->getInt("Button", "Button_XSize");
	if (this->themeJSON["Button"].contains("Button_YSize")) this->ButtonYSize = this->getInt("Button", "Button_YSize");
	if (this->themeJSON["Button"].contains("Text_Color")) this->ButtonTextColor = getHexColor(this->getString("Button", "Text_Color"));

	// Title.
	if (this->themeJSON["Title"].contains("ThemeName")) this->TitleName = this->getString("Title", "ThemeName");
	if (this->themeJSON["Title"].contains("Text_Size")) this->TitleTextSize = this->getFloat("Title", "Text_Size");
	if (this->themeJSON["Title"].contains("Top_YTop")) this->TitleYTop = this->getInt("Title", "Top_YTop");
	if (this->themeJSON["Title"].contains("Top_YBottom")) this->TitleYBottom = this->getInt("Title", "Top_YBottom");
	if (this->themeJSON["Title"].contains("Text_Color")) this->TitleTextColor = getHexColor(this->getString("Title", "Text_Color"));

	// Grid.
	if (this->themeJSON["Grid"].contains("XPos")) this->GridXPos = this->getInt("Grid", "XPos");
	if (this->themeJSON["Grid"].contains("YPos")) this->GridYPos = this->getInt("Grid", "YPos");
	if (this->themeJSON["Grid"].contains("BlockSize")) this->GridBlockSize = this->getInt("Grid", "BlockSize");

	// MSG.
	if (this->themeJSON["Msg"].contains("XPos")) this->MsgXPos = this->getInt("Msg", "XPos");
	if (this->themeJSON["Msg"].contains("YPos")) this->MsgYPos = this->getInt("Msg", "YPos");
	if (this->themeJSON["Msg"].contains("MsgX")) this->MsgTextX = this->getInt("Msg", "MsgX");
	if (this->themeJSON["Msg"].contains("MsgY")) this->MsgTextY = this->getInt("Msg", "MsgY");
	if (this->themeJSON["Msg"].contains("XSize")) this->MsgXSize = this->getInt("Msg", "XSize");
	if (this->themeJSON["Msg"].contains("YSize")) this->MsgYSize = this->getInt("Msg", "YSize");
	if (this->themeJSON["Msg"].contains("Text_Size")) this->MsgTextSize = this->getFloat("Msg", "Text_Size");
	if (this->themeJSON["Msg"].contains("Text_Color")) this->MsgTextColor = getHexColor(this->getString("Msg", "Text_Color"));

	// Credits.
	if (this->themeJSON["Credits"].contains("Stack_Img_X")) this->stackX = this->getInt("Credits", "Stack_Img_X");
	if (this->themeJSON["Credits"].contains("Stack_Img_Y")) this->stackY = this->getInt("Credits", "Stack_Img_Y");
	if (this->themeJSON["Credits"].contains("Dev_By_Y")) this->dev_by = this->getInt("Credits", "Dev_By_Y");
	if (this->themeJSON["Credits"].contains("Main_Dev_Y")) this->main_dev = this->getInt("Credits", "Main_Dev_Y");
	if (this->themeJSON["Credits"].contains("SuperSaiyajinStackZ")) this->SSSZ = this->getInt("Credits", "SuperSaiyajinStackZ");
	if (this->themeJSON["Credits"].contains("Develope_BlockEscape")) this->Dev_BE = this->getInt("Credits", "Develope_BlockEscape");
	if (this->themeJSON["Credits"].contains("Universal-Team")) this->UT = this->getInt("Credits", "Universal-Team");
	if (this->themeJSON["Credits"].contains("Universal-Core")) this->UC = this->getInt("Credits", "Universal-Core");
	if (this->themeJSON["Credits"].contains("devkitPro")) this->DKP = this->getInt("Credits", "devkitPro");
	if (this->themeJSON["Credits"].contains("devkitARM")) this->DKA = this->getInt("Credits", "devkitARM");
	if (this->themeJSON["Credits"].contains("TitleSize")) this->CreditsTitleSize = this->getFloat("Credits", "TitleSize");
	if (this->themeJSON["Credits"].contains("DescSize")) this->CreditsDescSize = this->getFloat("Credits", "DescSize");

	// MainMenu.
	if (this->themeJSON["MainMenu"].contains("Button1X")) this->mmButton1X = this->getInt("MainMenu", "Button1X");
	if (this->themeJSON["MainMenu"].contains("Button1Y")) this->mmButton1Y = this->getInt("MainMenu", "Button1Y");
	if (this->themeJSON["MainMenu"].contains("Button2X")) this->mmButton2X = this->getInt("MainMenu", "Button2X");
	if (this->themeJSON["MainMenu"].contains("Button2Y")) this->mmButton2Y = this->getInt("MainMenu", "Button2Y");
	if (this->themeJSON["MainMenu"].contains("Button3X")) this->mmButton3X = this->getInt("MainMenu", "Button3X");
	if (this->themeJSON["MainMenu"].contains("Button3Y")) this->mmButton3Y = this->getInt("MainMenu", "Button3Y");
	if (this->themeJSON["MainMenu"].contains("Button4X")) this->mmButton4X = this->getInt("MainMenu", "Button4X");
	if (this->themeJSON["MainMenu"].contains("Button4Y")) this->mmButton4Y = this->getInt("MainMenu", "Button4Y");

	// UI Settings.
	if (this->themeJSON["UI-Settings"].contains("Button1X")) this->uiButton1X = this->getInt("UI-Settings", "Button1X");
	if (this->themeJSON["UI-Settings"].contains("Button1Y")) this->uiButton1Y = this->getInt("UI-Settings", "Button1Y");
	if (this->themeJSON["UI-Settings"].contains("Button2X")) this->uiButton2X = this->getInt("UI-Settings", "Button2X");
	if (this->themeJSON["UI-Settings"].contains("Button2Y")) this->uiButton2Y = this->getInt("UI-Settings", "Button2Y");

	// Browse.
	if (this->themeJSON["Browse"].contains("TitleY")) this->browseTitle1 = this->getInt("Browse", "TitleY");
	if (this->themeJSON["Browse"].contains("Title2Y")) this->browseTitle2 = this->getInt("Browse", "Title2Y");
	if (this->themeJSON["Browse"].contains("Title3Y")) this->browseTitle3 = this->getInt("Browse", "Title3Y");
	if (this->themeJSON["Browse"].contains("PreviewTxt")) this->browseTitlePrev = this->getInt("Browse", "PreviewTxt");
	if (this->themeJSON["Browse"].contains("ListBeginX")) this->browseListBeginX = this->getInt("Browse", "ListBeginX");
	if (this->themeJSON["Browse"].contains("ListBeginY")) this->browseListBeginY = this->getInt("Browse", "ListBeginY");
	if (this->themeJSON["Browse"].contains("ListSize")) this->browseListSize = this->getFloat("Browse", "ListSize");
	if (this->themeJSON["Browse"].contains("ListColor")) this->browseTextColor = getHexColor(this->getString("Browse", "ListColor"));
	if (this->themeJSON["Browse"].contains("GridX")) this->browseGridX = this->getInt("Browse", "GridX");
	if (this->themeJSON["Browse"].contains("GridY")) this->browseGridY = this->getInt("Browse", "GridY");

	// Level Creator.
	if (this->themeJSON["Level-Creator"].contains("GridX")) this->creatorGridX = this->getInt("v", "GridX");
	if (this->themeJSON["Level-Creator"].contains("GridY")) this->creatorGridY = this->getInt("Level-Creator", "GridY");
	if (this->themeJSON["Level-Creator"].contains("BlockX")) this->creatorXPos = this->getInt("Level-Creator", "BlockX");
	if (this->themeJSON["Level-Creator"].contains("BlockY")) this->creatorYPos = this->getInt("Level-Creator", "BlockY");
	if (this->themeJSON["Level-Creator"].contains("SaveX")) this->creatorSaveX = this->getInt("Level-Creator", "SaveX");
	if (this->themeJSON["Level-Creator"].contains("SaveY")) this->creatorSaveY = this->getInt("Level-Creator", "SaveY");

	this->valid = true;
}

// Helper functions.
bool Theme::getBool(const std::string &key, const std::string &key2) {
	if (!this->themeJSON[key].contains(key2)) {
		return false;
	}
	
	return this->themeJSON[key][key2].get_ref<const bool&>();
}

int Theme::getInt(const std::string &key, const std::string &key2) {
	if (!this->themeJSON[key].contains(key2)) {
		return 0;
	}

	return this->themeJSON[key][key2].get_ref<const int64_t&>();
}

float Theme::getFloat(const std::string &key, const std::string &key2) {
	if (!this->themeJSON[key].contains(key2)) {
		return 0;
	}

	return this->themeJSON[key][key2].get_ref<const double&>();
}

std::string Theme::getString(const std::string &key, const std::string &key2) {
	if (!this->themeJSON[key].contains(key2)) {
		return "";
	}

	return this->themeJSON[key][key2].get_ref<const std::string&>();
}