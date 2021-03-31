#include "Rook.h"

Rook::Rook(int y, int x, char t)
{
    this->location.x = x;
    this->location.y = y;
    this->type = t;
}

bool Rook::movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
    int i;
    if (src_x == dst_x)
    {
        // Horizontal move
        if (src_y < dst_y)
        {
            // Move right
            for (i = src_y + 1; i < dst_y; ++i)
                if (board[i][src_x] != '#')
                    return false;
        }
        else
        {
            // Move left
            for (i = src_y - 1; i > dst_y; --i)
                if (board[i][src_x] != '#')
                    return false;
        }
    }
    else if (src_y == dst_y)
    {
        // Vertical move
        if (src_x < dst_x)
        {
            // Move down
            for (i = src_x + 1; i < dst_x; ++i)
                if (board[src_y][i] != '#')
                    return false;
        }
        else
        {
            // Move up
            for (i = src_x - 1; i > dst_x; --i)
                if (board[src_y][i] != '#')
                    return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

void Rook::setMoved(bool hasMoved)
{
    this->moved = hasMoved;
}

bool Rook::getMoved()
{
    return this->moved;
}
