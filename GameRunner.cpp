#include "GameRunner.h"
void GameRunner::re_size(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	this->renderState.Width = rect.right - rect.left;
	this->renderState.Height = rect.bottom - rect.top;

	int bufferSize = this->renderState.Width * this->renderState.Height * sizeof(u32);

	if (this->renderState.Memory)
		VirtualFree(this->renderState.Memory, 0, MEM_RELEASE);

	this->renderState.Memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	this->renderState.BitMapInfo.bmiHeader.biSize = sizeof(this->renderState.BitMapInfo.bmiHeader);
	this->renderState.BitMapInfo.bmiHeader.biWidth = this->renderState.Width;
	this->renderState.BitMapInfo.bmiHeader.biHeight = this->renderState.Height;
	this->renderState.BitMapInfo.bmiHeader.biPlanes = 1;
	this->renderState.BitMapInfo.bmiHeader.biBitCount = 32;
	this->renderState.BitMapInfo.bmiHeader.biCompression = BI_RGB;

	//movment 
	this->renderMovment.Width = 50;
	this->renderMovment.Height = 50;

	int buffer_size = this->renderMovment.Width * this->renderMovment.Height * sizeof(u32);

	if (this->renderMovment.Memory)
		VirtualFree(this->renderMovment.Memory, 0, MEM_RELEASE);

	this->renderMovment.Memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	this->renderMovment.BitMapInfo.bmiHeader.biSize = sizeof(this->renderMovment.BitMapInfo.bmiHeader);
	this->renderMovment.BitMapInfo.bmiHeader.biWidth = this->renderMovment.Width;
	this->renderMovment.BitMapInfo.bmiHeader.biHeight = this->renderMovment.Height;
	this->renderMovment.BitMapInfo.bmiHeader.biPlanes = 1;
	this->renderMovment.BitMapInfo.bmiHeader.biBitCount = 32;
	this->renderMovment.BitMapInfo.bmiHeader.biCompression = BI_RGB;
}

void GameRunner::run_game(HWND window)
{
	Piece* holder;
	if (playerBlack)
	{
		holder = gm.board[0][4];
		gm.board[0][4] = gm.board[0][3];
		gm.board[0][3] = holder;
		gm.board[0][3]->setLocation(0, 3);
		gm.board[0][4]->setLocation(0, 4);

		holder = gm.board[7][4];
		gm.board[7][4] = gm.board[7][3];
		gm.board[7][3] = holder;
		gm.board[7][3]->setLocation(7, 3);
		gm.board[7][4]->setLocation(7, 4);

		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (isupper(gm.board[y][x]->getType()))
				{
					gm.board[y][x]->setType(gm.board[y][x]->getType() + 0x20);
				}
				else if (islower(gm.board[y][x]->getType()))
				{
					gm.board[y][x]->setType(gm.board[y][x]->getType() - 0x20);
				}
			}
		}
		gm.setBlackKing(7, 3);
		gm.setWhiteKing(0, 3);
	}
	else
	{
		gm.setBlackKing(0, 4);
		gm.setWhiteKing(7, 4);
	}


	this->check = true; //make sure i can check pixel colors when clicks
	HDC hdc = GetDC(window);
	pub = hdc;

	setMovmentRenderState();
	clear_my_screen();

	StretchDIBits(hdc, 0, 0, this->renderState.Width, this->renderState.Height, 0, 0, this->renderState.Width, this->renderState.Height, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	drawPiecesDiff();

	while (running)
	{
		// input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// simulate
		//clear_screen(0xeeeed2, 0x769656);
		//drawPieces();




		// rander
		//StretchDIBits(hdc, 0, 0, renderState.Width, renderState.Height, 0, 0, renderState.Width, renderState.Height, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);

	}
	Sleep(500);
	announceWinner(whoWon, this->renderState);
	StretchDIBits(hdc, 0, 0, this->renderState.Width, this->renderState.Height, 0, 0, this->renderState.Width, this->renderState.Height, this->renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	Sleep(1000);
	VirtualFree(this->renderState.Memory, 0, MEM_RELEASE);

	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			delete gm.board[y][x];
		}
	}

}

void GameRunner::play_turn(int move, Squere pre_squere, Squere squere)
{
	int playerPlayed = 0;
	int aiMove = illegal;
	Move am;
	playerPlayed = play_move(move, pre_squere, squere);
	if (!playerPlayed)
		return;
	else if (whoWon)
		return;
	gm.updatePiecesMoves();
	AI.get_pieces(gm.board);
	AI.choose_move(gm.board, gm.getWhitesTurn());
	am = AI.get_move();
	//am = AI.get_random_move(this->board);//problam with not random
	play_move(am.move_type, am.src_squere, am.dst_squere);
}

int GameRunner::play_move(int move, Squere pre_squere, Squere squere)
{
	int playerMove = move;
	Squere cleaning = pre_squere;
	Piece* pieceHolder;
	Piece* checkHolder;
	if ((move > 0 && (isupper(gm.board[pre_squere.y][pre_squere.x]->getType()) != isupper(gm.board[squere.y][squere.x]->getType()) || islower(gm.board[pre_squere.y][pre_squere.x]->getType()) != islower(gm.board[squere.y][squere.x]->getType()))) || move == castle)
	{
		if (((gm.board[pre_squere.y][pre_squere.x]->getType() == 'K' || gm.board[pre_squere.y][pre_squere.x]->getType() == 'k' || gm.board[pre_squere.y][pre_squere.x]->getType() == 'r' || gm.board[pre_squere.y][pre_squere.x]->getType() == 'R') && !gm.board[pre_squere.y][pre_squere.x]->getMoved()) && move != castle)
		{
			gm.board[pre_squere.y][pre_squere.x]->setMoved(true);
		}
		if (move == castle)
		{
			if (!gm.checkIfCheck(gm.getWhitesTurn()))
			{
				if (squere.x < pre_squere.x)
				{
					if (!gm.canCastle(true))
					{
						move = illegal;
					}
					else
					{
						gm.board[pre_squere.y][pre_squere.x]->setMoved(true);
						gm.board[squere.y][squere.x]->setMoved(true);

						delete gm.board[pre_squere.y][pre_squere.x - 2];
						gm.board[pre_squere.y][pre_squere.x - 2] = gm.board[pre_squere.y][pre_squere.x];
						gm.board[pre_squere.y][pre_squere.x] = new Piece();

						delete gm.board[pre_squere.y][pre_squere.x - 1];
						gm.board[pre_squere.y][pre_squere.x - 1] = gm.board[squere.y][squere.x];
						gm.board[squere.y][squere.x] = new Piece();
					}
				}
				else
				{
					if (!gm.canCastle(false))
					{
						move = illegal;
					}
					else
					{
						gm.board[pre_squere.y][pre_squere.x]->setMoved(true);
						gm.board[squere.y][squere.x]->setMoved(true);

						delete gm.board[pre_squere.y][pre_squere.x + 2];
						gm.board[pre_squere.y][pre_squere.x + 2] = gm.board[pre_squere.y][pre_squere.x];
						gm.board[pre_squere.y][pre_squere.x] = new Piece();

						delete gm.board[pre_squere.y][pre_squere.x + 1];
						gm.board[pre_squere.y][pre_squere.x + 1] = gm.board[squere.y][squere.x];
						gm.board[squere.y][squere.x] = new Piece();
					}
				}
				gm.updateKingLocation(move, pre_squere, squere);
			}
		}
		else
		{
			if (move == pawnPush)
				gm.board[pre_squere.y][pre_squere.x]->setEn(true);
			if (move == en)
			{
				if (playerBlack && !gm.getWhitesTurn())
				{
					delete gm.board[squere.y + 1][squere.x];
					gm.board[squere.y + 1][squere.x] = new Piece();
				}
				else if (!playerBlack && gm.getWhitesTurn())
				{
					delete gm.board[squere.y + 1][squere.x];
					gm.board[squere.y + 1][squere.x] = new Piece();
				}
				else if (playerBlack && gm.getWhitesTurn())
				{
					delete gm.board[squere.y - 1][squere.x];
					gm.board[squere.y - 1][squere.x] = new Piece();
				}
				else if (!playerBlack && !gm.getWhitesTurn())
				{
					delete gm.board[squere.y - 1][squere.x];
					gm.board[squere.y - 1][squere.x] = new Piece();
				}
			}
			if (gm.board[pre_squere.y][pre_squere.x]->getType() == 'K' || gm.board[pre_squere.y][pre_squere.x]->getType() == 'k')
			{
				gm.updateKingLocation(move, pre_squere, squere);
			}
			pieceHolder = gm.board[pre_squere.y][pre_squere.x];
			checkHolder = gm.board[squere.y][squere.x];
			gm.board[pre_squere.y][pre_squere.x] = new Piece();
			gm.board[squere.y][squere.x] = pieceHolder;
			if (gm.checkIfCheck(gm.getWhitesTurn()))
			{
				delete gm.board[pre_squere.y][pre_squere.x];
				gm.board[pre_squere.y][pre_squere.x] = pieceHolder;
				gm.board[squere.y][squere.x] = checkHolder;
				gm.quick_kings_update();

				newTurn = true;
				clear_piece_move(cleaning);
				drawPiecesDiff();
				return 0;

				move = illegal;
			}
			delete gm.board[pre_squere.y][pre_squere.x];
			gm.board[pre_squere.y][pre_squere.x] = pieceHolder;
			gm.board[squere.y][squere.x] = checkHolder;
			gm.quick_kings_update();

			if (move != illegal)
			{
				pieceHolder = gm.board[pre_squere.y][pre_squere.x];
				pieceHolder->setLocation(squere.y, squere.x);

				delete gm.board[squere.y][squere.x];

				gm.board[pre_squere.y][pre_squere.x] = new Piece();
				pieceHolder = gm.pawnChange(pieceHolder, squere.y);
				gm.board[squere.y][squere.x] = pieceHolder;
			}
		}

		if (move != illegal)
		{
			newTurn = true;
		}

		clear_my_screen();
		updateSpesificSquers(move, pre_squere, squere);
		drawPiecesDiff();

		if (gm.checkIfCheck(!gm.getWhitesTurn()))
		{
			if (gm.checkMate(pre_squere, squere))
			{
				if (gm.getWhitesTurn())
					whoWon = 1;
				else
					whoWon = 2;
				running = false;
			}
		}

		if (gm.getWhitesTurn() && newTurn)
			gm.setTurn(false);
		else if (!gm.getWhitesTurn() && newTurn)
			gm.setTurn(true);

		if (playerMove != pawnPush)
		{
			gm.clearEn();
		}
	}
	else
	{
		newTurn = true;
		clear_piece_move(cleaning);
		drawPiecesDiff();
		return 0;
	}
	if (playerMove == castle)
		StretchDIBits(pub, 0, 0, this->renderState.Width, this->renderState.Height, 0, 0, this->renderState.Width, this->renderState.Height, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	//needs to be changed having problam with castleing fix
	else
	{
		clear_piece_move(squere);
	}
	drawPiecesDiff();
	return 1;
}

int GameRunner::do_move(int move, Squere pre_squere, Squere squere)
{
	return 0;
}

bool GameRunner::first_check(Squere squere)
{
	if (((isupper(gm.board[squere.y][squere.x]->getType()) && gm.getWhitesTurn() && newTurn) || (islower(gm.board[squere.y][squere.x]->getType()) && !gm.getWhitesTurn() && newTurn) || !newTurn))
	{
		gm.updatePiecesMoves();
		return true;
	}
	return false;
}

int GameRunner::get_player_move(Squere s, Squere pre_s)
{
	gm.quick_kings_update();
	int move = gm.board[pre_s.y][pre_s.x]->movment(gm.board, pre_s.x, pre_s.y, s.x, s.y);
	if (move == castle)
	{
		if (gm.checkIfCheck(gm.getWhitesTurn()))
		{
			return illegal;
		}
	}

	return move;
}

void GameRunner::drawPiecesDiff()
{
	char type;
	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			type = gm.board[y][x]->getType();
			switch (type)
			{
			case 'p': //black pawn
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_p_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_p_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'P': //white pawn
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_p_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_p_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'o': //black knight
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_o_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_o_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'O': //white knight
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_o_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_o_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'b': //black bishop
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_b_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_b_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'B': //white bishop
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_b_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_b_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'r': //black rook
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_r_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_r_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'R': //white rook
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_r_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_r_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'q': //black queen
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_q_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_q_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'Q': //white queen
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_q_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_q_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'k': //black king
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_k_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_k_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'K': //white king
			{
				if ((y + x) % 2 == 0)
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_k_w.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
				else
					this->DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_k_g.bmp", src / 2 + (this->renderState.Width / BoardSize * x), (this->renderState.Height / BoardSize * y) + 15, pub);
			}break;

			default:
				break;
			}

			/*	if (chessBoard[y][x] != '#')
				{
					DrawBitmap(white_p, src + (renderState.Width / size * x), src + (renderState.Height / size * y), pub);
				}*/
		}
	}
}

void GameRunner::DrawBitmap(LPCSTR piecePath, int x, int y, HDC dc)
{
	//load the bitmap image  "C:\\Users\\USER\\Desktop\\white-Pawn.bmp"
	HBITMAP image = (HBITMAP)LoadImage(0, piecePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	BITMAP bm;
	GetObject(image, sizeof(BITMAP), &bm);

	HDC hdcImage = CreateCompatibleDC(dc);
	SelectObject(hdcImage, image);

	BitBlt(dc, x, y, bm.bmWidth, bm.bmHeight, hdcImage, 0, 0, SRCCOPY);
}

void GameRunner::clear_my_screen()
{
	u32 colorA = 0xffffcc;
	u32 colorB = 0x00bb00;
	u32 color = colorB;
	u32 colorTwo = colorB;
	u32* pixel = (u32*)this->renderState.Memory;
	for (int y = 0; y < this->renderState.Height; y++)
	{
		for (int x = 0; x < this->renderState.Width; x++)
		{
			*pixel++ = color;
			if ((x + 1) % (this->renderState.Width / 8) == 0)
			{
				if (color == colorA)
					color = colorB;
				else
					color = colorA;
			}
		}
		if ((y + 1) % (this->renderState.Height / 8) == 0)
		{
			if (colorTwo == colorA)
				colorTwo = colorB;
			else
				colorTwo = colorA;
		}
		color = colorTwo;
	}
}

void GameRunner::setMovmentRenderState()
{
	u32 color = 0xf54242;
	u32* pixel = (u32*)this->renderMovment.Memory;
	for (int y = 0; y < this->renderMovment.Height; y++)
	{
		for (int x = 0; x < this->renderMovment.Width; x++)
		{
			*pixel++ = color;
		}
	}
}

void GameRunner::drew_piece_move(Squere sq)
{

	for (auto it = gm.board[sq.y][sq.x]->avalableMoves.begin(); it != gm.board[sq.y][sq.x]->avalableMoves.end(); it++)
	{
		StretchDIBits(pub, it->dst_squere.x * this->renderState.Width / 8 + 25, it->dst_squere.y * this->renderState.Height / 8 + 50, this->renderMovment.Width, this->renderMovment.Height / 8, 0, 0, this->renderMovment.Width, this->renderMovment.Height, this->renderMovment.Memory, &this->renderMovment.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
	//need to do it to spesific squeres
	/*StretchDIBits(pub, 0, 0, this->renderState.Width, this->renderState.Height, 0, 0, this->renderState.Width, this->renderState.Height, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	this->drawPiecesDiff();*/
}

void GameRunner::clear_piece_move(Squere sq)
{
	for (auto it = gm.board[sq.y][sq.x]->avalableMoves.begin(); it != gm.board[sq.y][sq.x]->avalableMoves.end(); it++)
	{
		if ((it->dst_squere.y + it->dst_squere.x) % 2 == 0)
		{
			StretchDIBits(pub, it->dst_squere.x * this->renderState.Width / 8, it->dst_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 106, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		else
		{
			StretchDIBits(pub, it->dst_squere.x * this->renderState.Width / 8, it->dst_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 1, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
	}
}

void GameRunner::setCheck(bool check_)
{
	this->check = check_;
}

void GameRunner::setRunning(bool running_)
{
	this->running = running_;
}

void GameRunner::setNewTurn(bool newTurn_)
{
	this->newTurn = newTurn_;
}

bool GameRunner::getNewTurn()
{
	return this->newTurn;
}

bool GameRunner::getCheck()
{
	return this->check;
}

RenderState GameRunner::getRenderState()
{
	return this->renderState;
}
void GameRunner::updateSpesificSquers(int move, Squere pre_squere, Squere this_squere)
{
	if ((pre_squere.y + pre_squere.x) % 2 == 0)
	{
		StretchDIBits(pub, pre_squere.x * this->renderState.Width / 8, pre_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 106, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		if (move == en)
		{
			StretchDIBits(pub, (pre_squere.x - 1) * this->renderState.Width / 8, pre_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 1, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
			StretchDIBits(pub, (pre_squere.x + 1) * this->renderState.Width / 8, pre_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 1, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		else if (move == castle)
		{
			if (this_squere.x < pre_squere.x)
				StretchDIBits(pub, this_squere.x * this->renderState.Width / 8, this_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 106, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
			else
				StretchDIBits(pub, this_squere.x * this->renderState.Width / 8, this_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 1, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
	}
	else
	{
		StretchDIBits(pub, pre_squere.x * this->renderState.Width / 8, pre_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 1, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		if (move == en)
		{
			StretchDIBits(pub, (pre_squere.x - 1) * this->renderState.Width / 8, pre_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 106, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
			StretchDIBits(pub, (pre_squere.x + 1) * this->renderState.Width / 8, pre_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 106, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		else if (move == castle)
		{
			if (this_squere.x < pre_squere.x)
				StretchDIBits(pub, this_squere.x * this->renderState.Width / 8, this_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 1, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
			else
				StretchDIBits(pub, this_squere.x * this->renderState.Width / 8, this_squere.y * this->renderState.Height / 8, this->renderState.Width / 8, this->renderState.Height / 8, 0, 106, this->renderState.Width / 8, this->renderState.Height / 8, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
	}
}