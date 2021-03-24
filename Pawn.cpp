#include "Pawn.h"

bool Pawn::movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
    bool black = islower(board[src_y][src_x]);
    if ((black && src_y + 1 == dst_y || (!black && src_y - 1 == dst_y)) && board[dst_y][dst_x] == '#' && src_x == dst_x)
    {
        return true;
    }
    if (((black && src_y == 1 && src_y + 2 == dst_y && board[src_y+1][src_x] == '#') || (!black && src_y == 6 && src_y - 2 == dst_y && board[src_y-1][src_x] == '#')) && dst_x == src_x && board[dst_y][dst_x] == '#')
    {
        return true;
    }
    if ((black && (src_x + 1 == dst_x || src_x - 1 == dst_x) && src_y + 1 == dst_y && isupper(board[dst_y][dst_x])) || (!black && (src_x - 1 == dst_x || src_x + 1 == dst_x) && src_y - 1 == dst_y && islower(board[dst_y][dst_x])))
    {
        return true;
    }
    return false;
}
