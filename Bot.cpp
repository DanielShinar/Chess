#include "Bot.h"

// ── deep copy real board into tempGM.board ────────────────────────────────────
void Bot::deepCopyBoard(Piece* src[][BoardSize])
{
	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			delete tempGM.board[y][x];
			char t = src[y][x]->getType();
			switch (tolower(t))
			{
			case 'k': tempGM.board[y][x] = new King(y, x, t);   break;
			case 'q': tempGM.board[y][x] = new Queen(y, x, t);  break;
			case 'r': tempGM.board[y][x] = new Rook(y, x, t);   break;
			case 'b': tempGM.board[y][x] = new Bishop(y, x, t); break;
			case 'o': tempGM.board[y][x] = new Knight(y, x, t); break;
			case 'p': tempGM.board[y][x] = new Pawn(y, x, t);   break;
			default:  tempGM.board[y][x] = new Piece();          break;
			}
			tempGM.board[y][x]->setMoved(src[y][x]->getMoved());
			tempGM.board[y][x]->setEn(src[y][x]->getEn());
		}
	}
}

// ── apply a move on tempGM.board (does NOT call quick_kings_update) ───────────
void Bot::apply_move(Move& m, MoveUndo& undo)
{
	int sy = m.src_squere.y, sx = m.src_squere.x;
	int dy = m.dst_squere.y, dx = m.dst_squere.x;

	undo.src_piece       = tempGM.board[sy][sx];
	undo.dst_piece       = tempGM.board[dy][dx];
	undo.en_piece        = nullptr;
	undo.was_promotion   = false;
	undo.src_moved_before = undo.src_piece->getMoved();
	undo.dst_moved_before = undo.dst_piece->getMoved();
	undo.src_en_before   = undo.src_piece->getEn();

	if (m.move_type == castle)
	{
		int king_dx = (dx > sx) ? sx + 2 : sx - 2;
		int rook_dx = (dx > sx) ? sx + 1 : sx - 1;

		// king_dx / rook_dx must be empty for legal castle — safe to delete
		delete tempGM.board[sy][king_dx];
		delete tempGM.board[sy][rook_dx];
		tempGM.board[sy][king_dx] = undo.src_piece;   // king
		tempGM.board[sy][rook_dx] = undo.dst_piece;   // rook
		tempGM.board[sy][sx]      = new Piece();
		tempGM.board[sy][dx]      = new Piece();       // dy == sy
		undo.src_piece->setLocation(sy, king_dx);
		undo.dst_piece->setLocation(sy, rook_dx);
		undo.src_piece->setMoved(true);
		undo.dst_piece->setMoved(true);
	}
	else if (m.move_type == en)
	{
		// captured pawn sits beside the destination square
		bool src_is_white = isupper(undo.src_piece->getType());
		int cap_y = (!playerBlack) ? (src_is_white ? dy + 1 : dy - 1)
		                           : (src_is_white ? dy - 1 : dy + 1);
		undo.en_sq    = { cap_y, dx };
		undo.en_piece = tempGM.board[cap_y][dx];
		tempGM.board[cap_y][dx] = new Piece();
		tempGM.board[dy][dx]    = undo.src_piece;
		tempGM.board[sy][sx]    = new Piece();
		undo.src_piece->setLocation(dy, dx);
	}
	else
	{
		// legal or pawnPush
		tempGM.board[dy][dx] = undo.src_piece;
		tempGM.board[sy][sx] = new Piece();
		undo.src_piece->setLocation(dy, dx);

		if (m.move_type == pawnPush)
			undo.src_piece->setEn(true);

		char t = undo.src_piece->getType();

		// track first-move flag for king/rook (affects future castling legality)
		if (t == 'K' || t == 'k' || t == 'R' || t == 'r')
			undo.src_piece->setMoved(true);

		// pawn promotion — always to queen
		bool promotes = (!playerBlack) ? ((t == 'P' && dy == 0) || (t == 'p' && dy == 7))
		                               : ((t == 'p' && dy == 0) || (t == 'P' && dy == 7));
		if (promotes)
		{
			char qt = isupper(t) ? 'Q' : 'q';
			tempGM.board[dy][dx] = new Queen(dy, dx, qt);
			undo.was_promotion = true;
			// undo.src_piece (the pawn) is saved and will be restored on undo
		}
	}
}

// ── undo a move on tempGM.board (does NOT call quick_kings_update) ────────────
void Bot::undo_move(Move& m, MoveUndo& undo)
{
	int sy = m.src_squere.y, sx = m.src_squere.x;
	int dy = m.dst_squere.y, dx = m.dst_squere.x;

	if (m.move_type == castle)
	{
		int king_dx = (dx > sx) ? sx + 2 : sx - 2;
		int rook_dx = (dx > sx) ? sx + 1 : sx - 1;

		delete tempGM.board[sy][sx];           // empty placed in apply
		tempGM.board[sy][sx] = undo.src_piece; // king back
		undo.src_piece->setLocation(sy, sx);
		undo.src_piece->setMoved(undo.src_moved_before);

		delete tempGM.board[sy][dx];           // empty placed in apply
		tempGM.board[sy][dx] = undo.dst_piece; // rook back
		undo.dst_piece->setLocation(sy, dx);   // dy == sy
		undo.dst_piece->setMoved(undo.dst_moved_before);

		// restore the squares king/rook vacated to empty
		tempGM.board[sy][king_dx] = new Piece();
		tempGM.board[sy][rook_dx] = new Piece();
	}
	else if (m.move_type == en)
	{
		tempGM.board[dy][dx] = undo.dst_piece;    // restore empty at en-capture dst
		delete tempGM.board[sy][sx];              // empty placed in apply
		tempGM.board[sy][sx] = undo.src_piece;    // capturing pawn back
		undo.src_piece->setLocation(sy, sx);
		undo.src_piece->setMoved(undo.src_moved_before);
		undo.src_piece->setEn(undo.src_en_before);
		delete tempGM.board[undo.en_sq.y][undo.en_sq.x]; // empty placed in apply
		tempGM.board[undo.en_sq.y][undo.en_sq.x] = undo.en_piece; // captured pawn back
	}
	else
	{
		if (undo.was_promotion)
			delete tempGM.board[dy][dx];   // delete the queen we created

		delete tempGM.board[sy][sx];       // empty placed in apply
		tempGM.board[sy][sx] = undo.src_piece;
		undo.src_piece->setLocation(sy, sx);
		undo.src_piece->setMoved(undo.src_moved_before);
		undo.src_piece->setEn(undo.src_en_before);
		tempGM.board[dy][dx] = undo.dst_piece; // restore captured / empty piece
	}
}

// ── recursive MinMax worker — returns a position evaluation score ─────────────
int Bot::minimax_score(bool whites_turn, int depth, int alpha, int beta)
{
	if (depth == 0)
		return getPositonEval(tempGM.board);

	bool maximizing    = whites_turn;
	int  best          = maximizing ? INT_MIN : INT_MAX;
	bool has_legal_move = false;
	bool cutoff        = false;

	for (int sy = 0; sy < BoardSize && !cutoff; sy++)
	{
		for (int sx = 0; sx < BoardSize && !cutoff; sx++)
		{
			char src_t = tempGM.board[sy][sx]->getType();
			if (src_t == '#') continue;
			if ( whites_turn && !isupper(src_t)) continue;
			if (!whites_turn && !islower(src_t)) continue;

			for (int dy = 0; dy < BoardSize && !cutoff; dy++)
			{
				for (int dx = 0; dx < BoardSize && !cutoff; dx++)
				{
					int move_t = tempGM.board[sy][sx]->movment(tempGM.board, sx, sy, dx, dy);
					if (!move_t) continue;

					Move m;
					m.src_squere = { sy, sx };
					m.dst_squere = { dy, dx };
					m.move_type  = move_t;

					MoveUndo undo;
					apply_move(m, undo);
					tempGM.quick_kings_update();

					if (!tempGM.checkIfCheck(whites_turn))
					{
						has_legal_move = true;
						int score = minimax_score(!whites_turn, depth - 1, alpha, beta);

						if (maximizing)
						{
							if (score > best) best = score;
							if (score > alpha) alpha = score;
						}
						else
						{
							if (score < best) best = score;
							if (score < beta)  beta  = score;
						}
						if (alpha >= beta) cutoff = true;
					}

					undo_move(m, undo);
					tempGM.quick_kings_update();
				}
			}
		}
	}

	if (!has_legal_move)
	{
		if (tempGM.checkIfCheck(whites_turn))
			return maximizing ? -1000000 : 1000000; // checkmate
		return 0; // stalemate
	}

	return best;
}

// ── top-level MinMax: returns the best move for whites_turn player ─────────────
Move Bot::MinMax(bool whites_turn, int depth, int alpha, int beta)
{
	Move bestMove;
	bool maximizing = whites_turn;
	int  best       = maximizing ? INT_MIN : INT_MAX;
	bool cutoff     = false;

	for (int sy = 0; sy < BoardSize && !cutoff; sy++)
	{
		for (int sx = 0; sx < BoardSize && !cutoff; sx++)
		{
			char src_t = tempGM.board[sy][sx]->getType();
			if (src_t == '#') continue;
			if ( whites_turn && !isupper(src_t)) continue;
			if (!whites_turn && !islower(src_t)) continue;

			for (int dy = 0; dy < BoardSize && !cutoff; dy++)
			{
				for (int dx = 0; dx < BoardSize && !cutoff; dx++)
				{
					int move_t = tempGM.board[sy][sx]->movment(tempGM.board, sx, sy, dx, dy);
					if (!move_t) continue;

					Move m;
					m.src_squere = { sy, sx };
					m.dst_squere = { dy, dx };
					m.move_type  = move_t;

					MoveUndo undo;
					apply_move(m, undo);
					tempGM.quick_kings_update();

					if (!tempGM.checkIfCheck(whites_turn))
					{
						int score = minimax_score(!whites_turn, depth - 1, alpha, beta);

						if ((maximizing && score > best) || (!maximizing && score < best))
						{
							best     = score;
							bestMove = m;
						}
						if (maximizing && score > alpha) alpha = score;
						if (!maximizing && score < beta)  beta  = score;
						if (alpha >= beta) cutoff = true;
					}

					undo_move(m, undo);
					tempGM.quick_kings_update();
				}
			}
		}
	}

	return bestMove;
}

// ── existing functions below (unchanged) ─────────────────────────────────────

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
	deepCopyBoard(board);
	tempGM.setTurn(whites_turn);
	tempGM.quick_kings_update();
	this->tempWhitesTurn = whites_turn;
	this->chosenMove = MinMax(whites_turn, SEARCH_DEPTH, INT_MIN, INT_MAX);
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
