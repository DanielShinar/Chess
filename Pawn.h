#pragma once
#include "Piece.h"
#include "utility.cpp"

class Pawn : Piece
{
public:
	bool movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y);
private:
	bool En = false;
	int worth = 1;
};

