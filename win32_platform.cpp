#include "utility.h"

#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "Pawn.h"

#include "GameMaster.h"

#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

POINT pt;
Squere squere;
Squere preSquere;

GameMaster gm;

//#include "renderer.cpp"


LRESULT CALLBACK windowCallBack(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg)
	{
		int wmId, wmEvent;
		case WM_CLOSE:
		case WM_DESTROY:
		{
			gm.setRunning(false);
		} break;
		case WM_ERASEBKGND:
		{
			return true;
		}
		case WM_SIZE:
		{
			gm.re_size(hwnd);
		}
		case WM_LBUTTONDOWN:
		{
			int move = illegal;
			if (gm.getCheck())
			{
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				squere.x = pt.x / (gm.getRenderState().Width / 8);
				squere.y = pt.y / (gm.getRenderState().Height / 8);
				if (gm.first_check(squere))//checks if the player picks a piece to play
				{
					gm.updatePiecesMoves();
					if (gm.getNewTurn())
					{
						gm.drew_piece_move(squere);

						preSquere.x = squere.x;
						preSquere.y = squere.y;
						gm.setNewTurn(false);
					}
					else
					{
						move = gm.get_player_move(squere, preSquere);
						gm.play_turn(move, preSquere, squere);
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
	//create a window class
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = "game window class";
	windowClass.lpfnWndProc = windowCallBack;

	// regiser class
	RegisterClass(&windowClass);


	//create window
	HWND window = CreateWindow(windowClass.lpszClassName, "Chess", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 880, 880, 0, 0, hInstance, 0);
	gm.run_game(window);
}