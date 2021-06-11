#pragma once

#include <Windows.h>
#include <time.h>
#include <windowsx.h>
#include <wingdi.h>
#include <cctype>
#include <cstring>
#include <string>
#include <list>

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

#define BoardSize 8

#define dst 80
#define src 30

enum legalMoves
{
	illegal = 0, legal = 1, en = 2, castle = 3, pawnPush = 4
};
struct Squere
{
	int y, x;
	inline Squere operator=(Squere a) 
	{
		x = a.x;
		y = a.y;
		return a;
	}
};
struct Move
{
	Squere src_squere, dst_squere;
	int move_type = legal;
};
struct RenderState
{
	int Height, Width;
	void* Memory;

	BITMAPINFO BitMapInfo;

};