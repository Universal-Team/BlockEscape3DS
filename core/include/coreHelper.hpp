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

#ifndef _BLOCK_ESCAPE_3DS_CORE_HELPER_HPP
#define _BLOCK_ESCAPE_3DS_CORE_HELPER_HPP

#define GRIDSIZE 6 // The Grid size is 6x6.
#define STARTPOS 1 // 1 is the start of Grid.

enum class Blocks {
	Lock_Invalid,
	Lock1, // 2.
	Lock2, // 2.
	Lock3, // 2.
	Lock4, // 2.
	Lock5, // 2.
	Lock6, // 2.
	Lock7, // 2.
	Lock8, // 2.
	Lock9, // 2.
	Lock10, // 2.
	Lock11, // 2.
	Lock12, // 3.
	Lock13, // 3.
	Lock14, // 3.
	Lock15, // 3.
	Escape // 2.
};

enum class Direction {
	None,
	Vertical,
	Horizontal
};

struct GameField {
	int blocktype;
	Direction direction;
	int index;
};

#endif