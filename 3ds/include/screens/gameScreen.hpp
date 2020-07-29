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
	int selectedCar = 0;

	// Car Selection.
	const std::vector<Structs::ButtonPos> carList = {
		{240, 20, 60, 20},
		{240, 40, 60, 20},
		{240, 60, 60, 20},
		{240, 80, 60, 20},
		{240, 100, 60, 20},
		{240, 120, 60, 20},
		{240, 140, 60, 20},
		{240, 160, 60, 20},
		{240, 180, 60, 20},
		{240, 200, 60, 20},
		{240, 220, 60, 20},
		{240, 240, 60, 20},
		{240, 260, 60, 20},
		{240, 280, 60, 20},
		{240, 400, 60, 20},
		{240, 320, 60, 20}
	};
};

#endif