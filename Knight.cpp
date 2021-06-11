#include "Knight.h"

Knight::Knight(int y, int x, char t)
{
    this->location.x = x;
    this->location.y = y;
    this->type = t;
}

int Knight::movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
    if (isupper(this->getType()) && isupper(board[dst_y][dst_x]->getType()) || islower(this->getType()) && islower(board[dst_y][dst_x]->getType()))
    {
        return illegal;
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

int Knight::getWorth()
{
    return this->worth;
}
