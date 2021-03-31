#pragma once
#include "Piece.h"

class King : public Piece
{
public:
	King(int y, int x, char t);
	int movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y) override;

	//setters
	void setType(char t);
	void setLocation(int y, int x);
	void setMoved(bool hasMoved = true);
	//getters
	bool getMoved();
	int getLocationY();
	int getLocationX();
	char getType();

private:
	char type;
	int worth = 10000;
	Squere location;
	bool moved = false;
};

