#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <wingdi.h>
#include <cctype>
#include <cstring>
#include <string>

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#define globalVariable static
#define internal static

#define playerBlack false

struct Squere
{
	int x, y;
};
struct RenderState
{
	int Height, Width;
	void* Memory;

	BITMAPINFO BitMapInfo;

};
enum legalMoves
{
	illegal = 0, legal = 1, en = 2, castle = 3
};