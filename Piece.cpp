#include "Piece.h"

bool Piece::movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
	return false;
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
