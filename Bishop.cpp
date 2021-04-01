#include "Bishop.h"

Bishop::Bishop(int y, int x, char t)
{
    this->location.x = x;
    this->location.y = y;
    this->type = t;
}

int Bishop::movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
    int i;
    if (dst_x - src_x == dst_y - src_y)// butt right - top left
    {
        if (src_x + src_y > dst_x + dst_y) //top left
        {
            for (i = 1; i < src_x - dst_x; i++)
                if (board[src_y - i][src_x - i]->getType() != '#')
                    return illegal;
        }
        else // butt right
        {
            for (i = 1; i < dst_x - src_x; i++)
                if (board[src_y + i][src_x + i]->getType() != '#')
                    return illegal;
        }
    }
    else if (dst_x - src_x == src_y - dst_y) // top right - butt left
    {
        if (src_x > dst_x && src_y < dst_y) //butt left
        {
            for (i = 1; i < src_x - dst_x; i++)
                if (board[src_y + i][src_x - i]->getType() != '#')
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
        // Not a valid bishop move
        return illegal;
    }
    if (!this->getMoved())
        this->setMoved(true);
    return legal;
}

void Bishop::setType(char t)
{
    this->type = t;
}

void Bishop::setLocation(int y, int x)
{
    this->location.y = y;
    this->location.x = x;
}

int Bishop::getLocationY()
{
    return this->location.y;
}

int Bishop::getLocationX()
{
    return this->location.x;
}

char Bishop::getType()
{
    return this->type;
}
