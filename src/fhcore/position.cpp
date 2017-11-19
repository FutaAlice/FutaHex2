#include <iomanip>
#include "position.h"
using namespace std;
namespace position
{

pos_t::pos_t(coord_t row, coord_t col, coord_t size)
    :row(row), col(col), size(size), index(row * size + col)
{
}

pos_t *position::pos_t::adj(int dir) const
{
    return _adjacent[dir];
}

void position::pos_t::setAdj(int dir, pos_t *adj)
{
    _adjacent[dir] = adj;
}

ostream & operator<<(ostream & stream, const pos_t pt)
{
    stream << endl;
    auto func = [&stream](pos_t pt, const char *indent = "") {
        stream << indent << pt.index
            << " (row: " << pt.row << ", col:" << pt.col << ")" << endl;
    };
    stream << typeid(pt).name() << ", ";
    func(pt);
    stream << "adjacent pos: " << endl;
    for (int i = 0; i < 6; ++i)
    {
        auto adj = pt.adj(i);
        if (adj)
        {
            func(*adj, "\t");
        }
    }
    return stream;
}

}
