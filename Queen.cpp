#include "Queen.h"

Queen::Queen(int y, int x, char t)
{
    this->location.x = x;
    this->location.y = y;
    this->type = t;
}

int Queen::movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
    int i;
    if (isupper(this->getType()) && isupper(board[dst_y][dst_x]->getType()) || islower(this->getType()) && islower(board[dst_y][dst_x]->getType()))
    {
        return illegal;
    }
    if (src_x == dst_x) 
    {
        // Horizontal move
        if (src_y < dst_y) 
        {
            // Move right
            for (i = src_y + 1; i < dst_y; ++i)
                if (board[i][src_x]->getType() != '#')
                    return illegal;
        }
        else 
        {
            // Move left
            for (i = src_y - 1; i > dst_y; --i)
                if (board[i][src_x]->getType() != '#')
                    return illegal;
        }
    }
    else if (src_y == dst_y) 
    {
        // Vertical move
        if (src_x < dst_x) 
        {
            // Move down
            for (i = src_x + 1; i < dst_x; ++i)
                if (board[src_y][i]->getType() != '#')
                    return illegal;
        }
        else 
        {
            // Move up
            for (i = src_x - 1; i > dst_x; --i)
                if (board[src_y][i]->getType() != '#')
                    return illegal;
        }
    }
    //bishop movment
    else if (dst_x - src_x == dst_y - src_y)// butt right - top left
    {
        if (src_x + src_y > dst_x + dst_y) //top left
        {
            for (i = 1; i < src_x - dst_x; i++)
                if(board[src_y - i][src_x - i]->getType() != '#')
                    return illegal;
        }
        else // butt right
        {
            for (i = 1; i < dst_x - src_x; i++)
                if(board[src_y+i][src_x+i]->getType() != '#')
                    return illegal;
        }
    }
    else if (dst_x - src_x == src_y - dst_y) // top right - butt left
    {
        if (src_x > dst_x && src_y < dst_y) //butt left
        {
            for(i = 1; i < src_x - dst_x ;i++)
                if(board[src_y+i][src_x-i]->getType() != '#')
                    return illegal;
        }
        else
        {
            for (i = 1; i < dst_x - src_x; i++)
                if (board[src_y - i][src_x + i]->getType() != '#')
                    return illegal;//top right
        }
    } 
    else    
    {
        // Not a valid queen move
        return illegal;
    }
	return legal;

}

void Queen::setType(char t)
{
    this->type = t;
}

void Queen::setLocation(int y, int x)
{
    this->location.y = y;
    this->location.x = x;
}

int Queen::getLocationY()
{
    return this->location.y;
}

int Queen::getLocationX()
{
    return this->location.x;
}

char Queen::getType()
{
    return this->type;
}

int Queen::getWorth()
{
    return this->worth;
}
