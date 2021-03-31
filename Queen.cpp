#include "Queen.h"

Queen::Queen(int y, int x, char t)
{
    this->location.x = x;
    this->location.y = y;
    this->type = t;
}

bool Queen::movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y)
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
    //bishop movment
    else if (dst_x - src_x == dst_y - src_y)// butt right - top left
    {
        if (src_x + src_y > dst_x + dst_y) //top left
        {
            for (i = 1; i < src_x - dst_x; i++)
                if(board[src_y - i][src_x - i] != '#')
                    return false;
        }
        else // butt right
        {
            for (i = 1; i < dst_x - src_x; i++)
                if(board[src_y+i][src_x+i] != '#')
                    return false;
        }
    }
    else if (dst_x - src_x == src_y - dst_y) // top right - butt left
    {
        if (src_x > dst_x && src_y < dst_y) //butt left
        {
            for(i = 1; i < src_x - dst_x ;i++)
                if(board[src_y+i][src_x-i] != '#')
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
