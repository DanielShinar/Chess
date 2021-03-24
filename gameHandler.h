#pragma once
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "utility.cpp"



class gameHandler
{
public:
	bool canMove(char chessboard[][8], char piece ,int src_x, int src_y, int dst_x, int dst_y);

private:
	King king;
	Knight knight;
	Pawn pawn;
};

