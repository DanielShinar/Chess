#include "GameMaster.h"

void GameMaster::re_size(HWND hwnd)
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

void GameMaster::run_game(HWND window)
{
	Piece* holder;
	if (playerBlack)
	{
		holder = this->board[0][4];
		this->board[0][4] = this->board[0][3];
		this->board[0][3] = holder;
		this->board[0][3]->setLocation(0, 3);
		this->board[0][4]->setLocation(0, 4);

		holder = this->board[7][4];
		this->board[7][4] = this->board[7][3];
		this->board[7][3] = holder;
		this->board[7][3]->setLocation(7, 3);
		this->board[7][4]->setLocation(7, 4);

		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (isupper(this->board[y][x]->getType()))
				{
					this->board[y][x]->setType(this->board[y][x]->getType() + 0x20);
				}
				else if (islower(this->board[y][x]->getType()))
				{
					this->board[y][x]->setType(this->board[y][x]->getType() - 0x20);
				}
			}
		}
		this->whiteKing.y = 0;
		this->whiteKing.x = 3;
		this->blackKing.y = 7;
		this->blackKing.x = 3;
	}
	else
	{
		this->whiteKing.y = 7;
		this->whiteKing.x = 4;
		this->blackKing.y = 0;
		this->blackKing.x = 4;
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
			delete this->board[y][x];
		}
	}

}

void GameMaster::play_turn(int move, Squere pre_squere, Squere squere)
{
	int playerPlayed = 0;
	int aiMove = illegal;
	Move am;
	playerPlayed = play_move(move, pre_squere, squere);
	if (!playerPlayed)
		return;
	else if (whoWon)
		return;
	updatePiecesMoves();
	AI.get_pieces(this->board);
	AI.choose_move(board, whitesTurn);
	am = AI.get_move();
	//am = AI.get_random_move(this->board);//problam with not random
	play_move(am.move_type, am.src_squere, am.dst_squere);
}

int GameMaster::play_move(int move, Squere pre_squere, Squere squere)
{
	int playerMove = move;
	Squere cleaning = pre_squere;
	Piece* pieceHolder;
	Piece* checkHolder;
	if ((move > 0 && (isupper(this->board[pre_squere.y][pre_squere.x]->getType()) != isupper(this->board[squere.y][squere.x]->getType()) || islower(this->board[pre_squere.y][pre_squere.x]->getType()) != islower(this->board[squere.y][squere.x]->getType()))) || move == castle)
	{
		if (((this->board[pre_squere.y][pre_squere.x]->getType() == 'K' || this->board[pre_squere.y][pre_squere.x]->getType() == 'k' || this->board[pre_squere.y][pre_squere.x]->getType() == 'r' || this->board[pre_squere.y][pre_squere.x]->getType() == 'R') && !this->board[pre_squere.y][pre_squere.x]->getMoved()) && move != castle)
		{
			this->board[pre_squere.y][pre_squere.x]->setMoved(true);
		}
		if (move == castle)
		{
			if (!checkIfCheck(whitesTurn))
			{
				if (squere.x < pre_squere.x)
				{
					if (!canCastle(true))
					{
						move = illegal;
					}
					else
					{
						this->board[pre_squere.y][pre_squere.x]->setMoved(true);
						this->board[squere.y][squere.x]->setMoved(true);

						delete this->board[pre_squere.y][pre_squere.x - 2];
						this->board[pre_squere.y][pre_squere.x - 2] = this->board[pre_squere.y][pre_squere.x];
						this->board[pre_squere.y][pre_squere.x] = new Piece();

						delete this->board[pre_squere.y][pre_squere.x - 1];
						this->board[pre_squere.y][pre_squere.x - 1] = this->board[squere.y][squere.x];
						this->board[squere.y][squere.x] = new Piece();
					}
				}
				else
				{
					if (!canCastle(false))
					{
						move = illegal;
					}
					else
					{
						this->board[pre_squere.y][pre_squere.x]->setMoved(true);
						this->board[squere.y][squere.x]->setMoved(true);

						delete this->board[pre_squere.y][pre_squere.x + 2];
						this->board[pre_squere.y][pre_squere.x + 2] = this->board[pre_squere.y][pre_squere.x];
						this->board[pre_squere.y][pre_squere.x] = new Piece();

						delete this->board[pre_squere.y][pre_squere.x + 1];
						this->board[pre_squere.y][pre_squere.x + 1] = this->board[squere.y][squere.x];
						this->board[squere.y][squere.x] = new Piece();
					}
				}
				updateKingLocation(move, pre_squere, squere);
			}
		}
		else
		{
			if (move == en)
			{
				if (playerBlack && !whitesTurn)
				{
					delete board[squere.y + 1][squere.x];
					board[squere.y + 1][squere.x] = new Piece();
				}
				else if (!playerBlack && whitesTurn)
				{
					delete board[squere.y + 1][squere.x];
					board[squere.y + 1][squere.x] = new Piece();
				}
				else if (playerBlack && whitesTurn)
				{
					delete board[squere.y - 1][squere.x];
					board[squere.y - 1][squere.x] = new Piece();
				}
				else if (!playerBlack && !whitesTurn)
				{
					delete board[squere.y - 1][squere.x];
					board[squere.y - 1][squere.x] = new Piece();
				}
			}
			if (this->board[pre_squere.y][pre_squere.x]->getType() == 'K' || this->board[pre_squere.y][pre_squere.x]->getType() == 'k')
			{
				updateKingLocation(move, pre_squere, squere);
			}
			pieceHolder = this->board[pre_squere.y][pre_squere.x];
			checkHolder = this->board[squere.y][squere.x];
			this->board[pre_squere.y][pre_squere.x] = new Piece();
			this->board[squere.y][squere.x] = pieceHolder;
			if (checkIfCheck(whitesTurn))
			{
				quick_kings_update();
				move = illegal;
			}
			delete this->board[pre_squere.y][pre_squere.x];
			this->board[pre_squere.y][pre_squere.x] = pieceHolder;
			this->board[squere.y][squere.x] = checkHolder;

			if (move != illegal)
			{
				pieceHolder = this->board[pre_squere.y][pre_squere.x];
				pieceHolder->setLocation(squere.y, squere.x);

				delete this->board[squere.y][squere.x];

				this->board[pre_squere.y][pre_squere.x] = new Piece();
				pieceHolder = pawnChange(pieceHolder, squere.y);
				this->board[squere.y][squere.x] = pieceHolder;
			}
		}

		if (move != illegal)
		{
			newTurn = true;
		}

		clear_my_screen();
		updateSpesificSquers(move, pre_squere, squere);
		drawPiecesDiff();

		if (checkIfCheck(!this->whitesTurn))
		{
			if (checkMate(pre_squere, squere))
			{
				if (whitesTurn)
					whoWon = 1;
				else
					whoWon = 2;
				running = false;
			}
		}

		if (whitesTurn && newTurn)
			whitesTurn = false;
		else if (!whitesTurn && newTurn)
			whitesTurn = true;

		if (playerMove != pawnPush)
		{
			this->clearEn();
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

int GameMaster::do_move(int move, Squere pre_squere, Squere squere)
{
	return 0;
}

bool GameMaster::first_check(Squere squere)
{
	return ((isupper(this->board[squere.y][squere.x]->getType()) && whitesTurn && newTurn) || (islower(this->board[squere.y][squere.x]->getType()) && !whitesTurn && newTurn) || !newTurn);
}

int GameMaster::get_player_move(Squere s, Squere pre_s)
{
	quick_kings_update();
	int move = this->board[pre_s.y][pre_s.x]->movment(this->board, pre_s.x, pre_s.y, s.x, s.y);
	if (move == castle)
	{
		if (checkIfCheck(whitesTurn))
		{
			return illegal;
		}
	}
		
	return move;
}

void GameMaster::drawPiecesDiff()
{
	char type;
	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			type = this->board[y][x]->getType();
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

		deafult:
			break;
			}

			/*	if (chessBoard[y][x] != '#')
				{
					DrawBitmap(white_p, src + (renderState.Width / size * x), src + (renderState.Height / size * y), pub);
				}*/
		}
	}
}

void GameMaster::DrawBitmap(LPCSTR piecePath, int x, int y, HDC dc)
{
	//load the bitmap image  "C:\\Users\\USER\\Desktop\\white-Pawn.bmp"
	HBITMAP image = (HBITMAP)LoadImage(0, piecePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	BITMAP bm;
	GetObject(image, sizeof(BITMAP), &bm);

	HDC hdcImage = CreateCompatibleDC(dc);
	SelectObject(hdcImage, image);

	BitBlt(dc, x, y, bm.bmWidth, bm.bmHeight, hdcImage, 0, 0, SRCCOPY);
}

void GameMaster::clear_my_screen()
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

void GameMaster::setMovmentRenderState()
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

void GameMaster::drew_piece_move(Squere sq)
{

	for (auto it = this->board[sq.y][sq.x]->avalableMoves.begin(); it != this->board[sq.y][sq.x]->avalableMoves.end(); it++)
	{
		StretchDIBits(pub, it->dst_squere.x * this->renderState.Width / 8 + 25, it->dst_squere.y * this->renderState.Height / 8 + 50, this->renderMovment.Width, this->renderMovment.Height / 8, 0, 0, this->renderMovment.Width, this->renderMovment.Height, this->renderMovment.Memory, &this->renderMovment.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
	//need to do it to spesific squeres
	/*StretchDIBits(pub, 0, 0, this->renderState.Width, this->renderState.Height, 0, 0, this->renderState.Width, this->renderState.Height, this->renderState.Memory, &this->renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	this->drawPiecesDiff();*/
}

void GameMaster::clear_piece_move(Squere sq)
{
	for (auto it = this->board[sq.y][sq.x]->avalableMoves.begin(); it != this->board[sq.y][sq.x]->avalableMoves.end(); it++)
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
				if (islower(this->board[y][x]->getType()) && this->board[y][x]->movment(this->board, x, y, this->whiteKing.x, this->whiteKing.y))
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
				if (isupper(this->board[y][x]->getType()) && this->board[y][x]->movment(this->board, x, y, this->blackKing.x, this->blackKing.y))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void GameMaster::updateSpesificSquers(int move, Squere pre_squere, Squere this_squere)
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

void GameMaster::updateKingLocation(int move, Squere pre_squere, Squere this_squere)
{
	if (move == castle)
	{
		for (int y = 0; y < BoardSize; y++)//keep the location of the kings and rooks when castling
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (this->board[y][x]->getType() == 'k')
				{
					this->blackKing.y = y;
					this->blackKing.x = x;
					this->board[y][x]->setLocation(y, x);
				}
				else if (this->board[y][x]->getType() == 'K')
				{
					this->whiteKing.y = y;
					this->whiteKing.x = x;
					this->board[y][x]->setLocation(y, x);
				}
				else if (this->board[y][x]->getType() == 'r')
				{
					this->board[y][x]->setLocation(y, x);
				}
				else if (this->board[y][x]->getType() == 'R')
				{
					this->board[y][x]->setLocation(y, x);
				}
			}
		}
	}
	else
	{
		if (this->board[pre_squere.y][pre_squere.x]->getType() == 'k')//always keeps where the kings are
		{
			this->blackKing.y = this_squere.y;
			this->blackKing.x = this_squere.x;
		}
		else if (this->board[pre_squere.y][pre_squere.x]->getType() == 'K')
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

	if (!this->whitesTurn)
	{
		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (isupper(this->board[y][x]->getType()))
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
									if (!checkIfCheck(!this->whitesTurn))
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
				if (islower(this->board[y][x]->getType()))
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
									if (!checkIfCheck(!this->whitesTurn))
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
			piece = this->board[src_y][src_x];
			this->board[src_y][src_x]->avalableMoves.clear();
			for (int y = 0; y < BoardSize; y++)
			{
				for (int x = 0; x < BoardSize; x++)
				{
					sq.x = x;
					sq.y = y;
					moveGood = piece->movment(this->board, srcSq.x, srcSq.y, x, y);
					m.move_type = moveGood;
					if (moveGood)
					{
						if (this->board[srcSq.y][srcSq.x]->getType() == 'K' || this->board[srcSq.y][srcSq.x]->getType() == 'k')
						{
							updateKingLocation(moveGood, srcSq, sq);
						}

						firstHolder = this->board[srcSq.y][srcSq.x];
						secondHolder = this->board[y][x];
						this->board[srcSq.y][srcSq.x] = new Piece();
						this->board[y][x] = firstHolder;

						if (checkIfCheck(whitesTurn))
						{
							updateKingLocation(moveGood, sq, srcSq);
							moveGood = illegal;
						}
						delete this->board[srcSq.y][srcSq.x];
						this->board[srcSq.y][srcSq.x] = firstHolder;
						this->board[y][x] = secondHolder;
						if (moveGood)
						{
							m.src_squere = srcSq;
							m.dst_squere = sq;
							this->board[srcSq.y][srcSq.x]->avalableMoves.push_back(m);
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

void GameMaster::setCheck(bool check_)
{
	this->check = check_;
}

void GameMaster::setRunning(bool running_)
{
	this->running = running_;
}

void GameMaster::setNewTurn(bool newTurn_)
{
	this->newTurn = newTurn_;
}

bool GameMaster::getNewTurn()
{
	return this->newTurn;
}

bool GameMaster::getCheck()
{
	return this->check;
}

RenderState GameMaster::getRenderState()
{
	return this->renderState;
}
