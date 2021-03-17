#include "utility.cpp"
#include <Windows.h>
#include <windowsx.h>
#include <wingdi.h>
#include <cctype>
#include <cstring>


#include <string>

#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

#define chessPath "C:\\Users\\USER\\Desktop\\chessBoardBMP.bmp"
#define size 8
#define dst 80
#define src 30

POINT pt;
struct Squere
{
	int x, y;
};
struct RenderState
{
	int Height, Width;
	void* Memory;

	BITMAPINFO BitMapInfo;

};

globalVariable bool check = false;
globalVariable bool newTurn = true;
globalVariable bool running = true;
globalVariable RenderState renderState;
globalVariable Squere preSquere;
globalVariable Squere squere;
globalVariable HDC pub;
globalVariable HBITMAP hBitmap;
globalVariable char chessBoard[size][size] = {
	{'r','o','b','q','k','b','o','r'},
	{'p','p','p','p','p','p','p','p'},
	{'#','#','#','#','#','#','#','#'},
	{'#','#','#','#','#','#','#','#'},
	{'#','#','#','#','#','#','#','#'},
	{'#','#','#','#','#','#','#','#'},
	{'P','P','P','P','P','P','P','P'},
	{'R','O','B','Q','K','B','O','R'} }; //upper case is white lower case is black


void drawPieces();
void drawPiecesDiff();
void DrawBitmap(LPCSTR piecePath, int x, int y, HDC dc);

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
			char pieceHolder;
			if (check)
			{
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				squere.x = pt.x / (renderState.Width / 8);
				squere.y = pt.y / (renderState.Height / 8);
				if ((chessBoard[squere.y][squere.x] != '#' && newTurn) || (!newTurn))//checks if the player picks a piece to play
				{
					if (newTurn)
					{
						preSquere.x = squere.x;
						preSquere.y = squere.y;
						newTurn = false;
					}
					else
					{
						pieceHolder = chessBoard[preSquere.y][preSquere.x];
						chessBoard[preSquere.y][preSquere.x] = '#';
						chessBoard[squere.y][squere.x] = pieceHolder;
						newTurn = true;

					}
					clear_screen(0xffffcc, 0x00bb00);
				}
			}
			StretchDIBits(pub, 0, 0, renderState.Width, renderState.Height, 0, 0, renderState.Width, renderState.Height, renderState.Memory, &renderState.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
			drawPiecesDiff();
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

	clear_screen(0xffffcc, 0x00bb00);

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
}
void drawPieces()
{
	int upsideY = 7;
	for (int y = 0; y < size; y++)
	{
		int upsideX = 7;
		for (int x = 0; x < size; x++)
			{
				switch (chessBoard[y][x])
				{
				case 'p': //black pawn
				{

				}break;
				case 'P': //white pawn
				{

				}break;
				case 'o': //black knight
				{

				}break;
				case 'O': //white knight
				{

				}break;
				case 'b': //black bishop
				{

				}break;
				case 'B': //white bishop
				{

				}break;
				case 'r': //black rook
				{

				}break;
				case 'R': //white rook
				{

				}break;
				case 'q': //black queen
				{

				}break;
				case 'Q': //white queen
				{

				}break;
				case 'k': //black king
				{

				}break;
				case 'K': //white king
				{

				}break;

				deafult:
					break;
				}

				if (chessBoard[y][x] != '#')
				{
					if (isupper(chessBoard[y][x]))
					{
						draw_rect_in_pixels(src + (renderState.Width / size * x), src + (renderState.Height / size * upsideY), dst + (renderState.Width / size * x), dst + (renderState.Height / size * upsideY), 0xf667f0);
					}
					else
					{
						draw_rect_in_pixels(src + (renderState.Width / size * x), src + (renderState.Height / size * upsideY), dst + (renderState.Width / size * x), dst + (renderState.Height / size * upsideY), 0x990099);
					}
				}
				upsideX--;
			}
		upsideY--;
	}
}

void drawPiecesDiff()
{
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			switch (chessBoard[y][x])
			{
			case 'p': //black pawn
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_p_w.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_p_g.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
			}break;
			case 'P': //white pawn
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_p_w.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_p_g.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
			}break;
			case 'o': //black knight
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_o_w.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_o_g.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
			}break;
			case 'O': //white knight
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_o_w.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_o_g.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
			}break;
			case 'b': //black bishop
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_b_w.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_b_g.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
			}break;
			case 'B': //white bishop
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_b_w.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_b_g.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
			}break;
			case 'r': //black rook
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_r_w.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_r_g.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
			}break;
			case 'R': //white rook
			{
				if((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_r_w.bmp", src /2 + (renderState.Width / size * x),(renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_r_g.bmp", src /2 + (renderState.Width / size * x),(renderState.Height / size * y) + 15, pub);
			}break;
			case 'q': //black queen
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_q_w.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_q_g.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
			}break;
			case 'Q': //white queen
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_q_w.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_q_g.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
			}break;
			case 'k': //black king
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_k_w.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\black_k_g.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
			}break;
			case 'K': //white king
			{
				if ((y + x) % 2 == 0)
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_k_w.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
				else
					DrawBitmap("C:\\Users\\USER\\Desktop\\Projects\\CPP\\Chess\\pieces\\white_k_g.bmp", src / 2 + (renderState.Width / size * x), (renderState.Height / size * y) + 15, pub);
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