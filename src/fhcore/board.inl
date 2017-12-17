#include <cassert>
#include <string>
#include <ostream>
#include <iomanip>
#include <algorithm>
#include "logger.h"
#include "board.h"

namespace board
{
template<typename Test, coord_t size>
inline BoardT<Test, size>::BoardT() noexcept
{
    check_boardsize();

    for (auto & link_array : _link)
    {
        int index = 0;
        for (auto & link : link_array)
        {
            if (size * size == index)
                break;
            auto pt_center = _pos(index);
            for (auto pt_adjacent : pt_center->adj())
            {
                auto pt = pt_adjacent;
                if (!pt)
                    continue;
                link.insert(pt->index);
            }
            ++index;
        }
    }

    for (int i = 0; i < size * size; ++i)
    {
        auto link = _link[1][i];
        ostringstream os;
        os << "Red " << i << " linked with: ";
        for (auto index : link)
        {
            os << index << " ";
        }
        debug() << os.str();
    }


}

template<typename Test, coord_t size>
inline BoardT<Test, size> &
BoardT<Test, size>::operator()(coord_t row, coord_t col)
{
    assert(row < size && col < size);
    _rowBuf = row;
    _colBuf = col;
    return *this;
}

template<typename Test, coord_t size>
inline void BoardT<Test, size>::operator=(const Color color)
{
    if (Color::Empty == color)
    {
        _bit[&Color::Red].reset(index());
        _bit[&Color::Blue].reset(index());
    }
    else // Not empty
    {
        _bit[&color].set(index());
    }
}

template<typename Test, coord_t size>
inline BoardT<Test, size>::operator Color()
{
    if (_bit[&Color::Red][index()])
        return Color::Red;
    else if (_bit[&Color::Blue][index()])
        return Color::Blue;
    else
        return Color::Empty;
}

template<typename Test, coord_t size>
inline size_t BoardT<Test, size>::terns() const
{
    return _bit[&Color::Red].count() + _bit[&Color::Blue].count();
}

template<typename Test, coord_t size>
inline Color BoardT<Test, size>::color() const
{
    return terns() % 2 ? Color::Blue : Color::Red;
}

template<typename Test, coord_t size>
inline Color BoardT<Test, size>::winner() const
{
    return Color();
}

template<typename Test, coord_t size>
inline coord_t BoardT<Test, size>::index() const
{
    return _pos(_rowBuf, _colBuf)->index;
}

template<typename Test, coord_t size>
inline coord_t BoardT<Test, size>::index(coord_t row, coord_t col) const
{
    return row * size + col;
}

template<typename Test, coord_t size>
inline void board::BoardT<Test, size>::check_boardsize()
{
    using namespace std;
    using namespace logger;
    if (0 == size % 2)
    {
        ostringstream oss;
        oss << __func__ << "(" << size << ")" << " illegal!" << endl;
        oss << "board size supposed to an odd number greater than five.";

        debug(Level::Fatal) << oss.str();
        throw;
    }
}

template<typename Test, coord_t size>
std::ostream& operator<< (std::ostream& stream, BoardT<Test, size> b)
{
    using namespace std;
    stream << __func__ << endl;
    stream << "current turns: " << b.terns() << endl;
    stream << "current color: " << b.color() << endl;
    Color c;
    for (int row = 0; row < size; ++row)
    {
        stream << setfill(' ') << setw(row) << "";
        for (int col = 0; col < size; ++col)
        {
            c = b(row, col);
            stream << c << " ";
        }
        stream << endl;
    }

    return stream;
}

}
