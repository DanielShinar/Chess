#pragma once
#include "Piece.h"

class King : Piece
{
public:
	bool movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y);
private:
	int worth = 10000;


};

