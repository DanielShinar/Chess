#include "King.h"

bool King::movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y)
{

    if ((dst_x == src_x + 1 || dst_x == src_x - 1 || dst_x == src_x) && (dst_y == src_y + 1 || dst_y == src_y - 1 || dst_y == src_y))
        return true;
    return false;
}
