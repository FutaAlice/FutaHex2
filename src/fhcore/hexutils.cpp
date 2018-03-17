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

}
