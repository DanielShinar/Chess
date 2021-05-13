#include "King.h"

King::King(int y, int x, char t)
{
    this->location.x = x;
    this->location.y = y;
    this->type = t;
}

int King::movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
    if ((dst_x == src_x + 1 || dst_x == src_x - 1 || dst_x == src_x) && (dst_y == src_y + 1 || dst_y == src_y - 1 || dst_y == src_y))
    {
        return legal;
    }
    if (board[dst_y][dst_x]->getType() == 'O' || board[dst_y][dst_x]->getType() == 'o')
    {
        bool checkIfthisMoved = this->getMoved();
        bool checkIfthatMoved = board[dst_y][dst_x]->getMoved();
        int check = board[dst_y][dst_x]->movment(board, dst_x, dst_y, src_x, src_y);
        if (!this->getMoved() && !board[dst_y][dst_x]->getMoved() && board[dst_y][dst_x]->movment(board, dst_x, dst_y, src_x, src_y) == legal)
        {
            return castle;
        }
    }
    return illegal;
}

void King::setType(char t)
{
    this->type = t;
}

void King::setLocation(int y, int x)
{
    this->location.y = y;
    this->location.x = x;
}

void King::setMoved(bool hasMoved)
{
    this->moved = hasMoved;
}

bool King::getMoved()
{
    return this->moved;
}

int King::getLocationY()
{
    return this->location.y;
}

int King::getLocationX()
{
    return this->location.x;
}

char King::getType()
{
    return this->type;
}
