#include "utility.h"

#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "Pawn.h"

#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

#define dst 80
#define src 30

POINT pt;

Squere blackKing;
Squere whiteKing;
Squere tempKing;

int whoWon = 0;

bool whitesTurn = true;
bool check = false;
bool newTurn = true;
bool running = true;
RenderState renderState;
Squere preSquere;
Squere squere;
globalVariable HDC pub;
globalVariable HBITMAP hBitmap;

//char chessBoard[size][size] = {
//	{'r','o','b','q','k','b','o','r'},
//	{'p','p','p','p','p','p','p','p'},
//	{'#','#','#','#','#','#','#','#'},
//	{'#','#','#','#','#','#','#','#'},
//	{'#','#','#','#','#','#','#','#'},
//	{'#','#','#','#','#','#','#','#'},
//	{'P','P','P','P','P','P','P','P'},
//	{'R','O','B','Q','K','B','O','R'} }; //upper case is white lower case is black
//
//globalVariable char chessBoardBlack[size][size] = {
//	{'R','O','B','K','Q','B','O','R'},
//	{'P','P','P','P','P','P','P','P'},
//	{'#','#','#','#','#','#','#','#'},
//	{'#','#','#','#','#','#','#','#'},
//	{'#','#','#','#','#','#','#','#'},
//	{'#','#','#','#','#','#','#','#'},
//	{'p','p','p','p','p','p','p','p'},
//	{'r','o','b','k','q','b','o','r'} };

Piece* chessBoard[BoardSize][BoardSize] = {
	{new Rook(0,0,'r'),new Knight(0,1,'o'),new Bishop(0,2,'b'),new Queen(0,3,'q'),new King(0,4,'k'),new Bishop(0,5,'b'),new Knight(0,6,'o'),new Rook(0,7,'r')},
	{new Pawn(1,0,'p'),new Pawn(1,1,'p'),new Pawn(1,2,'p'),new Pawn(1,3,'p'),new Pawn(1,4,'p'),new Pawn(1,5,'p'),new Pawn(1,6,'p'),new Pawn(1,7,'p')},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece(),new Piece()},
	{new Pawn(6,0,'P'),new Pawn(6,1,'P'),new Pawn(6,2,'P'),new Pawn(6,3,'P'),new Pawn(6,4,'P'),new Pawn(6,5,'P'),new Pawn(6,6,'P'),new Pawn(6,7,'P')},
	{new Rook(7,0,'R'),new Knight(7,1,'O'),new Bishop(7,2,'B'),new Queen(7,3,'Q'),new King(7,4,'K'),new Bishop(7,5,'B'),new Knight(7,6,'O'),new Rook(7,7,'R')} };


void drawPiecesDiff();
void DrawBitmap(LPCSTR piecePath, int x, int y, HDC dc);
Piece* pawnChange(Piece* piece, int y);
bool checkIfCheck(Piece* board[][BoardSize], bool whiteAttacked);
void updateSpesificSquers(int move, Squere pre_squere, Squere this_squere);
void updateKingLocation(int move, Squere pre_squere, Squere this_squere);
bool canCastle(Piece* board[][BoardSize], bool whitesTurn, bool left);
bool checkMate(Piece* board[][BoardSize], bool whites_turn, Squere src_s, Squere dst_s);

#include "renderer.cpp"


LRESULT CALLBACK windowCallBack(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg)
	{
		int wmId, wmEvent;
		case WM_CLOSE:
		case WM_DESTROY:
		{
			running = false;
		} break;
		case WM_ERASEBKGND:
		{
			return true;
		}
		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			renderState.Width = rect.right - rect.left;
			renderState.Height = rect.bottom - rect.top;

			int bufferSize = renderState.Width * renderState.Height * sizeof(u32);

			if (renderState.Memory)
				VirtualFree(renderState.Memory, 0, MEM_RELEASE);

			renderState.Memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			renderState.BitMapInfo.bmiHeader.biSize = sizeof(renderState.BitMapInfo.bmiHeader);
			renderState.BitMapInfo.bmiHeader.biWidth = renderState.Width;
			renderState.BitMapInfo.bmiHeader.biHeight = renderState.Height;
			renderState.BitMapInfo.bmiHeader.biPlanes = 1;
			renderState.BitMapInfo.bmiHeader.biBitCount = 32;
			renderState.BitMapInfo.bmiHeader.biCompression = BI_RGB;
		}
		case WM_LBUTTONDOWN:
		{
			Piece* pieceHolder;
			Piece* checkHolder;
			int move = illegal;
			if (check)
			{
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				squere.x = pt.x / (renderState.Width / 8);
				squere.y = pt.y / (renderState.Height / 8);
				if (((isupper(chessBoard[squere.y][squere.x]->getType()) && whitesTurn && newTurn) || (islower(chessBoard[squere.y][squere.x]->getType()) && !whitesTurn && newTurn) || !newTurn))//checks if the player picks a piece to play
				{
					if (newTurn)
					{
						preSquere.x = squere.x;
						preSquere.y = squere.y;
						newTurn = false;
					}
					else
					{
						move = chessBoard[preSquere.y][preSquere.x]->movment(chessBoard, preSquere.x, preSquere.y, squere.x, squere.y);
						if ((move > 0 && (isupper(chessBoard[preSquere.y][preSquere.x]->getType()) != isupper(chessBoard[squere.y][squere.x]->getType()) || islower(chessBoard[preSquere.y][preSquere.x]->getType()) != islower(chessBoard[squere.y][squere.x]->getType()))) || move == castle)
						{
							if (((chessBoard[preSquere.y][preSquere.x]->getType() == 'K' || chessBoard[preSquere.y][preSquere.x]->getType() == 'k' || chessBoard[preSquere.y][preSquere.x]->getType() == 'r' || chessBoard[preSquere.y][preSquere.x]->getType() == 'R') && !chessBoard[preSquere.y][preSquere.x]->getMoved()) && move != castle)
							{
								chessBoard[preSquere.y][preSquere.x]->setMoved(true);
							}
							if (move == castle)
							{
								if (squere.x < preSquere.x)
								{
									if (!canCastle(chessBoard, whitesTurn, true))
									{
										break;
									}
									chessBoard[preSquere.y][preSquere.x]->setMoved(true);
									chessBoard[squere.y][squere.x]->setMoved(true);

									delete chessBoard[preSquere.y][preSquere.x - 2];
									chessBoard[preSquere.y][preSquere.x - 2] = chessBoard[preSquere.y][preSquere.x];
									chessBoard[preSquere.y][preSquere.x] = new Piece();

									delete chessBoard[preSquere.y][preSquere.x - 1];
									chessBoard[preSquere.y][preSquere.x - 1] = chessBoard[squere.y][squere.x];
									chessBoard[squere.y][squere.x] = new Piece();
								}
								else
								{
									if (!canCastle(chessBoard, whitesTurn, false))
									{
										break;
									}
									chessBoard[preSquere.y][preSquere.x]->setMoved(true);
									chessBoard[squere.y][squere.x]->setMoved(true);

									delete chessBoard[preSquere.y][preSquere.x + 2];
									chessBoard[preSquere.y][preSquere.x + 2] = chessBoard[preSquere.y][preSquere.x];
									chessBoard[preSquere.y][preSquere.x] = new Piece();

									delete chessBoard[preSquere.y][preSquere.x + 1];
									chessBoard[preSquere.y][preSquere.x + 1] = chessBoard[squere.y][squere.x];
									chessBoard[squere.y][squere.x] = new Piece();
								}
								updateKingLocation(move, preSquere, squere);
							}
							else
							{
								if (chessBoard[preSquere.y][preSquere.x]->getType() == 'K' || chessBoard[preSquere.y][preSquere.x]->getType() == 'k')
								{
									tempKing.y = squere.y;
									tempKing.x = squere.x;
								}
								else
								{
									if (whitesTurn)
									{
										tempKing.y = whiteKing.y;
										tempKing.x = whiteKing.x;
									}
									else
									{
										tempKing.y = blackKing.y;
										tempKing.x = blackKing.x;
									}
								}
								pieceHolder = chessBoard[preSquere.y][preSquere.x];
								checkHolder = chessBoard[squere.y][squere.x];
								chessBoard[preSquere.y][preSquere.x] = new Piece();
								chessBoard[squere.y][squere.x] = pieceHolder;
								if (checkIfCheck(chessBoard, whitesTurn))
								{
									delete chessBoard[preSquere.y][preSquere.x];
									chessBoard[preSquere.y][preSquere.x] = pieceHolder;
									chessBoard[squere.y][squere.x] = checkHolder;
									break;
								}
								else
								{
									delete chessBoard[preSquere.y][preSquere.x];
									chessBoard[preSquere.y][preSquere.x] = pieceHolder;
									chessBoard[squere.y][squere.x] = checkHolder;
								}
								updateKingLocation(move, preSquere, squere);

								pieceHolder = chessBoard[preSquere.y][preSquere.x];
								pieceHolder->setLocation(squere.y, squere.x);

								delete chessBoard[squere.y][squere.x];

								chessBoard[preSquere.y][preSquere.x] = new Piece();
								pieceHolder = pawnChange(pieceHolder, squere.y);
								chessBoard[squere.y][squere.x] = pieceHolder;
							}

							newTurn = true;

							clear_screen();
							updateSpesificSquers(move, preSquere, squere);
							drawPiecesDiff();

							if (!whitesTurn)
							{
								tempKing.y = whiteKing.y;
								tempKing.x = whiteKing.x;
							}
							else
							{
								tempKing.y = blackKing.y;
								tempKing.x = blackKing.x;
							}
							if (checkIfCheck(chessBoard, !whitesTurn))
							{
								if (checkMate(chessBoard, !whitesTurn, preSquere, squere))
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

							if (move != pawnPush)
							{
								for (int y = 0; y < 8; y++)//disable pawn en passante
								{
									for (int x = 0; x < 8; x++)
									{
										if (chessBoard[y][x]->getEn() && (chessBoard[y][x]->getType() == 'p') || chessBoard[y][x]->getType() == 'P')
										{
											chessBoard[y][x]->setEn(false);
										}
									}
								}
							}
						}
						else
						{
							newTurn = true;
						}
					}
				}
			}
		} break;
		default:
		{
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Piece* holder;
	if (playerBlack)
	{
		holder = chessBoard[0][4];
		chessBoard[0][4] = chessBoard[0][3];
		chessBoard[0][3] = holder;
		chessBoard[0][3]->setLocation(0, 3);
		chessBoard[0][4]->setLocation(0, 4);

		holder = chessBoard[7][4];
		chessBoard[7][4] = chessBoard[7][3];
		chessBoard[7][3] = holder;
		chessBoard[7][3]->setLocation(7, 3);
		chessBoard[7][4]->setLocation(7, 4);

		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (isupper(chessBoard[y][x]->getType()))
				{
					chessBoard[y][x]->setType(chessBoard[y][x]->getType() + 0x20);
				}
				else if (islower(chessBoard[y][x]->getType()))
				{
					chessBoard[y][x]->setType(chessBoard[y][x]->getType() - 0x20);
				}
			}
		}
		whiteKing.y = 0;
		whiteKing.x = 3;
		blackKing.y = 7;
		blackKing.x = 3;
	}
	else
	{
		whiteKing.y = 7;
		whiteKing.x = 4;
		blackKing.y = 0;
		blackKing.x = 4;
	}

	//create a window class
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = "game window class";
	windowClass.lpfnWndProc = windowCallBack;

	// regiser class
	RegisterClass(&windowClass);


	//create window
	HWND window = CreateWindow(windowClass.lpszClassName, "Chess", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE , CW_USEDEFAULT, CW_USEDEFAULT, 880, 880, 0, 0, hInstance, 0);
	check = true; //make sure i can check pixel colors when clicks
	HDC hdc = GetDC(window);
	pub = hdc;

	clear_screen();

	StretchDIBits(hdc, 0, 0, renderState.Width, renderState.Height, 0, 0, renderState.Width, renderState.Height, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
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
	announceWinner(whoWon);
	StretchDIBits(hdc, 0, 0, renderState.Width, renderState.Height, 0, 0, renderState.Width, renderState.Height, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	Sleep(1000);
	VirtualFree(renderState.Memory, 0, MEM_RELEASE);

	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			delete chessBoard[y][x];
		}
	}
}

void drawPiecesDiff()
{
	char type;
	for (int y = 0; y < BoardSize; y++)
	{
		for (int x = 0; x < BoardSize; x++)
		{
			type = chessBoard[y][x]->getType();
			switch (type)
			{
			case 'p': //black pawn
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_p_w.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_p_g.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'P': //white pawn
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_p_w.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_p_g.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'o': //black knight
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_o_w.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_o_g.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'O': //white knight
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_o_w.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_o_g.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'b': //black bishop
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_b_w.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_b_g.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'B': //white bishop
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_b_w.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_b_g.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'r': //black rook
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_r_w.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_r_g.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'R': //white rook
			{
				if((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_r_w.bmp", src /2 + (renderState.Width / BoardSize * x),(renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_r_g.bmp", src /2 + (renderState.Width / BoardSize * x),(renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'q': //black queen
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_q_w.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_q_g.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'Q': //white queen
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_q_w.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_q_g.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'k': //black king
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_k_w.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_k_g.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
			}break;
			case 'K': //white king
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_k_w.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_k_g.bmp", src / 2 + (renderState.Width / BoardSize * x), (renderState.Height / BoardSize * y) + 15, pub);
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
void DrawBitmap(LPCSTR piecePath, int x, int y, HDC dc)
{
	//load the bitmap image  "C:\\Users\\USER\\Desktop\\white-Pawn.bmp"
	HBITMAP image = (HBITMAP)LoadImage(0, piecePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	BITMAP bm;
	GetObject(image, sizeof(BITMAP), &bm);

	HDC hdcImage = CreateCompatibleDC(dc);
	SelectObject(hdcImage, image);

	BitBlt(dc, x, y, bm.bmWidth, bm.bmHeight, hdcImage, 0, 0, SRCCOPY);
}
Piece* pawnChange(Piece* piece, int y)
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
bool checkIfCheck(Piece *board[][BoardSize], bool whiteAttacked)
{
	int x = 0, y = 0;
	if (whiteAttacked)
	{
		for (y = 0; y < BoardSize; y++)
		{
			for (x = 0; x < BoardSize; x++)
			{
				if (islower(board[y][x]->getType()) && board[y][x]->movment(board, x, y, whiteKing.x, whiteKing.y))
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
				if (isupper(board[y][x]->getType()) && board[y][x]->movment(board, x, y, blackKing.x, blackKing.y))
				{
					return true;
				}
			}
		}
	}
	return false;
}
void updateSpesificSquers(int move, Squere pre_squere, Squere this_squere)
{
	//StretchDIBits(pub, 0, 0 , renderState.Width, renderState.Height, 0, 0, renderState.Width, renderState.Height, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);//if flickers use the if and else stuff
	if ((pre_squere.y + pre_squere.x) % 2 == 0)
	{
		StretchDIBits(pub, pre_squere.x * renderState.Width / 8, pre_squere.y * renderState.Height / 8, renderState.Width / 8, renderState.Height / 8, 0, 106, renderState.Width / 8, renderState.Height / 8, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		if (move == en)
		{
			StretchDIBits(pub, (pre_squere.x - 1) * renderState.Width / 8, pre_squere.y * renderState.Height / 8, renderState.Width / 8, renderState.Height / 8, 0, 1, renderState.Width / 8, renderState.Height / 8, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
			StretchDIBits(pub, (pre_squere.x + 1) * renderState.Width / 8, pre_squere.y * renderState.Height / 8, renderState.Width / 8, renderState.Height / 8, 0, 1, renderState.Width / 8, renderState.Height / 8, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		else if (move == castle)
		{
			if (this_squere.x < pre_squere.x)
				StretchDIBits(pub, this_squere.x * renderState.Width / 8, this_squere.y * renderState.Height / 8, renderState.Width / 8, renderState.Height / 8, 0, 106, renderState.Width / 8, renderState.Height / 8, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
			else
				StretchDIBits(pub, this_squere.x * renderState.Width / 8, this_squere.y * renderState.Height / 8, renderState.Width / 8, renderState.Height / 8, 0, 1, renderState.Width / 8, renderState.Height / 8, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
	}
	else
	{
		StretchDIBits(pub, pre_squere.x * renderState.Width / 8, pre_squere.y * renderState.Height / 8, renderState.Width / 8, renderState.Height / 8, 0, 1, renderState.Width / 8, renderState.Height / 8, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		if (move == en)
		{
			StretchDIBits(pub, (pre_squere.x - 1) * renderState.Width / 8, pre_squere.y * renderState.Height / 8, renderState.Width / 8, renderState.Height / 8, 0, 106, renderState.Width / 8, renderState.Height / 8, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
			StretchDIBits(pub, (pre_squere.x + 1) * renderState.Width / 8, pre_squere.y * renderState.Height / 8, renderState.Width / 8, renderState.Height / 8, 0, 106, renderState.Width / 8, renderState.Height / 8, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		else if (move == castle)
		{
			if (this_squere.x < pre_squere.x)
				StretchDIBits(pub, this_squere.x * renderState.Width / 8, this_squere.y * renderState.Height / 8, renderState.Width / 8, renderState.Height / 8, 0, 1, renderState.Width / 8, renderState.Height / 8, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
			else
				StretchDIBits(pub, this_squere.x * renderState.Width / 8, this_squere.y * renderState.Height / 8, renderState.Width / 8, renderState.Height / 8, 0, 106, renderState.Width / 8, renderState.Height / 8, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
	}

}
void updateKingLocation(int move, Squere pre_squere, Squere this_squere)
{
	if (move == castle)
	{
		for (int y = 0; y < BoardSize; y++)//keep the location of the kings and rooks when castling
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (chessBoard[y][x]->getType() == 'k')
				{
					blackKing.y = y;
					blackKing.x = x;
					chessBoard[y][x]->setLocation(y, x);
				}
				else if (chessBoard[y][x]->getType() == 'K')
				{
					whiteKing.y = y;
					whiteKing.x = x;
					chessBoard[y][x]->setLocation(y, x);
				}
				else if (chessBoard[y][x]->getType() == 'r')
				{
					chessBoard[y][x]->setLocation(y, x);
				}
				else if (chessBoard[y][x]->getType() == 'R')
				{
					chessBoard[y][x]->setLocation(y, x);
				}
			}
		}
	}
	else
	{
		if (chessBoard[pre_squere.y][pre_squere.x]->getType() == 'k')//always keeps where the kings are
		{
			blackKing.y = this_squere.y;
			blackKing.x = this_squere.x;
		}
		else if (chessBoard[pre_squere.y][pre_squere.x]->getType() == 'K')
		{
			whiteKing.y = this_squere.y;
			whiteKing.x = this_squere.x;
		}
	}
}
bool checkMate(Piece* board[][BoardSize], bool whites_turn, Squere src_s, Squere dst_s)
{
	Piece* holderFirst;
	Piece* holderSecond;


	if (whites_turn)
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
										whiteKing.x = xTemp;
										whiteKing.y = yTemp;
									}
									if (!checkIfCheck(board, whites_turn))
									{
										if (board[yTemp][xTemp]->getType() == 'K')
										{
											whiteKing.x = x;
											whiteKing.y = y;
										}
										delete board[y][x];
										board[y][x] = holderFirst;
										board[yTemp][xTemp] = holderSecond;
										return false;
									}
									if (board[yTemp][xTemp]->getType() == 'K')
									{
										whiteKing.x = x;
										whiteKing.y = y;
									}
									delete board[y][x];
									board[y][x] = holderFirst;
									board[yTemp][xTemp] = holderSecond;
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
										blackKing.x = xTemp;
										blackKing.y = yTemp;
									}
									if (!checkIfCheck(board, whites_turn))
									{
										if (board[yTemp][xTemp]->getType() == 'k')
										{
											blackKing.x = xTemp;
											blackKing.y = yTemp;
										}
										delete board[y][x];
										board[y][x] = holderFirst;
										board[yTemp][xTemp] = holderSecond;
										return false;
									}
									if (board[yTemp][xTemp]->getType() == 'k')
									{
										blackKing.x = xTemp;
										blackKing.y = yTemp;
									}
									delete board[y][x];
									board[y][x] = holderFirst;
									board[yTemp][xTemp] = holderSecond;
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
bool canCastle(Piece* board[][BoardSize], bool whitesTurn, bool left)
{
	if (whitesTurn && left)
	{
		for (int y = 0; y < BoardSize; y++)
		{
			for (int x = 0; x < BoardSize; x++)
			{
				if (islower(board[y][x]->getType()) && board[y][x]->movment(board, x, y, whiteKing.x - 1, whiteKing.y))
				{
					return false;
				}
				if (islower(board[y][x]->getType()) && board[y][x]->movment(board, x, y, whiteKing.x - 2, whiteKing.y))
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