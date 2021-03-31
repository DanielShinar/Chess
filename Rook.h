#pragma once
#include "Piece.h"
class Rook : public Piece
{
public:
	Rook(int y, int x, char t);
	bool movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y);

	//setters
	void setMoved(bool hasMoved = true);
	//getters
	bool getMoved();

private:
	char type;
	int worth = 5;
	Squere location;
	bool moved = false;
};

