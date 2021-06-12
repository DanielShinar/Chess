#include "Bot.h"

Move Bot::get_random_move(Piece* board[][BoardSize])
{
	Piece* chosenOne = NULL;
	Move randMove;


	chosenOne = get_rand_from_list(pieces_who_can_move);
	randMove = get_rand_from_list(chosenOne->avalableMoves);

	return randMove;
}

Move Bot::get_best_move_from_position(Piece* board[][BoardSize], bool whites_turn)
{
	Move bestMove;
	int bestEval = 0;
	Piece* chosenPiece = NULL, * temp = NULL, * srcHolder, * dstHolder;

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
				else if (itMove->move_type == en)
				{

				}
				else if (itMove->move_type == castle)
				{

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
				if (itMove->move_type == legal || itMove->move_type == pawnPush)//not a permanent if
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
				else if (itMove->move_type == en)
				{

				}
				else if (itMove->move_type == castle)
				{

				}
			}
		}
	}
	if (bestEval == getPositonEval(board))
	{
		chosenPiece = get_rand_from_list(pieces_who_can_move);;
		bestMove = get_rand_from_list(chosenPiece->avalableMoves);
	}
	return bestMove;
}

void Bot::choose_move(Piece* board[][BoardSize], bool whites_turn)
{
	setBoardView(board);
	copyBoard();
	this->tempWhitesTurn = whites_turn;
	this->chosenMove = get_best_move_from_position(board, whites_turn);
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

void Bot::setBoardView(Piece* board[][BoardSize])
{
	int index = 0;
	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			switch (board[y][x]->getType())
			{
			case 'K':
				if (board[y][x]->getMoved())
				{
					this->boardView[index] = '1';
					break;
				}
			case 'k':
				if (board[y][x]->getMoved())
				{
					this->boardView[index] = '2';
					break;
				}
			case 'R':
				if (board[y][x]->getMoved())
				{
					this->boardView[index] = '3';
					break;
				}
			case 'r':
				if (board[y][x]->getMoved())
				{
					this->boardView[index] = '4';
					break;
				}
			case 'P':
				if (board[y][x]->getEn())
				{
					this->boardView[index] = '5';
					break;
				}
			case 'p':
				if (board[y][x]->getEn())
				{
					this->boardView[index] = '6';
					break;
				}
			default:
				this->boardView[index] = board[y][x]->getType();
				break;

			}
			index++;

		}
	}
}

void Bot::copyBoard()
{
	tempGM.freeBoardMem();
	int index = 0;
	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			switch (boardView[index])
			{
			case '1':
				tempBoard[y][x] = new King(y, x, 'K');
				tempBoard[y][x]->setMoved(true);
				break;
			case '2':
				tempBoard[y][x] = new King(y, x, 'k');
				tempBoard[y][x]->setMoved(true);
				break;
			case '3':
				tempBoard[y][x] = new Rook(y, x, 'R');
				tempBoard[y][x]->setMoved(true);
				break;
			case '4':
				tempBoard[y][x] = new Rook(y, x, 'r');
				tempBoard[y][x]->setMoved(true);
				break;
			case '5':
				tempBoard[y][x] = new Pawn(y, x, 'P');
				tempBoard[y][x]->setEn(true);
				break;
			case '6':
				tempBoard[y][x] = new Pawn(y, x, 'p');
				tempBoard[y][x]->setEn(true);
				break;
			case '#':
				tempGM.board[y][x] = new Piece();
				break;
			case 'k':
			case 'K':
				tempGM.board[y][x] = new King(y, x, boardView[index]);
				break;
			case 'q':
			case 'Q':
				tempGM.board[y][x] = new Queen(y, x, boardView[index]);
				break;
			case 'r':
			case 'R':
				tempGM.board[y][x] = new Rook(y, x, boardView[index]);
				break;
			case 'b':
			case 'B':
				tempGM.board[y][x] = new Bishop(y, x, boardView[index]);
				break;
			case 'p':
			case 'P':
				tempGM.board[y][x] = new Pawn(y, x, boardView[index]);
				break;
			case 'o':
			case 'O':
				tempGM.board[y][x] = new Knight(y, x, boardView[index]);
				break;
			default:
				break;
			}
			index++;
		}
	}

}

Move Bot::get_move()
{
	return this->chosenMove;
}
