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
#include "fileBrowse.hpp"
#include "level.hpp"
#include "overlay.hpp"
#include <dirent.h>
#include <unistd.h>

extern std::unique_ptr<Config> config;

// Draw the Top (File) browse.
static void DrawTop(uint Selection, std::vector<DirEntry> dirContents, bool romfs) {
	std::string levels;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	Gui::ScreenDraw(Top);
	GFX::DrawThemeSprite(theme_fileBrowse_idx, 0, 0);
	Gui::DrawStringCentered(0, currentTheme->browseTitle1, currentTheme->TitleTextSize, currentTheme->TitleTextColor, Lang::get("LEVEL_SELECT") + " " + std::string(romfs ? "[RomFS]" : "[SD Card]"), 390);

	for (uint i = (Selection < 5) ? 0 : Selection - 5; i < dirContents.size() && i < ((Selection < 5) ? 6 : Selection + 1); i++) {
		levels += dirContents[i].name + "\n\n";
	}

	for (uint i = 0; i < ((dirContents.size() < 6) ? 6 - dirContents.size() : 0); i++) {
		levels += "\n\n";
	}

	Gui::DrawString(currentTheme->browseListBeginX, currentTheme->browseListBeginY, currentTheme->browseListSize, currentTheme->browseTextColor, levels, 360);
	Gui::DrawStringCentered(0, currentTheme->browseTitle2, currentTheme->TitleTextSize, currentTheme->TitleTextColor, Lang::get("REFRESH"), 390);

	if (Selection < 6) GFX::DrawThemeSprite(theme_fbSelector_idx, currentTheme->browseSelectorX, currentTheme->browseSelectorY + (Selection * currentTheme->browseMultiply));
	else GFX::DrawThemeSprite(theme_fbSelector_idx, currentTheme->browseSelectorX, currentTheme->browseSelectorY + (5 * currentTheme->browseMultiply));
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

// Draw blocks from a level unique_ptr.
static void DrawBlock(std::unique_ptr<Level> &level, int block) {
	if (level && level->isValid()) {
		if (level->getDirection(block) != Direction::None || level->getBlock(block) != Blocks::Block_Invalid) {
			GFX::DrawBox(level->getDirection(block), level->getBlock(block), currentTheme->browseGridX + (currentTheme->GridBlockSize * (level->getXRow(block) - 1)), currentTheme->browseGridY + (currentTheme->GridBlockSize * (level->getYRow(block) -1)));
		}
	}
}

// The Preview part.
static void DrawBottom(std::unique_ptr<Level> &currentLevel) {
	Gui::ScreenDraw(Bottom);
	GFX::DrawThemeSprite(theme_filebrowse_bottom_idx, 0, 0);
	Gui::DrawStringCentered(0, currentTheme->browseTitle3, currentTheme->TitleTextSize, currentTheme->TitleTextColor, Lang::get("CHANGE_LOCATION_MODE"), 310);

	// Display Level here!
	if (currentLevel && currentLevel->isValid()) {
		GFX::DrawThemeSprite(theme_field_idx, currentTheme->browseGridX, currentTheme->browseGridY);

		for (int i = 0; i < currentLevel->getBlockAmount(); i++) {
			DrawBlock(currentLevel, i);
		}

	}

	Gui::DrawStringCentered(0, currentTheme->browseTitlePrev, currentTheme->TitleTextSize, currentTheme->TitleTextColor, Lang::get("Y_PREVIEW"), 310);
	C3D_FrameEnd(0);
}

std::string Overlays::SelectLevel() {
	s32 selectedLevel = 0;
	bool romfsMode = true;
	std::vector<DirEntry> dirContents;
	bool dirChanged = true;
	std::unique_ptr<Level> currentLevel = std::make_unique<Level>();

	// Initial dir change.
	dirContents.clear();

	chdir("romfs:/Levels/");
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, {"lvl"});
	for(uint i = 0; i < dirContentsTemp.size(); i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}

	while (1) {
		// Screen draw part.
		DrawTop(selectedLevel, dirContents, romfsMode);
		DrawBottom(currentLevel);

		// The input part.
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();

		// if directory changed -> Refresh it.
		if (dirChanged) {
			dirContents.clear();
			std::vector<DirEntry> dirContentsTemp;
			getDirectoryContents(dirContentsTemp, {"lvl"});
			for(uint i = 0; i < dirContentsTemp.size(); i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}

			dirChanged = false;
		}

		if (hRepeat & KEY_UP) {
			if (selectedLevel > 0) {
				selectedLevel--;
			}
		}
		
		if (hDown & KEY_Y) {
			if (dirContents.size() > 0) {
				if (!dirContents[selectedLevel].isDirectory) {
					char path[PATH_MAX];
					getcwd(path, PATH_MAX);
					currentLevel->loadLevel(path + dirContents[selectedLevel].name);
				}
			}
		}
		

		if (hRepeat & KEY_DOWN) {
			if ((uint)selectedLevel < dirContents.size()-1) {
				selectedLevel++;
			}
		}
		
		if (hDown & KEY_A) {
			if (dirContents.size() > 0) {
				if (dirContents[selectedLevel].isDirectory) {
					chdir(dirContents[selectedLevel].name.c_str());
					selectedLevel = 0;
					dirChanged = true;
				} else {
					char path[PATH_MAX];
					getcwd(path, PATH_MAX);
					return path + dirContents[selectedLevel].name;
				}
			}
		}

		if (hDown & KEY_B) {
			char path[PATH_MAX];
			getcwd(path, PATH_MAX);
			if (strcmp(path, "sdmc:/3ds/BlockEscape3DS/Levels/") == 0 || strcmp(path, "/3ds/BlockEscape3DS/Levels/") == 0 || strcmp(path, "romfs:/Levels/") == 0) {
				if (Msg::promptMsg(Lang::get("LEVEL_SELECT_CANCEL"))) {
					return "!NO_LEVEL";
				}
			} else {
				chdir("..");
				selectedLevel = 0;
				dirChanged = true;
			}
		}

		if (hDown & KEY_X) {
			if (romfsMode) {
				romfsMode = false;
				chdir("sdmc:/3ds/BlockEscape3DS/Levels/");
			} else {
				romfsMode = true;
				chdir("romfs:/Levels/");
			}
			
			selectedLevel = 0;
			dirChanged = true;
		}
		
		if (hDown & KEY_START) {
			dirChanged = true;
		}
	}
}