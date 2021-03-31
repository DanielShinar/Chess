#pragma once
#include "utility.h"
class Piece
{
public:
	virtual bool movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y);
	//setters
	void setLocation(int y, int x);
	void setMoved(bool hasMoved = true);
	void setEn(bool canEn);
	//getters
	int getLocationY();
	int getLocationX();
	char getType();
	bool getMoved();
	bool getEn();

private:
	Squere location;
	int worth = 0;
	char type = '#';
};

