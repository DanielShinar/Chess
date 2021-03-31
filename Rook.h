#pragma once
#include "Piece.h"
class Rook : public Piece
{
public:
	Rook(int y, int x, char t);
	int movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y) override;

	//setters
	void setType(char t);
	void setLocation(int y, int x);
	void setMoved(bool hasMoved = true);
	//getters
	int getLocationY();
	int getLocationX();
	char getType();
	bool getMoved();

private:
	char type;
	int worth = 5;
	Squere location;
	bool moved = false;
};

