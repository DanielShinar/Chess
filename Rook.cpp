#include "Rook.h"

Rook::Rook(int y, int x, char t)
{
    this->location.x = x;
    this->location.y = y;
    this->type = t;
}

int Rook::movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y)
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
    else
    {
        return illegal;
    }
    return legal;
}

void Rook::setType(char t)
{
    this->type = t;
}

void Rook::setLocation(int y, int x)
{
    this->location.y = y;
    this->location.x = x;
}

void Rook::setMoved(bool hasMoved)
{
    this->moved = hasMoved;
}

int Rook::getLocationY()
{
    return this->location.y;
}

int Rook::getLocationX()
{
    return this->location.x;
}

char Rook::getType()
{
    return this->type;
}

bool Rook::getMoved()
{
    return this->moved;
}

int Rook::getWorth()
{
    return this->worth;
}
