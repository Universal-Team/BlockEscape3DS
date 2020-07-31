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

#ifndef _RUSH_HOUR_3D_GAME_SCREEN_HPP
#define _RUSH_HOUR_3D_GAME_SCREEN_HPP

#include "common.hpp"
#include "game.hpp"
#include "structs.hpp"

class GameScreen : public Screen {
public:
	GameScreen();
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	void DrawCar(int car) const;
	void DrawSelectedCar(int car) const;
	std::unique_ptr<Game> currentGame; // Our current game.
	void DrawGameField() const;
	int selectedCar = 0, mode = 0, subSel = 0;

	const std::vector<ButtonStruct> subPos = {
		{90, 40, 140, 40, Lang::get("LOAD_LEVEL")},
		{90, 100, 140, 40, Lang::get("RESTART")},
		{90, 160, 140, 40, Lang::get("EXIT_GAME")}
	};

	const std::vector<Structs::ButtonPos> gridPos = {
		{30, 30, 30, 30},
		{60, 30, 30, 30},
		{90, 30, 30, 30},
		{120, 30, 30, 30},
		{150, 30, 30, 30},
		{180, 30, 30, 30},

		{30, 60, 30, 30},
		{60, 60, 30, 30},
		{90, 60, 30, 30},
		{120, 60, 30, 30},
		{150, 60, 30, 30},
		{180, 60, 30, 30},

		{30, 90, 30, 30},
		{60, 90, 30, 30},
		{90, 90, 30, 30},
		{120, 90, 30, 30},
		{150, 90, 30, 30},
		{180, 90, 30, 30},

		{30, 120, 30, 30},
		{60, 120, 30, 30},
		{90, 120, 30, 30},
		{120, 120, 30, 30},
		{150, 120, 30, 30},
		{180, 120, 30, 30},

		{30, 150, 30, 30},
		{60, 150, 30, 30},
		{90, 150, 30, 30},
		{120, 150, 30, 30},
		{150, 150, 30, 30},
		{180, 150, 30, 30},

		{30, 180, 30, 30},
		{60, 180, 30, 30},
		{90, 180, 30, 30},
		{120, 180, 30, 30},
		{150, 180, 30, 30},
		{180, 180, 30, 30}
	};
};

#endif