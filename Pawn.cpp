#include "Pawn.h"

Pawn::Pawn(int y, int x, char t)
{
    this->location.x = x;
    this->location.y = y;
    this->type = t;
}

int Pawn::movment(Piece* board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
    bool black = islower(board[src_y][src_x]->getType());

    if (!playerBlack)
    {
        if ((!black && src_y == 3 && (dst_x == src_x + 1 || dst_x == src_x - 1) && dst_y == src_y - 1 && board[dst_y + 1][dst_x]->getEn()) || (black && src_y == 4 && (dst_x == src_x + 1 || dst_x == src_x - 1) && dst_y == src_y + 1 && board[dst_y - 1][dst_x]->getEn()))
        {
            return en;
        }
        if ((black && src_y + 1 == dst_y || (!black && src_y - 1 == dst_y)) && board[dst_y][dst_x]->getType() == '#' && src_x == dst_x)
        {
            return legal;
        }
        if (((black && src_y == 1 && src_y + 2 == dst_y && board[src_y + 1][src_x]->getType() == '#') || (!black && src_y == 6 && src_y - 2 == dst_y && board[src_y - 1][src_x]->getType() == '#')) && dst_x == src_x && board[dst_y][dst_x]->getType() == '#')
        {
            this->En = true;
            return pawnPush;
        }
        if ((black && (src_x + 1 == dst_x || src_x - 1 == dst_x) && src_y + 1 == dst_y && isupper(board[dst_y][dst_x]->getType())) || (!black && (src_x - 1 == dst_x || src_x + 1 == dst_x) && src_y - 1 == dst_y && islower(board[dst_y][dst_x]->getType())))
        {
            return legal;
        }
    }
    else
    {
        if ((black && src_y == 3 && (dst_x == src_x + 1 || dst_x == src_x - 1) && dst_y == src_y - 1 && board[dst_y + 1][dst_x]->getEn()) || (!black && src_y == 4 && (dst_x == src_x + 1 || dst_x == src_x - 1) && dst_y == src_y + 1 && board[dst_y - 1][dst_x]->getEn()))
        {
            return en;
        }
        if ((black && src_y - 1 == dst_y || (!black && src_y + 1 == dst_y)) && board[dst_y][dst_x]->getType() == '#' && src_x == dst_x)
        {
            return legal;
        }
        if (((black && src_y == 6 && src_y - 2 == dst_y && board[src_y - 1][src_x]->getType() == '#') || (!black && src_y == 1 && src_y + 2 == dst_y && board[src_y + 1][src_x]->getType() == '#')) && dst_x == src_x && board[dst_y][dst_x]->getType() == '#')
        {
            this->En = true;
            return pawnPush;
        }
        if ((black && (src_x + 1 == dst_x || src_x - 1 == dst_x) && src_y - 1 == dst_y && isupper(board[dst_y][dst_x]->getType())) || (!black && (src_x - 1 == dst_x || src_x + 1 == dst_x) && src_y + 1 == dst_y && islower(board[dst_y][dst_x]->getType())))
        {
            return legal;
        }
    }
    return illegal;
}

void Pawn::setType(char t)
{
    this->type = t;
}

void Pawn::setLocation(int y, int x)
{
    this->location.y = y;
    this->location.x = x;
}

void Pawn::setEn(bool canEn)
{
    this->En = canEn;
}

bool Pawn::getEn()
{
    return this->En;
}

int Pawn::getLocationY()
{
    return this->location.y;
}

int Pawn::getLocationX()
{
    return this->location.x;
}

char Pawn::getType()
{
    return this->type;
}

int Pawn::getWorth()
{
    return this->worth;
}
