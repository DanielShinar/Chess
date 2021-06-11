#pragma once
#include "Piece.h"
class Knight : public Piece
{
public:
	Knight(int y, int x, char t);
	int movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y) override;

	//setters
	void setType(char t);
	void setLocation(int y, int x);
	//getters
	int getLocationY();
	int getLocationX();
	char getType();
	int getWorth();

private:
	char type;
	int worth = 30;
	Squere location;
};

