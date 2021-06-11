#pragma once
#include "utility.h"
#include "Bot.h"

#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "Pawn.h"
#include "renderer.cpp"

#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

globalVariable HDC pub;
globalVariable HBITMAP hBitmap;

class GameMaster
{
public:
	//window
	void re_size(HWND hwnd);


	void run_game(HWND window);

	void play_turn(int move, Squere pre_squere, Squere squere);
	int play_move(int move, Squere pre_squere, Squere squere);
	int do_move(int move, Squere pre_squere, Squere squere);
	bool first_check(Squere squere);
	int get_player_move(Squere s, Squere pre_s);

	void drawPiecesDiff();
	void DrawBitmap(LPCSTR piecePath, int x, int y, HDC dc);
	void clear_my_screen();
	void setMovmentRenderState();
	void drew_piece_move(Squere sq);
	void clear_piece_move(Squere sq);


	Piece* pawnChange(Piece* piece, int y);
	bool checkIfCheck(bool whiteAttacked);
	void updateSpesificSquers(int move, Squere pre_squere, Squere this_squere);
	void updateKingLocation(int move, Squere pre_squere, Squere this_squere);
	void quick_kings_update();
	bool canCastle(bool left);
	bool checkMate(Squere src_s, Squere dst_s);
	//void updatePieceMoves(Squere srcSq);
	void updatePiecesLoc();
	void updatePiecesMoves();
	void clearEn();


	//setters
	void setCheck(bool check_);
	void setRunning(bool running_);
	void setNewTurn(bool newTurn_);
	//getters
	bool getNewTurn();
	bool getCheck();
	RenderState getRenderState();

private:
	Bot AI;

	Piece* board[BoardSize][BoardSize] = {
	{new Rook(0,0,'r'),new Knight(0,1,'o'),new Bishop(0,2,'b'),new Queen(0,3,'q'),new King(0,4,'k'),new Bishop(0,5,'b'),new Knight(0,6,'o'),new Rook(0,7,'r')},
	{new Pawn(1,0,'p'),new Pawn(1,1,'p'),new Pawn(1,2,'p'),new Pawn(1,3,'p'),new Pawn(1,4,'p'),new Pawn(1,5,'p'),new Pawn(1,6,'p'),new Pawn(1,7,'p')},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Pawn(6,0,'P'),new Pawn(6,1,'P'),new Pawn(6,2,'P'),new Pawn(6,3,'P'),new Pawn(6,4,'P'),new Pawn(6,5,'P'),new Pawn(6,6,'P'),new Pawn(6,7,'P')},
	{new Rook(7,0,'R'),new Knight(7,1,'O'),new Bishop(7,2,'B'),new Queen(7,3,'Q'),new King(7,4,'K'),new Bishop(7,5,'B'),new Knight(7,6,'O'),new Rook(7,7,'R')} };

	/*Piece* board[BoardSize][BoardSize] = {
	{new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece(), new Piece()},
	{new Piece(), new Queen(1,1,'Q'), new Piece(), new Piece(), new Piece(), new Piece(), new Rook(1,6,'R'), new Piece()},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new King(3, 0, 'k'),new Pawn(3,1,'p'),new Piece(),new Pawn(3,3,'p'),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Piece(),new Piece(),new Pawn(4,2,'p'),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Pawn(5, 0,'p'),new Piece(),new Pawn(5,2,'P'),new Piece(),new Piece(),new Piece(),new King(5,6,'K'),new Piece()},
	{new Pawn(6,0,'P'),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Piece(),new Piece(),new Piece(),new Rook(7,3,'R'),new Piece(),new Piece(),new Piece(),new Piece()} };*/

	Squere blackKing;
	Squere whiteKing;
	Squere tempKing;
	RenderState renderState;
	RenderState renderMovment;

	bool whitesTurn = true;
	bool check = false;
	bool newTurn = true;
	bool running = true;

	int whoWon = 0;
};

