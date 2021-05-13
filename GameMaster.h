#pragma once
#include "utility.h"
#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "Pawn.h"

class GameMaster
{
public:
	void run_game();


private:
	Piece* board[BoardSize][BoardSize];
};

