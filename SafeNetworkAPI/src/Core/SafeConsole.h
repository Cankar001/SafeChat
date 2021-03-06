/*****************************************************************
 * \file   SafeConsole.h
 * \brief
 *
 * \author Can Karka
 * \date   January 2021
 *
 * Copyright (C) 2021 Can Karka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/.
 *********************************************************************/

#pragma once

#include <SafeAsioInclude.h>

 // @See: https://stackoverflow.com/a/63426872/12873837

// Console Colors
#define SAFE_CONSOLE_COLOR_BLACK				  0x00
#define SAFE_CONSOLE_COLOR_RED					  0x04
#define SAFE_CONSOLE_COLOR_GREEN				  0x02
#define SAFE_CONSOLE_COLOR_YELLOW				  0x06
#define SAFE_CONSOLE_COLOR_BLUE					  0x01
#define SAFE_CONSOLE_COLOR_WHITE				  0x07

#define SAFE_CONSOLE_COLOR_LIGHT_RED              0x0C
#define SAFE_CONSOLE_COLOR_LIGHT_GREEN            0x0A
#define SAFE_CONSOLE_COLOR_LIGHT_YELLOW           0x0E
#define SAFE_CONSOLE_COLOR_LIGHT_BLUE             0x09
#define SAFE_CONSOLE_COLOR_LIGHT_WHITE            0x0F

inline void safe_set_console_color(int color = 0x07)
	{
#ifdef SAFE_PLATFORM_WINDOWS
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
	}
