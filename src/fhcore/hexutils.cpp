#include <cmath>
#include <iomanip>
#include <sstream>
#include "hexutils.h"

using namespace std;
namespace hexutils
{

string xy2symbol(int row, int col, int size)
{
    if (row >= size || col >= size)
        throw;

    int digits = (int)log10(size) + 1;
    ostringstream oss;
    oss << (char)('A' + col) << "-"
        << setw(digits) << setfill('0') << (row + 1);
    return oss.str();
}

// gam_locate = (3 + size) + col + row * (size + 2)
uint8_t xy2gamlocate(int row, int col, int size)
{
    if (row >= size || col >= size || size > 11)
        throw;
    return 3 + size + col + (size + 2) * row;
}

tuple<int, int> gamlocate2xy(int gamlocate, int size)
{
    if (size > 11 || size % 2 == 0)
        throw;
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            if (gamlocate == xy2gamlocate(row, col, size))
                return make_tuple(row, col);
        }
    }
    throw;
}



}
