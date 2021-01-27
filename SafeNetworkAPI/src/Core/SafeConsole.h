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

 // See: https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c

// Console Colors
#define SAFE_CONSOLE_COLOR_BLACK            "\033[30m"
#define SAFE_CONSOLE_COLOR_RED              "\033[31m"
#define SAFE_CONSOLE_COLOR_GREEN            "\033[32m"
#define SAFE_CONSOLE_COLOR_YELLOW           "\033[33m"
#define SAFE_CONSOLE_COLOR_BLUE             "\033[34m"
#define SAFE_CONSOLE_COLOR_MAGENTA          "\033[35m"
#define SAFE_CONSOLE_COLOR_CYAN             "\033[36m"
#define SAFE_CONSOLE_COLOR_WHITE            "\033[37m"

// Console Fonts
#define SAFE_CONSOLE_FONT_BRIGHT            "\033[1m"
#define SAFE_CONSOLE_FONT_DARK              "\033[2m"    // abgedunkelte Schrift
#define SAFE_CONSOLE_FONT_ITALIC            "\033[3m" 
#define SAFE_CONSOLE_FONT_UNDERLINE         "\033[4m"    // Unterstrichen
#define SAFE_CONSOLE_FONT_BLINK             "\033[5m"    // Blinken ein
#define SAFE_CONSOLE_FONT_QBLINK            "\033[6m"    // schnelles Blinken ein
#define SAFE_CONSOLE_FONT_REVERSE           "\033[7m"    // reverse video
#define SAFE_CONSOLE_FONT_NOECHO            "\033[8m"
#define SAFE_CONSOLE_FONT_NORMAL            "\033[0m"    // Alles wieder normal

// Console Cursor
#define SAFE_CONSOLE_CURSOR_CLSCR           "\033[2J"
#define SAFE_CONSOLE_CURSOR_CLREOL          "\033[K"     // Loescht bis Zeilenende
#define SAFE_CONSOLE_CURSOR_SAVECURPOS      "\033[s"     // speicher actuelle Position

#define SAFE_CONSOLE_CURSOR_SETSAVEDPOS     "\033[u"     // geht zur gespeicherten Position

#define SAFE_CONSOLE_CURSOR_SCURUP          "\033[A"     // Eine Zeile hoch
#define SAFE_CONSOLE_CURSOR_SCURDOWN        "\033[B"     // eine Zeile runter

#define SAFE_CONSOLE_CURSOR_SCURRIGHT       "\033[C"     // eine Zeile nach rechts
#define SAFE_CONSOLE_CURSOR_SCURLEFT        "\033[D"     // eine Spalte nach links

#define SAFE_CONSOLE_CURSOR_CURUP(x)        "\033[xA"   // Gehe x Zeilen nach oben
#define SAFE_CONSOLE_CURSOR_CURDOWN(x)      "\033[xB"   // gehe x Zeile nach unten
#define SAFE_CONSOLE_CURSOR_CURRIGHT(x)     "\033[xC"   // gehe x Spalten nach rechts
#define SAFE_CONSOLE_CURSOR_CURLEFT(x)      "\033[xD"   // gehe x Spalten nach links
#define SAFE_CONSOLE_CURSOR_GOTO(x, y)      "\033[x;yH" // Gehe zur Position x,y

#define SAFE_CONSOLE_CURSOR_CURGOHOME(x)    "\033[xf"   // Geh nach Zeile x und setze Curser auf Anfang
#define SAFE_CONSOLE_CURSOR_CURDOWNHOME(x)  "\033[xE"   // Gehe x Zeilen nach  unten und setzte Curser an den Anfang
#define SAFE_CONSOLE_CURSOR_CURUPHOME(x)    "\033[xF"   // Gehe x Zeilen nach oben und setze Curser an den Anfang


