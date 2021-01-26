/*****************************************************************
 * \file   SafeBaseTypes.h
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

#include <memory>

using Byte = unsigned char;

// Unsigned base types
typedef unsigned char uint8;		// 8-bit  unsigned
typedef unsigned short uint16;		// 16-bit unsigned
typedef unsigned int uint32;		// 32-bit unsigned
typedef unsigned long long uint64;  // 64-bit unsigned

// Signed base types
typedef signed char int8;			// 8-bit  signed
typedef signed short int16;			// 16-bit signed
typedef signed int int32;			// 32-bit signed
typedef signed long long int64;		// 64-bit signed

// Pointer size
#ifdef SAFE_PLATFORM_64BITS
typedef uint64 uintptr;
typedef int64 intptr;
#define SAFE_POINTER_SIZE 8
#else
typedef uint32 uintptr;
typedef int32 intptr;
#define SAFE_POINTER_SIZE 4
#endif

// Limits
#define SAFE_MIN_UINT8 ((uint8_t)0x00)
#define SAFE_MIN_UINT16 ((uint16_t)0x0000)
#define SAFE_MIN_UINT32 ((uint32_t)0x00000000)
#define SAFE_MIN_UINT64 ((uint64_t)0x0000000000000000)
#define SAFE_MIN_INT8 ((int8_t)-128)
#define SAFE_MIN_INT16 ((int16_t)-32768)
#define SAFE_MIN_INT32 -((int32_t)2147483648)
#define SAFE_MIN_INT64 -((int64_t)9223372036854775808)
#define SAFE_MIN_FLOAT -(3.402823466e+38f)
#define SAFE_MIN_DOUBLE -(1.7976931348623158e+308)

#define SAFE_MAX_UINT8 ((uint8_t)0xff)
#define SAFE_MAX_UINT16 ((uint16_t)0xffff)
#define SAFE_MAX_UINT32 ((uint32_t)0xffffffff)
#define SAFE_MAX_UINT64 ((uint64_t)0xffffffffffffffff)
#define SAFE_MAX_INT8 ((int8_t)127)
#define SAFE_MAX_INT16 ((int16_t)32767)
#define SAFE_MAX_INT32 ((int32_t)2147483647)
#define SAFE_MAX_INT64 ((int64_t)9223372036854775807)
#define SAFE_MAX_FLOAT (3.402823466e+38f)
#define SAFE_MAX_DOUBLE (1.7976931348623158e+308)

