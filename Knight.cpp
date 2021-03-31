#include "Knight.h"

Knight::Knight(int y, int x, char t)
{
    this->location.x = x;
    this->location.y = y;
    this->type = t;
}

int Knight::movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (board[y][x]->getEn() && (board[y][x]->getType() == 'p') || board[y][x]->getType() == 'P')
            {
                board[y][x]->setEn(false);
            }
        }
    }
    if ((src_x - 2 == dst_x && src_y + 1 == dst_y) || (src_x - 2 == dst_x && src_y - 1 == dst_y) || (src_x + 2 == dst_x && src_y + 1 == dst_y) || (src_x + 2 == dst_x && src_y - 1 == dst_y) || (src_y - 2 == dst_y && src_x + 1 == dst_x) || (src_y - 2 == dst_y && src_x - 1 == dst_x) || (src_y + 2 == dst_y && src_x + 1 == dst_x) || (src_y + 2 == dst_y && src_x - 1 == dst_x))
        return legal;
    return illegal;
}

void Knight::setType(char t)
{
    this->type = t;
}

void Knight::setLocation(int y, int x)
{
    this->location.y = y;
    this->location.x = x;
}

int Knight::getLocationY()
{
    return this->location.y;
}

int Knight::getLocationX()
{
    return this->location.x;
}

char Knight::getType()
{
    return this->type;
}
