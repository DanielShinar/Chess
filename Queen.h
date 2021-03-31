#pragma once
#include "Piece.h"

class Queen : public Piece
{
public:
	Queen(int y, int x, char t);
	bool movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y);

private:
	char type;
	Squere location;
	int worth = 8;
};

