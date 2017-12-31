#include <cassert>
#include <string>
#include <ostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include "logger.h"
#include "board.h"

namespace board
{

template<typename Test, coord_t size>
inline BoardT<Test, size>::BoardT() noexcept
{
    static_assert((0 != size % 2) && (5 <= size),
                  "board size supposed to an odd number greater than five.");
    using namespace std;
    using namespace color;
    
    auto func = [this](auto & link_rb, const Position & pos, Color color) {
        auto & link_array = link_rb[&color];
        int index = 0;
        for (set<coord_t> & link : link_array)
        {
            if (size * size == index)
                break;
            auto pt_center = pos(index);
            for (auto pt_adjacent : pt_center->adj())
            {
                if (!pt_adjacent)
                    continue;
                link.insert(pt_adjacent->index);
            }
            if (pt_center->bAdjBegin[&color])
            {
                link.insert(nBegin);
                link_array[nBegin].insert(index);
            }
            if (pt_center->bAdjEnd[&color])
            {
                link.insert(nEnd);
                link_array[nEnd].insert(index);
            }
            ++index;
        }
    };
    func(_link, _pos, Color::Red);
    func(_link, _pos, Color::Blue);
}

template<typename Test, coord_t size>
inline BoardT<Test, size> &
BoardT<Test, size>::operator()(coord_t row, coord_t col)
{
    assert(size > row && size > col);
    _rowBuf = row;
    _colBuf = col;
    return *this;
}

template<typename Test, coord_t size>
inline BoardT<Test, size>& BoardT<Test, size>::operator()(coord_t index)
{
    assert(size * size > index);
    _rowBuf = _pos(index)->row;
    _colBuf = _pos(index)->col;
    return *this;
}

template<typename Test, coord_t size>
inline const std::array<std::set<coord_t>, size*size + 2> &
BoardT<Test, size>::operator[](Color color) const
{
    return _link[&color];
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
inline std::string BoardT<Test, size>::debug_state_info() const
{
    using namespace std;
    ostringstream oss;
    oss << "current turns: " << terns() << endl;
    oss << "current color: " << color() << endl;
    return oss.str();
}

template<typename Test, coord_t size>
inline std::string BoardT<Test, size>::debug_bit_info() const
{
    using namespace std;
    ostringstream oss;
    Color color = Color::Empty;
    oss << "����";
    for (auto i = 0; i < size * 2; ++i) oss << "��";
    oss << endl << "��" << "  ";
    for (coord_t i = 0; i < size; ++i)
    {
        char c = (i <= 9) ? (char)('0' + i) : (char)('A' + i - 10);
        oss << c << " ";
    }
    oss << " ---> col" << endl;
    for (coord_t row = 0; row < size; ++row)
    {
        char c = (row <= 9) ? (char)('0' + row) : (char)('A' + row - 10);
        oss << "��" << c << "  ";
        oss << setfill(' ') << setw(row) << "";
        for (int col = 0; col < size; ++col)
        {
            // undefined behaviour, maybe not safe?
            auto p_this = const_cast<BoardT<Test, size> *>(this);
            color = (*p_this)(row, col);
            oss << color << " ";
        }
        oss << endl;
    }
    oss << "����";
    for (auto i = 0; i < size * 2; ++i) oss << "��";
    return oss.str();
}

template<typename Test, coord_t size>
inline std::string BoardT<Test, size>::debug_link_info() const
{
    using namespace std;
    ostringstream oss;
    oss << __func__ << endl;
    const char c = '0';
    streamsize s = (streamsize)log10(size - 1) + 1;
    streamsize sp = 15;
    for (coord_t i = 0; i < size * size + 2; ++i)
    {
        auto link = _link[&Color::Red][i];
        oss << setw((streamsize)log10(size * size + 1) + 1) << i;
        switch (i)
        {
        case nBegin:
            oss << " (" << setw(sp) << "begin): ";
            break;
        case nEnd:
            oss << " (" << setw(sp) << "end): ";
            break;
        default:
            oss << " (row_" << setfill(c) << setw(s) << _pos(i)->row
                << ", col_" << setfill(c) << setw(s) << _pos(i)->col
                << "): ";
            break;
        }
        auto func = [&, this](Color color) {
            oss << color << "[";
            for_each(begin(color, i), end(color, i), [&](auto index) {
                oss << " " << index;
            });
            oss << "]";
        };
        func(Color::Red);
        oss << ", ";
        func(Color::Blue);
        oss << endl;
    }
    return oss.str();
}

template<typename Test, coord_t size>
std::ostream& operator<< (std::ostream& stream, BoardT<Test, size> b)
{
    using namespace std;
    stream << __func__ << endl;
    stream << b.debug_state_info();
    stream << b.debug_link_info();
    stream << b.debug_bit_info();
    return stream;
}

}
