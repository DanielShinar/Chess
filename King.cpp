#include "King.h"

King::King(int y, int x, char t)
{
    this->location.x = x;
    this->location.y = y;
    this->type = t;
}

bool King::movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y)
{

    if ((dst_x == src_x + 1 || dst_x == src_x - 1 || dst_x == src_x) && (dst_y == src_y + 1 || dst_y == src_y - 1 || dst_y == src_y))
        return true;
    return false;
}

void King::setMoved(bool hasMoved)
{
    this->moved = hasMoved;
}

bool King::getMoved()
{
    return this->moved;
}
