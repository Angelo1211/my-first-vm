#pragma once

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <string.h>

typedef uint8_t u8;
#define U8_MIN 0
#define U8_MAX UINT8_MAX
typedef uint16_t u16;
#define U16_MIN 0
#define U16_MAX UINT16_MAX
typedef uint32_t u32;
#define U32_MIN 0
#define U32_MAX UINT32_MAX
typedef uint64_t u64;
#define U64_MIN 0
#define U64_MAX UINT64_MAX

typedef int8_t s8;
#define S8_MIN INT8_MIN
#define S8_MAX INT8_MAX
typedef int16_t s16;
#define S16_MIN INT16_MIN
#define S16_MAX INT16_MAX
typedef int32_t s32;
#define S32_MIN INT32_MIN
#define S32_MAX INT32_MAX
typedef int64_t s64;
#define S64_MIN INT64_MIN
#define S64_MAX INT64_MAX

typedef float f32;
#define F32_MIN FLT_MIN
#define F32_MAX FLT_MAX

typedef double f64;
#define F64_MIN DBL_MIN
#define F64_MAX DBL_MAX

typedef bool b32;

// Files
char *Read_Entire_File(char *file_name, size_t *size);

// Arrays
#define NELEM(array) (sizeof(array)/sizeof(array[0]))