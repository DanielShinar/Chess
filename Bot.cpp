#include "Bot.h"

Move Bot::get_random_move(Piece* board[][BoardSize])
{
	Piece* chosenOne = NULL;
	Move randMove;


	chosenOne = get_rand_from_list(pieces_who_can_move);
	randMove = get_rand_from_list(chosenOne->avalableMoves);

	return randMove;
}

void Bot::choose_move(Piece* board[][BoardSize], bool whites_turn)
{
	Move bestMove;
	int bestEval = 0;
	Piece * chosenPiece = NULL, * temp = NULL, * srcHolder, * dstHolder;

	this->position_eval = getPositonEval(board);
	bestEval = getPositonEval(board);
	if (whites_turn)
	{
		for (auto itPiece = pieces_who_can_move.begin(); itPiece != pieces_who_can_move.end(); itPiece++)
		{
			temp = *itPiece;
			for (auto itMove = temp->avalableMoves.begin(); itMove != temp->avalableMoves.end(); itMove++)
			{
				if (itMove->move_type == legal)//not a permanent if
				{
					srcHolder = board[itMove->src_squere.y][itMove->src_squere.x];
					dstHolder = board[itMove->dst_squere.y][itMove->dst_squere.x];
					//change board to check eval
					board[itMove->src_squere.y][itMove->src_squere.x] = new Piece();
					board[itMove->dst_squere.y][itMove->dst_squere.x] = srcHolder;
					if (bestEval < getPositonEval(board))
					{
						bestEval = getPositonEval(board);
						bestMove.src_squere.x = srcHolder->getLocationX();
						bestMove.src_squere.y = srcHolder->getLocationY();
						bestMove.dst_squere.x = dstHolder->getLocationX();
						bestMove.dst_squere.y = dstHolder->getLocationY();
					}
					delete board[temp->getLocationY()][temp->getLocationX()];
					board[temp->getLocationY()][temp->getLocationX()] = srcHolder;
					board[itMove->dst_squere.y][itMove->dst_squere.x] = dstHolder;
				}
			}
		}
	}
	else
	{
		for (auto itPiece = pieces_who_can_move.begin(); itPiece != pieces_who_can_move.end(); itPiece++)
		{
			temp = *itPiece;
			for (auto itMove = temp->avalableMoves.begin(); itMove != temp->avalableMoves.end(); itMove++)
			{
				if (itMove->move_type == legal)//not a permanent if
				{
					srcHolder = board[itMove->src_squere.y][itMove->src_squere.x];
					dstHolder = board[itMove->dst_squere.y][itMove->dst_squere.x];
					//change board to check eval
					board[itMove->src_squere.y][itMove->src_squere.x] = new Piece();
					board[itMove->dst_squere.y][itMove->dst_squere.x] = srcHolder;
					if (bestEval > getPositonEval(board))
					{
						bestEval = getPositonEval(board);
						bestMove.src_squere.x = srcHolder->getLocationX();
						bestMove.src_squere.y = srcHolder->getLocationY();
						bestMove.dst_squere.x = dstHolder->getLocationX();
						bestMove.dst_squere.y = dstHolder->getLocationY();
					}
					delete board[temp->getLocationY()][temp->getLocationX()];
					board[temp->getLocationY()][temp->getLocationX()] = srcHolder;
					board[itMove->dst_squere.y][itMove->dst_squere.x] = dstHolder;
				}
			}
		}
	}
	if (bestEval == getPositonEval(board))
	{
		chosenPiece = get_rand_from_list(pieces_who_can_move);;
		bestMove = get_rand_from_list(chosenPiece->avalableMoves);
	}
	this->chosenMove = bestMove;
}

void Bot::get_pieces(Piece* board[][BoardSize])
{
	pieces_who_can_move.clear();
	if (playerBlack)
	{
		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (isupper(board[y][x]->getType()) && !board[y][x]->avalableMoves.empty())
				{
					this->pieces_who_can_move.push_back(board[y][x]);
				}
			}
		}
	}
	else
	{
		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (islower(board[y][x]->getType()) && !board[y][x]->avalableMoves.empty())
				{
					this->pieces_who_can_move.push_back(board[y][x]);
				}
			}
		}
	}
}

int Bot::getPositonEval(Piece* board[][BoardSize])
{
	int eval = 0;
	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			if(islower(board[y][x]->getType()))
				eval -= board[y][x]->getWorth();
			else
				eval += board[y][x]->getWorth();
		}
	}
	return eval;
}

Move Bot::get_move()
{
	return this->chosenMove;
}
