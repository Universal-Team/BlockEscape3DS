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
#include "structs.hpp"

#include <vector>

extern std::unique_ptr<Config> config;
const static std::vector<ButtonStruct> promptBtn = {
	{10, 100, 140, 40, "YES"}, // Yes.
	{170, 100, 140, 40, "NO"}, // No.
	{100, 100, 140, 40, "OK"} // OK.
};

extern touchPosition touch;
extern bool btnTouch(touchPosition touch, ButtonStruct button);

// Display a Message, which needs to be confirmed with A/B.
bool Msg::promptMsg2(std::string promptMsg) {
	s32 selection = 0;
	while(1) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

		Gui::ScreenDraw(Top);
		GFX::DrawSprite(sprites_top_browse_idx, 0, 0);
		GFX::DrawSprite(sprites_msgBox_idx, 0, 46);
		Gui::DrawStringCentered(19 - 200 + (365 / 2), (16 + (104 - (Gui::GetStringHeight(0.6, promptMsg))/2)), 0.6, C2D_Color32(255, 255, 255, 255), promptMsg, 365 - 10, 104 - 10);
		GFX::DrawBottom();

		// Draw Bottom Screen part.
		GFX::Button(promptBtn[0]);
		GFX::Button(promptBtn[1]);

		GFX::DrawButtonSelector(promptBtn[selection].X, promptBtn[selection].Y);

		C3D_FrameEnd(0);

		// Selection part.
		gspWaitForVBlank();
		hidScanInput();
		hidTouchRead(&touch);

		if (hidKeysDown() & KEY_LEFT) {
			selection = 0;
		} else if (hidKeysDown() & KEY_RIGHT) {
			selection = 1;
		}

		if (hidKeysDown() & KEY_A) {
			if (selection == 0) {
				return true;
			} else {
				return false;
			}
		}
		
		if (hidKeysDown() & KEY_TOUCH && btnTouch(touch, promptBtn[0])) {
			return true;
		}

		if (hidKeysDown() & KEY_TOUCH && btnTouch(touch, promptBtn[1])) {
			return false;
		}
	}
}

bool Msg::promptMsg(std::string msg) {
	return Msg::promptMsg2(msg);
}

// Displays a Warn Message.
void Msg::DisplayWarnMsg(std::string Text) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	Gui::ScreenDraw(Top);
	GFX::DrawSprite(sprites_top_browse_idx, 0, 0);
	GFX::DrawSprite(sprites_msgBox_idx, 0, 46);
	Gui::DrawStringCentered(19 - 200 + (365 / 2), (16 + (104 - (Gui::GetStringHeight(0.6, Text))/2)), 0.6, C2D_Color32(255, 255, 255, 255), Text, 365 - 10, 104 - 10);
	GFX::DrawBottom();
	C3D_FrameEnd(0);
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

// Displays a Warn Message. This is mostly be used for things with more text.
void Msg::DisplayWarnMsg2(std::string Text) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	Gui::ScreenDraw(Top);
	GFX::DrawSprite(sprites_top_browse_idx, 0, 0);
	GFX::DrawSprite(sprites_msgBox_idx, 0, 46);
	Gui::DrawStringCentered(19 - 200 + (365 / 2), (16 + (104 - (Gui::GetStringHeight(0.6, Text))/2)), 0.6, C2D_Color32(255, 255, 255, 255), Text, 365 - 10, 104 - 10);
	GFX::DrawBottom();
	C3D_FrameEnd(0);
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

// Display a Message, which can be skipped with A.
void Msg::DisplayWaitMsg(std::string waitMsg, ...) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	Gui::ScreenDraw(Top);
	GFX::DrawSprite(sprites_top_browse_idx, 0, 0);
	GFX::DrawSprite(sprites_msgBox_idx, 0, 46);
	Gui::DrawStringCentered(19 - 200 + (365 / 2), (16 + (104 - (Gui::GetStringHeight(0.6, waitMsg))/2)), 0.6, C2D_Color32(255, 255, 255, 255), waitMsg, 365 - 10, 104 - 10);
	Gui::DrawStringCentered(0, 221, 0.6, C2D_Color32(255, 255, 255, 255), Lang::get("A_CONTINUE"), 390);
	GFX::DrawBottom();
	GFX::Button(promptBtn[2]);
	C3D_FrameEnd(0);

	while(1) {
		gspWaitForVBlank();
		hidScanInput();
		hidTouchRead(&touch);
		if ((hidKeysDown() & KEY_A) || (hidKeysDown() & KEY_TOUCH && btnTouch(touch, promptBtn[2]))) break;
	}
}

void Msg::DisplayMsg(std::string Message) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	Gui::ScreenDraw(Top);
	GFX::DrawSprite(sprites_top_browse_idx, 0, 0);
	GFX::DrawSprite(sprites_msgBox_idx, 0, 46);
	Gui::DrawStringCentered(19 - 200 + (365 / 2), (16 + (104 - (Gui::GetStringHeight(0.6, Message))/2)), 0.6, C2D_Color32(255, 255, 255, 255), Message, 365 - 10, 104 - 10);
	GFX::DrawBottom();
	C3D_FrameEnd(0);
}

void Msg::NotImplementedYet(void) {
	Msg::DisplayWaitMsg(Lang::get("NOT_IMPLEMENTED_YET"));
}

// For debug messages with "Debug": true in the config.
void Msg::DebugMessage(std::string msg) {
	if (config->debug()) {
		Msg::DisplayWaitMsg(msg);
	}
}