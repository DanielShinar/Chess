#include "Piece.h"

int Piece::movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
	return 0;
}
void Piece::setLocation(int y, int x)
{
	this->location.y = y;
	this->location.x = x;
}
void Piece::setMoved(bool hasMoved)
{
}
void Piece::setEn(bool canEn)
{
}
void Piece::setType(char t)
{
	this->type = t;
}
char Piece::getType()
{
	return this->type;
}

bool Piece::getMoved()
{
	return false;
}

bool Piece::getEn()
{
	return false;
}

int Piece::getLocationY()
{
	return this->location.y;
}

int Piece::getLocationX()
{
	return this->location.x;
}
