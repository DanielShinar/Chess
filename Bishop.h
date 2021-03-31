#pragma once
#include "Piece.h"

class Bishop : public Piece
{
public:
	Bishop(int y, int x, char t);
	bool movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y);

private:
	int worth = 3;
	Squere location;
	char type;
};

