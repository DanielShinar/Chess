#include "Bishop.h"

Bishop::Bishop(int y, int x, char t)
{
    this->location.x = x;
    this->location.y = y;
    this->type = t;
}

bool Bishop::movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
    int i;
    if (dst_x - src_x == dst_y - src_y)// butt right - top left
    {
        if (src_x + src_y > dst_x + dst_y) //top left
        {
            for (i = 1; i < src_x - dst_x; i++)
                if (board[src_y - i][src_x - i] != '#')
                    return false;
        }
        else // butt right
        {
            for (i = 1; i < dst_x - src_x; i++)
                if (board[src_y + i][src_x + i] != '#')
                    return false;
        }
    }
    else if (dst_x - src_x == src_y - dst_y) // top right - butt left
    {
        if (src_x > dst_x && src_y < dst_y) //butt left
        {
            for (i = 1; i < src_x - dst_x; i++)
                if (board[src_y + i][src_x - i] != '#')
                    return false;
        }
        else
        {
            for (i = 1; i < dst_x - src_x; i++)
                if (board[src_y - i][src_x + i] != '#')
                    return false;//top right
        }
    }
    else
    {
        // Not a valid queen move
        return false;
    }
    return true;
}