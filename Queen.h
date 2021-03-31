#pragma once
#include "Piece.h"

class Queen : public Piece
{
public:
	Queen(int y, int x, char t);
	int movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y) override;

	//setters
	void setType(char t);
	void setLocation(int y, int x);
	//getters
	int getLocationY();
	int getLocationX();
	char getType();
private:
	char type;
	Squere location;
	int worth = 8;
};

