#pragma once

using Byte = unsigned char;

// Unsigned base types
typedef unsigned char uint8;		// 8-bit  unsigned
typedef unsigned short uint16;		// 16-bit unsigned
typedef unsigned int uint32;		// 32-bit unsigned
typedef unsigned long ULONG;		// 32-Bit unsigned
typedef unsigned long long uint64;  // 64-bit unsigned

// Signed base types
typedef signed char int8;			// 8-bit  signed
typedef signed short int16;			// 16-bit signed
typedef signed int int32;			// 32-bit signed
typedef signed long LONG;			// 32-bit signed
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

// Invalid ids
#define SAFE_INVALID_ID_U64 18446744073709551615UL
#define SAFE_INVALID_ID 4294967295U
#define SAFE_INVALID_ID_U16 65535U
#define SAFE_INVALID_ID_U8 255U

// Limits
#define SAFE_MIN_UINT8 ((uint8)0x00)
#define SAFE_MIN_UINT16 ((uint16)0x0000)
#define SAFE_MIN_UINT32 ((uint32)0x00000000)
#define SAFE_MIN_UINT64 ((uint64)0x0000000000000000)
#define SAFE_MIN_INT8 ((int8)-128)
#define SAFE_MIN_INT16 ((int16)-32768)
#define SAFE_MIN_INT32 -((int32)2147483648)
#define SAFE_MIN_INT64 -((int64)9223372036854775808)
#define SAFE_MIN_FLOAT -(3.402823466e+38f)
#define SAFE_MIN_DOUBLE -(1.7976931348623158e+308)

#define SAFE_MAX_UINT8 ((uint8)0xff)
#define SAFE_MAX_UINT16 ((uint16)0xffff)
#define SAFE_MAX_UINT32 ((uint32)0xffffffff)
#define SAFE_MAX_UINT64 ((uint64)0xffffffffffffffff)
#define SAFE_MAX_INT8 ((int8)127)
#define SAFE_MAX_INT16 ((int16)32767)
#define SAFE_MAX_INT32 ((int32)2147483647)
#define SAFE_MAX_INT64 ((int64)9223372036854775807)
#define SAFE_MAX_FLOAT (3.402823466e+38f)
#define SAFE_MAX_DOUBLE (1.7976931348623158e+308)

#ifndef SAFE_PLATFORM_THREADS_LIMIT
#define SAFE_PLATFORM_THREADS_LIMIT 64
#endif

