#include "stats_util.h"
#include "global.h"

char hand(int row0, int col0)
{
    if (col0 < COL/2) {return 'l';}
    else {return 'r';}
}
