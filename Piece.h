#pragma once
#include "utility.h"
class Piece
{
public:
	std::list<Move> avalableMoves;
	virtual int movment(Piece *board[][8], int src_x, int src_y, int dst_x, int dst_y);
	//setters
	virtual void setLocation(int y, int x);
	virtual void setMoved(bool hasMoved = true);
	virtual void setEn(bool canEn);
	virtual void setType(char t);
	//getters
	virtual int getLocationY();
	virtual int getLocationX();
	virtual char getType();
	virtual bool getMoved();
	virtual bool getEn();
	virtual int getWorth();

private:
	Squere location;
	int worth = 0;
	char type = '#';
};

