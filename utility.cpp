#include <cstring>

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

#define black_p L"C:\\Users\\USER\\Desktop\\white-Pawn.bmp";
#define black_o "dsad";
#define black_b "dsad";
#define black_r "dsad";
#define black_q "dsad";
#define black_k "dsad";

#define white_p "C:\\Users\\USER\\Desktop\\white-Pawn.bmp";
#define white_o "dsad";
#define white_b "dsad";
#define white_r "dsad";
#define white_q "dsad";
#define white_k "dsad";

inline int
clamp(int min, int val, int max)
{
	if (val < min)
		return min;
	if (val > max)
		return max;
	return val;

}