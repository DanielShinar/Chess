#include "gameHandler.h"

bool gameHandler::canMove(char chessboard[8][8], char piece, int src_x, int src_y, int dst_x, int dst_y)
{
    if (((chessboard[dst_y][dst_x] == '#') || islower(chessboard[src_y][src_x]) && isupper(chessboard[dst_y][dst_x]) || isupper(chessboard[src_y][src_x]) && islower(chessboard[dst_y][dst_x])))
    {
        if (piece == 'k' || piece == 'K')
            return this->king.movment(chessboard, src_x, src_y, dst_x, dst_y);
        if (piece == 'q' || piece == 'Q')
            return this->queen.movment(chessboard, src_x, src_y, dst_x, dst_y);
        if (piece == 'r' || piece == 'R')
            return this->rook.movment(chessboard, src_x, src_y, dst_x, dst_y);
        if (piece == 'b' || piece == 'B')
            return this->bishop.movment(chessboard, src_x, src_y, dst_x, dst_y);
        if (piece == 'o' || piece == 'O')
            return this->knight.movment(chessboard, src_x, src_y, dst_x, dst_y);
        if (piece == 'p' || piece == 'P')
            return this->pawn.movment(chessboard, src_x, src_y, dst_x, dst_y);
    }
    return false;
    
}
