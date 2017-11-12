#include <cassert>
#include <string>
#include <ostream>
#include <iomanip>
#include "logger.h"
#include "board.h"

#define BUFFER_INDEX (_rowBuf * size + _colBuf)

namespace board
{
template<typename Test, int size>
inline BoardT<Test, size>::BoardT() noexcept
{
    check_boardsize();
}

template<typename Test, int size>
inline BoardT<Test, size> &
BoardT<Test, size>::operator()(coord_t row, coord_t col)
{
    assert(row < size && col < size);
    _rowBuf = row;
    _colBuf = col;
    return *this;
}

template<typename Test, int size>
inline void BoardT<Test, size>::operator=(const Color color)
{
    if (Color::Empty == color)
    {
        _bit[&Color::Red].reset(BUFFER_INDEX);
        _bit[&Color::Blue].reset(BUFFER_INDEX);
    }
    else // Not empty
    {
        _bit[&color].set(BUFFER_INDEX, 1);
    }
}

template<typename Test, int size>
inline BoardT<Test, size>::operator Color()
{
    if (_bit[&Color::Red][BUFFER_INDEX])
        return Color::Red;
    else if (_bit[&Color::Blue][BUFFER_INDEX])
        return Color::Blue;
    else
        return Color::Empty;
}

template<typename Test, int size>
inline size_t BoardT<Test, size>::terns() const
{
    return _bit[&Color::Red].count() + _bit[&Color::Blue].count();
}

template<typename Test, int size>
inline Color BoardT<Test, size>::color() const
{
    return terns() % 2 ? Color::Blue : Color::Red;
}

template<typename Test, int size>
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

template<typename Test, int size>
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
