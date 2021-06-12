#include "utility.h"

#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "Pawn.h"

#include "GameMaster.h"
#include "GameRunner.h"

#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

POINT pt;
Squere squere;
Squere preSquere;


GameRunner gr;

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
			gr.setRunning(false);
		} break;
		case WM_ERASEBKGND:
		{
			return true;
		}
		case WM_SIZE:
		{
			gr.re_size(hwnd);
		}
		case WM_LBUTTONDOWN:
		{
			int move = illegal;
			if (gr.getCheck())
			{
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				squere.x = pt.x / (gr.getRenderState().Width / 8);
				squere.y = pt.y / (gr.getRenderState().Height / 8);
				if (gr.first_check(squere))//checks if the player picks a piece to play
				{
					if (gr.getNewTurn())
					{
						gr.drew_piece_move(squere);

						preSquere.x = squere.x;
						preSquere.y = squere.y;
						gr.setNewTurn(false);
					}
					else
					{
						move = gr.get_player_move(squere, preSquere);
						gr.play_turn(move, preSquere, squere);
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
	gr.run_game(window);
}