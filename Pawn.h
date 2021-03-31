#pragma once
#include "Piece.h"

class Pawn : public Piece
{
public:
	Pawn(int y, int x, char t);
	int movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y) override;

	//setters
	void setType(char t);
	void setLocation(int y, int x);
	void setEn(bool check);
	//getters
	bool getEn();
	int getLocationY();
	int getLocationX();
	char getType();

private:
	char type;
	Squere location;
	bool En = false;
	int worth = 1;
};

