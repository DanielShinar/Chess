#pragma once
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "utility.h"



class gameHandler
{
public:
	bool canMove(char chessboard[8][8], char piece ,int src_x, int src_y, int dst_x, int dst_y);

private:
	King king = King(0,0,'k');
	Queen queen = Queen(0, 0, 'k');
	Rook rook = Rook(0, 0, 'k');
	Bishop bishop = Bishop(0, 0, 'k');
	Knight knight = Knight(0, 0, 'k');
	Pawn pawn = Pawn(0, 0, 'k');
	
	
};

