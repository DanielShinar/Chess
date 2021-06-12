#pragma once
#include "utility.h"
#include "GameMaster.h"
#include "Piece.h"
#include <random>
class Bot
{
public:
	template <class T>
	T get_rand_from_list(std::list<T> list);
	Move get_random_move(Piece* board[][BoardSize]);
	Move get_best_move_from_position(Piece* board[][BoardSize], bool whites_turn);

	Move MinMax(Piece* board[][BoardSize], bool whites_turn, int ahead, int eval);//needs to make sure

	void choose_move(Piece* board[][BoardSize], bool whites_turn);
	void get_pieces(Piece* board[][BoardSize]);
	int getPositonEval(Piece* board[][BoardSize]);
	void setBoardView(Piece* board[][BoardSize]);
	void copyBoard();//might have problam of changing real board

	//getters
	Move get_move();
private:
	GameMaster tempGM;

	int position_eval = 0;
	std::list<Piece*>pieces_who_can_move;
	Move chosenMove;

	char boardView[BoardSize * BoardSize];
	Piece* tempBoard[BoardSize][BoardSize];

	bool tempWhitesTurn = true;

};

template<class T>
inline T Bot::get_rand_from_list(std::list<T> list)
{
	srand(time(NULL));
	int index = rand() % list.size();
	int indexCounter = 0;
	for (auto it = list.begin(); it != list.end(); it++)
	{
		if (indexCounter == index)
		{
			return *it;
		}
		indexCounter++;
	}
}
