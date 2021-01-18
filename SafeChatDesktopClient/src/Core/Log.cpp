/*****************************************************************
 * \file   Log.cpp
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

#include "SafePch.h"
#include "Log.h"

#include <stdarg.h>
#include "Utils/StringHelper.h"

SafeLogLevel SafeLog::Severity = SafeLogLevel::None;

template<typename T>
std::string &SafeLog::Replace(std::string &str, const std::string &from, const T &to)
	{
	size_t start_pos = str.find(from);

	if (start_pos == std::string::npos)
		return str;

	return str.replace(start_pos, from.length(), safe_to_string(to));
	}

void SafeLog::SetLogLevel(SafeLogLevel level)
	{
	Severity = level;
	}

void SafeLog::Print(const char *format, ...)
	{
	va_list args;
	std::string out = format;
	va_start(args, format);

	printf(SAFE_CONSOLE_FONT_BRIGHT);
	switch (Severity)
		{
		case SafeLogLevel::FatalLevel:
		case SafeLogLevel::ErrorLevel:
			printf(SAFE_CONSOLE_COLOR_RED);
			break;

		case SafeLogLevel::InfoLevel:
			printf(SAFE_CONSOLE_COLOR_BLUE);
			break;

		case SafeLogLevel::WarningLevel:
			printf(SAFE_CONSOLE_COLOR_YELLOW);
			break;

		case SafeLogLevel::TraceLevel:
			printf(SAFE_CONSOLE_COLOR_GREEN);
			break;

		case SafeLogLevel::None:
		default:
			printf(SAFE_CONSOLE_COLOR_WHITE);
			break;
		}

	int i = 0;
	while (out[i] != '\0')
		{
		if (out[i] == '%')
			{
			switch (out[i + 1])
				{
				case 'c':
					{
					char c = va_arg(args, char);
					out = Replace<char>(out, "%c", c);
					break;
					}

				case 's':
					{
					char *s = va_arg(args, char*);
					out = Replace<char*>(out, "%s", s);
					break;
					}

				case 'd':
					{
					int i = va_arg(args, int);
					out = Replace<int>(out, "%d", i);
					break;
					}

				case 'f':
					{
					double d = va_arg(args, double);
					out = Replace<double>(out, "%f", d);
					break;
					}

				case 'l':
					{
					if (out[i + 2] == 'l') // long long
						{
						long long l = va_arg(args, long long);
						out = Replace<long long>(out, "%ll", l);
						}
					else
						{
						long l = va_arg(args, long);
						out = Replace<long>(out, "%l", l);
						}
					break;
					}

				default:
					break;
				}
			}

		++i;
		}

	va_end(args);
	std::cout << out << std::endl;
	printf(SAFE_CONSOLE_COLOR_WHITE);
	}

