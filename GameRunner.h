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
#include "GameMaster.h"
#include "renderer.cpp"

#include <gdiplus.h>

globalVariable HDC pub;
globalVariable HBITMAP hBitmap;

class GameRunner
{
public:
	void re_size(HWND hwnd);

	void run_game(HWND window);

	void play_turn(int move, Squere pre_squere, Squere squere);
	int play_move(int move, Squere pre_squere, Squere squere);
	int do_move(int move, Squere pre_squere, Squere squere);
	bool first_check(Squere squere);
	int get_player_move(Squere s, Squere pre_s);

	void updateSpesificSquers(int move, Squere pre_squere, Squere this_squere);

	void drawPiecesDiff();
	void DrawBitmap(LPCSTR piecePath, int x, int y, HDC dc);
	void clear_my_screen();
	void setMovmentRenderState();
	void drew_piece_move(Squere sq);
	void clear_piece_move(Squere sq);

	//setters
	void setCheck(bool check_);
	void setRunning(bool running_);
	void setNewTurn(bool newTurn_);
	//getters
	bool getNewTurn();
	bool getCheck();
	RenderState getRenderState();

	GameMaster gm;

private:

	Bot AI;

	RenderState renderState;
	RenderState renderMovment;

	bool check = false;
	bool newTurn = true;
	bool running = true;

	int whoWon = 0;

	Piece* tempBoard[BoardSize][BoardSize];
};

