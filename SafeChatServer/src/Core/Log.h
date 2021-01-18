/*****************************************************************
 * \file   Log.h
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

enum class SafeLogLevel
	{
	None = 0,
	InfoLevel, TraceLevel, ErrorLevel, FatalLevel, WarningLevel
	};

class SafeLog
	{
	private:
		static SafeLogLevel Severity;

		template<typename T>
		static std::string &Replace(std::string &str, const std::string &find, const T &replaceValue);

	public:

		static void SetLogLevel(SafeLogLevel level);

		static void Print(const char *format, ...);
	};

#define SAFE_LOG_INFO(msg, ...) { SafeLog::SetLogLevel(SafeLogLevel::InfoLevel); SafeLog::Print(msg, __VA_ARGS__); }
#define SAFE_LOG_TRACE(msg, ...) { SafeLog::SetLogLevel(SafeLogLevel::TraceLevel); SafeLog::Print(msg, __VA_ARGS__); }
#define SAFE_LOG_WARN(msg, ...) { SafeLog::SetLogLevel(SafeLogLevel::WarningLevel); SafeLog::Print(msg, __VA_ARGS__); }
#define SAFE_LOG_ERROR(msg, ...) { SafeLog::SetLogLevel(SafeLogLevel::ErrorLevel); SafeLog::Print(msg, __VA_ARGS__); }
#define SAFE_ASSERT(x, msg, ...) { if (!(x)) { SAFE_LOG_ERROR(msg, __VA_ARGS__);  __debugbreak(); } }

