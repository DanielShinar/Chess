#include "utility.h"

inline int
clamp(int min, int val, int max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

internal void clear_screen(RenderState renderState)
{
	u32 colorA = 0xffffcc;
	u32 colorB = 0x00bb00;
	u32 color = colorB;
	u32 colorTwo = colorB;
	u32* pixel = (u32*)renderState.Memory;
	for (int y = 0; y < renderState.Height; y++)
	{
		for (int x = 0; x < renderState.Width; x++)
		{
			*pixel++ = color;
			if ((x + 1) % (renderState.Width / 8) == 0)
			{
				if (color == colorA)
					color = colorB;
				else
					color = colorA;
			}
		}
		if ((y + 1) % (renderState.Height / 8) == 0)
		{
			if (colorTwo == colorA)
				colorTwo = colorB;
			else
				colorTwo = colorA;
		}
		color = colorTwo;
	}
}

internal void announceWinner(int who_won, RenderState renderState)
{
	u32 colorA = 0xffffcc;
	u32 colorB = 0x00bb00;
	u32* pixel = (u32*)renderState.Memory;
	if (who_won == 1)
	{
		for (int y = 0; y < renderState.Height; y++)
		{
			for (int x = 0; x < renderState.Width; x++)
			{
				*pixel++ = colorA;
			}
		}
	}
	else if(who_won == 2)
	{
		for (int y = 0; y < renderState.Height; y++)
		{
			for (int x = 0; x < renderState.Width; x++)
			{
				*pixel++ = colorB;
			}
		}
	}
}

internal void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color, RenderState renderState)
{
	x0 = clamp(0, x0, renderState.Width);
	x1 = clamp(0, x1, renderState.Width);
	y0 = clamp(0, y0, renderState.Height);
	y1 = clamp(0, y1, renderState.Height);

	for (int y = y0; y < y1; y++)
	{
		u32* pixel = (u32*)renderState.Memory + x0 + y * renderState.Width;
		for (int x = x0; x < x1; x++)
		{
			if (x > x0 && x < x1 && y > y0 && y < y1)
			{
				*pixel++ = color;
			}
		}
	}

}

globalVariable float renderScale = 0.01f;

internal void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color, RenderState renderState)
{
	x *= renderState.Width * renderScale;
	y *= renderState.Width * renderScale;
	half_size_x *= renderState.Width * renderScale;
	half_size_y *= renderState.Width * renderScale;

	x += renderState.Width / 2.f;
	y += renderState.Height / 2.f;
	//change to pixels
	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color, renderState);
}