#pragma once
#include "utility.h"

#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "Pawn.h"

#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

class GameMaster
{
public:
	//window

	Piece* pawnChange(Piece* piece, int y);
	bool checkIfCheck(bool whiteAttacked);
	void updateKingLocation(int move, Squere pre_squere, Squere this_squere);
	void quick_kings_update();
	bool canCastle(bool left);
	bool checkMate(Squere src_s, Squere dst_s);
	//void updatePieceMoves(Squere srcSq);
	void updatePiecesLoc();
	void updatePiecesMoves();
	void clearEn();

	//setters
	void setTurn(bool whitesTurn_);
	void setBlackKing(int y, int x);
	void setWhiteKing(int y, int x);
	//getters
	bool getWhitesTurn();

	void freeBoardMem();

	Piece* board[BoardSize][BoardSize] = {
	{new Rook(0,0,'r'),new Knight(0,1,'o'),new Bishop(0,2,'b'),new Queen(0,3,'q'),new King(0,4,'k'),new Bishop(0,5,'b'),new Knight(0,6,'o'),new Rook(0,7,'r')},
	{new Pawn(1,0,'p'),new Pawn(1,1,'p'),new Pawn(1,2,'p'),new Pawn(1,3,'p'),new Pawn(1,4,'p'),new Pawn(1,5,'p'),new Pawn(1,6,'p'),new Pawn(1,7,'p')},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Pawn(6,0,'P'),new Pawn(6,1,'P'),new Pawn(6,2,'P'),new Pawn(6,3,'P'),new Pawn(6,4,'P'),new Pawn(6,5,'P'),new Pawn(6,6,'P'),new Pawn(6,7,'P')},
	{new Rook(7,0,'R'),new Knight(7,1,'O'),new Bishop(7,2,'B'),new Queen(7,3,'Q'),new King(7,4,'K'),new Bishop(7,5,'B'),new Knight(7,6,'O'),new Rook(7,7,'R')} };

private:
	bool whitesTurn = true;

	Squere blackKing;
	Squere whiteKing;
	Squere tempKing;
};

