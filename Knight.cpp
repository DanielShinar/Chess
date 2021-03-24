#include "Knight.h"

bool Knight::movment(char board[][8], int src_x, int src_y, int dst_x, int dst_y)
{
    if ((src_x - 2 == dst_x && src_y + 1 == dst_y) || (src_x - 2 == dst_x && src_y - 1 == dst_y) || (src_x + 2 == dst_x && src_y + 1 == dst_y) || (src_x + 2 == dst_x && src_y - 1 == dst_y) || (src_y - 2 == dst_y && src_x + 1 == dst_x) || (src_y - 2 == dst_y && src_x - 1 == dst_x) || (src_y + 2 == dst_y && src_x + 1 == dst_x) || (src_y + 2 == dst_y && src_x - 1 == dst_x))
        return true;
    return false;
}
