#pragma once
class Piece
{
public:
	virtual bool movment(int src_x, int src_y, int dst_x, int dst_y);
private:
	char kind;
};

