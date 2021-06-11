#pragma once
#include "utility.h"
#include "Piece.h"
#include <random>
class Bot
{
public:
	template <class T>
	T get_rand_from_list(std::list<T> list);
	Move get_random_move(Piece* board[][BoardSize]);
	void choose_move(Piece* board[][BoardSize], bool whites_turn);
	void get_pieces(Piece* board[][BoardSize]);
	int getPositonEval(Piece* board[][BoardSize]);

	//getters
	Move get_move();
private:
	int position_eval = 0;
	std::list<Piece*>pieces_who_can_move;
	Move chosenMove;

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
