#include "GameMaster.h"

Piece* GameMaster::pawnChange(Piece* piece, int y)
{
	if (playerBlack)
	{
		if (piece->getType() == 'p' && y == 0)
		{
			Piece* newPiece = new Queen(0, piece->getLocationX(), 'q');
			delete piece;
			return newPiece;
		}
		else if (piece->getType() == 'P' && y == 7)
		{
			Piece* newPiece = new Queen(7, piece->getLocationX(), 'Q');
			delete piece;
			return newPiece;
		}
	}
	else
	{
		if (piece->getType() == 'p' && y == 7)
		{
			Piece* newPiece = new Queen(7, piece->getLocationX(), 'q');
			delete piece;
			return newPiece;
		}
		else if (piece->getType() == 'P' && y == 0)
		{
			Piece* newPiece = new Queen(0, piece->getLocationX(), 'Q');
			delete piece;
			return newPiece;
		}
	}
	return piece;
}

bool GameMaster::checkIfCheck(bool whiteAttacked)
{
	int x = 0, y = 0;
	if (whiteAttacked)
	{
		for (y = 0; y < BoardSize; y++)
		{
			for (x = 0; x < BoardSize; x++)
			{
				if (islower(board[y][x]->getType()) && board[y][x]->movment(board, x, y, this->whiteKing.x, this->whiteKing.y))
				{
					return true;
				}
			}
		}
	}
	else
	{
		for (y = 0; y < BoardSize; y++)
		{
			for (x = 0; x < BoardSize; x++)
			{
				if (isupper(board[y][x]->getType()) && board[y][x]->movment(board, x, y, this->blackKing.x, this->blackKing.y))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void GameMaster::updateKingLocation(int move, Squere pre_squere, Squere this_squere)
{
	if (move == castle)
	{
		for (int y = 0; y < BoardSize; y++)//keep the location of the kings and rooks when castling
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (board[y][x]->getType() == 'k')
				{
					this->blackKing.y = y;
					this->blackKing.x = x;
					board[y][x]->setLocation(y, x);
				}
				else if (board[y][x]->getType() == 'K')
				{
					this->whiteKing.y = y;
					this->whiteKing.x = x;
					board[y][x]->setLocation(y, x);
				}
				else if (board[y][x]->getType() == 'r')
				{
					board[y][x]->setLocation(y, x);
				}
				else if (board[y][x]->getType() == 'R')
				{
					board[y][x]->setLocation(y, x);
				}
			}
		}
	}
	else
	{
		if (board[pre_squere.y][pre_squere.x]->getType() == 'k')//always keeps where the kings are
		{
			this->blackKing.y = this_squere.y;
			this->blackKing.x = this_squere.x;
		}
		else if (board[pre_squere.y][pre_squere.x]->getType() == 'K')
		{
			this->whiteKing.y = this_squere.y;
			this->whiteKing.x = this_squere.x;
		}
	}
}

void GameMaster::quick_kings_update()
{
	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			if (board[y][x]->getType() == 'k')
			{
				blackKing.y = y;
				blackKing.x = x;
			}
			else if (board[y][x]->getType() == 'K')
			{
				whiteKing.y = y;
				whiteKing.x = x;
			}
		}
	}
}

bool GameMaster::canCastle(bool left)
{
	if (whitesTurn && left)
	{
		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (islower(board[y][x]->getType()) && board[y][x]->movment(board, x, y, this->whiteKing.x - 1, this->whiteKing.y))
				{
					return false;
				}
				if (islower(board[y][x]->getType()) && board[y][x]->movment(board, x, y, this->whiteKing.x - 2, this->whiteKing.y))
				{
					return false;
				}
			}
		}
	}
	else if (whitesTurn && !left)
	{
		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (islower(board[y][x]->getType()) && board[y][x]->movment(board, x, y, whiteKing.x + 1, whiteKing.y))
				{
					return false;
				}
				if (islower(board[y][x]->getType()) && board[y][x]->movment(board, x, y, whiteKing.x + 2, whiteKing.y))
				{
					return false;
				}
			}
		}
	}
	else if (!whitesTurn && left)
	{
		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (isupper(board[y][x]->getType()) && board[y][x]->movment(board, x, y, blackKing.x - 1, blackKing.y))
				{
					return false;
				}
				if (isupper(board[y][x]->getType()) && board[y][x]->movment(board, x, y, blackKing.x - 2, blackKing.y))
				{
					return false;
				}
			}
		}
	}
	else if (!whitesTurn && !left)
	{
		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (isupper(board[y][x]->getType()) && board[y][x]->movment(board, x, y, blackKing.x + 1, blackKing.y))
				{
					return false;
				}
				if (isupper(board[y][x]->getType()) && board[y][x]->movment(board, x, y, blackKing.x + 2, blackKing.y))
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool GameMaster::checkMate(Squere src_s, Squere dst_s)
{
	Piece* holderFirst;
	Piece* holderSecond;

	if (!whitesTurn)
	{
		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (isupper(board[y][x]->getType()))
				{
					for (int yTemp = 0; yTemp < BoardSize; yTemp++)
					{
						for (int xTemp = 0; xTemp < BoardSize; xTemp++)
						{
							if (board[yTemp][xTemp]->getType() == '#' || islower(board[yTemp][xTemp]->getType()))
							{
								if (board[y][x]->movment(board, x, y, xTemp, yTemp))
								{
									holderFirst = board[y][x];
									holderSecond = board[yTemp][xTemp];
									board[y][x] = new Piece();
									board[yTemp][xTemp] = holderFirst;
									if (board[yTemp][xTemp]->getType() == 'K')
									{
										this->whiteKing.x = xTemp;
										this->whiteKing.y = yTemp;
									}
									if (!checkIfCheck(!whitesTurn))
									{
										delete board[y][x];
										board[y][x] = holderFirst;
										board[yTemp][xTemp] = holderSecond;
										quick_kings_update();
										return false;
									}
									quick_kings_update();
									if (board[yTemp][xTemp]->getType() == 'K')
									{
										this->whiteKing.x = x;
										this->whiteKing.y = y;
									}
									delete board[y][x];
									board[y][x] = holderFirst;
									board[yTemp][xTemp] = holderSecond;
									quick_kings_update();
								}
							}
						}
					}
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
				if (islower(board[y][x]->getType()))
				{
					for (int yTemp = 0; yTemp < BoardSize; yTemp++)
					{
						for (int xTemp = 0; xTemp < BoardSize; xTemp++)
						{
							if (board[yTemp][xTemp]->getType() == '#' || isupper(board[yTemp][xTemp]->getType()))
							{
								if (board[y][x]->movment(board, x, y, xTemp, yTemp))
								{
									holderFirst = board[y][x];
									holderSecond = board[yTemp][xTemp];
									board[y][x] = new Piece();
									board[yTemp][xTemp] = holderFirst;
									if (board[yTemp][xTemp]->getType() == 'k')
									{
										this->blackKing.x = xTemp;
										this->blackKing.y = yTemp;
									}
									if (!checkIfCheck(!whitesTurn))
									{
										delete board[y][x];
										board[y][x] = holderFirst;
										board[yTemp][xTemp] = holderSecond;
										quick_kings_update();
										return false;
									}
									if (board[yTemp][xTemp]->getType() == 'k')
									{
										this->blackKing.x = xTemp;
										this->blackKing.y = yTemp;
									}
									delete board[y][x];
									board[y][x] = holderFirst;
									board[yTemp][xTemp] = holderSecond;
									quick_kings_update();
								}
							}
						}
					}
				}
			}
		}
	}
	return true;
}

//void GameMaster::updatePieceMoves(Squere srcSq)
//{
//	Squere sq;
//	Piece* piece = this->board[srcSq.y][srcSq.x];
//	Piece* firstHolder;
//	Piece* secondHolder;
//	int moveGood;
//	this->pieceMoves.clear();
//	for (int y = 0; y < BoardSize; y++)
//	{
//		for (int x = 0; x < BoardSize; x++)
//		{
//			sq.x = x;
//			sq.y = y;
//			moveGood = piece->movment(this->board, srcSq.x, srcSq.y, x, y);
//			if (moveGood)
//			{
//				if (this->board[srcSq.y][srcSq.x]->getType() == 'K' || this->board[srcSq.y][srcSq.x]->getType() == 'k')
//				{
//					updateKingLocation(moveGood, srcSq, sq);
//				}
//
//				firstHolder = this->board[srcSq.y][srcSq.x];
//				secondHolder = this->board[y][x];
//				this->board[srcSq.y][srcSq.x] = new Piece();
//				this->board[y][x] = firstHolder;
//
//				if (checkIfCheck(whitesTurn))
//				{
//					updateKingLocation(moveGood, sq, srcSq);
//					moveGood = illegal;
//				}
//				delete this->board[srcSq.y][srcSq.x];
//				this->board[srcSq.y][srcSq.x] = firstHolder;
//				this->board[y][x] = secondHolder;
//				if (moveGood != illegal)
//				{
//					this->pieceMoves.push_back(sq);
//				}
//			}
//		}
//	}
//}

void GameMaster::updatePiecesLoc()
{
	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			board[y][x]->setLocation(y, x);
		}
	}
}

void GameMaster::updatePiecesMoves()
{
	Move m;
	Squere sq, srcSq;
	Piece* piece;
	Piece* firstHolder;
	Piece* secondHolder;
	int moveGood;

	for (int src_y = 0; src_y < BoardSize; src_y++)
	{
		for (int src_x = 0; src_x < BoardSize; src_x++)
		{
			quick_kings_update();
			srcSq.y = src_y;
			srcSq.x = src_x;
			piece = board[src_y][src_x];
			board[src_y][src_x]->avalableMoves.clear();
			for (int y = 0; y < BoardSize; y++)
			{
				for (int x = 0; x < BoardSize; x++)
				{
					sq.x = x;
					sq.y = y;
					moveGood = piece->movment(board, srcSq.x, srcSq.y, x, y);
					m.move_type = moveGood;
					if (moveGood)
					{
						if (board[srcSq.y][srcSq.x]->getType() == 'K' || board[srcSq.y][srcSq.x]->getType() == 'k')
						{
							updateKingLocation(moveGood, srcSq, sq);
						}

						firstHolder = board[srcSq.y][srcSq.x];
						secondHolder = board[y][x];
						board[srcSq.y][srcSq.x] = new Piece();
						board[y][x] = firstHolder;

						if (checkIfCheck(whitesTurn))
						{
							updateKingLocation(moveGood, sq, srcSq);
							moveGood = illegal;
						}
						delete board[srcSq.y][srcSq.x];
						board[srcSq.y][srcSq.x] = firstHolder;
						board[y][x] = secondHolder;
						if (moveGood)
						{
							m.src_squere = srcSq;
							m.dst_squere = sq;
							board[srcSq.y][srcSq.x]->avalableMoves.push_back(m);
						}
					}
				}
			}
		}
	}
}

void GameMaster::clearEn()
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if ((board[y][x]->getType() == 'p' || board[y][x]->getType() == 'P') && board[y][x]->getEn())
			{
				board[y][x]->setEn(false);
			}
		}
	}
}

void GameMaster::setTurn(bool whitesTurn_)
{
	this->whitesTurn = whitesTurn_;
}

void GameMaster::setBlackKing(int y, int x)
{
	this->blackKing.y = y;
	this->blackKing.x = x;
}

void GameMaster::setWhiteKing(int y, int x)
{
	this->whiteKing.y = y;
	this->whiteKing.x = x;
}

bool GameMaster::getWhitesTurn()
{
	return this->whitesTurn;
}

void GameMaster::freeBoardMem()
{
	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			if(this->board[y][x] == NULL)
				delete this->board[y][x];
			this->board[y][x] = NULL;
		}
	}
}
