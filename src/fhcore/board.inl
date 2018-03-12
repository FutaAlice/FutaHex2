#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <typeinfo>
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
    using namespace logger;
    using namespace color;

    debug(Level::Debug) << __func__
        << "<" << typeid(Test).name() << "," << size << ">" << " "
        << "constructor...";
    
    auto func = [this](auto & link_rb, const Position & pos, Color color) {
        auto & link_array = link_rb[*color];
        int index = 0;
        for (set<coord_t> & link : link_array)
        {
            if (size * size == index)
                break;
            auto pt_center = pos(index);
            for (auto pt_adjacent : pt_center->adj())
            {
                link.insert(pt_adjacent->index);
            }
            if (pt_center->bAdjBegin[*color])
            {
                link.insert(Position::nBegin);
                link_array[Position::nBegin].insert(index);
            }
            if (pt_center->bAdjEnd[*color])
            {
                link.insert(Position::nEnd);
                link_array[Position::nEnd].insert(index);
            }
            ++index;
        }
    };
    func(_link, _pos, Color::Red);
    func(_link, _pos, Color::Blue);
}

template<typename Test, coord_t size>
inline BoardT<Test, size>::BoardT(const BoardT & b) noexcept
    : _bit{ b._bit[0], b._bit[1] }
    , _link{ b._link[0], b._link[1] }
{
    using namespace logger;
    debug(Level::Debug) << __func__ 
        << "<" << typeid(Test).name() << "," << size << ">" << " "
        <<"copy constructor...";
}

template<typename Test, coord_t size>
inline BoardT<Test, size>::BoardT(BoardT &&) noexcept
{
    using namespace logger;
    debug(Level::Debug) << __func__
        << "<" << typeid(Test).name() << "," << size << ">" << " "
        << "move constructor...";
    // TODO!!!
    assert(false);
}

template<typename Test, coord_t size>
inline BoardT<Test, size>::~BoardT() noexcept
{
    using namespace logger;
    debug(Level::Debug) << __func__
        << "<" << typeid(Test).name() << "," << size << ">" << " "
        << "destructor...";
}

template<typename Test, coord_t size>
inline IBoard * BoardT<Test, size>::create()
{
    return new BoardT();
}

template<typename Test, coord_t size>
inline IBoard & BoardT<Test, size>::operator()(coord_t row, coord_t col)
{
    assert(size > row && size > col);
    _rowBuf = row;
    _colBuf = col;
    return *this;
}

template<typename Test, coord_t size>
inline IBoard & BoardT<Test, size>::operator()(coord_t index)
{
    assert(size * size > index);
    _rowBuf = _pos(index)->row;
    _colBuf = _pos(index)->col;
    return *this;
}

template<typename Test, coord_t size>
inline std::tuple<const std::set<coord_t>*, size_t>
BoardT<Test, size>::operator[](const Color color) const
{
    using namespace std;
    return make_tuple(_link[*color].data(), _link[*color].size());
}

template<typename Test, coord_t size>
inline void BoardT<Test, size>::operator=(const Color color)
{
    if (Color::Empty == color)
    {
        reset_piece();
    }
    else // Not empty
    {
        set_piece(color);
    }
}

template<typename Test, coord_t size>
inline BoardT<Test, size>::operator Color() const
{
    if (_bit[*Color::Red][buf_index()])
        return Color::Red;
    else if (_bit[*Color::Blue][buf_index()])
        return Color::Blue;
    else
        return Color::Empty;
}

template<typename Test, coord_t size>
inline size_t BoardT<Test, size>::boardsize() const
{
    return size;
}

template<typename Test, coord_t size>
inline size_t BoardT<Test, size>::terns() const
{
    return _bit[*Color::Red].count() + _bit[*Color::Blue].count();
}

template<typename Test, coord_t size>
inline Color BoardT<Test, size>::color() const
{
    return terns() % 2 ? Color::Blue : Color::Red;
}

template<typename Test, coord_t size>
inline Color BoardT<Test, size>::winner() const
{
    bool r_win = (_link[*Color::Red][Position::nBegin].end() !=
                    _link[*Color::Red][Position::nBegin].find(Position::nEnd));
    bool b_win = (_link[*Color::Blue][Position::nBegin].end() !=
                    _link[*Color::Blue][Position::nBegin].find(Position::nEnd));
    assert(!(r_win && b_win));
    if (r_win)
        return Color::Red;
    else if (b_win)
        return Color::Blue;
    else
        return Color::Empty;
}

template<typename Test, coord_t size>
inline bool BoardT<Test, size>::empty() const
{
    return _bit[*Color::Red].none() && _bit[*Color::Blue].none();
}

template<typename Test, coord_t size>
inline bool BoardT<Test, size>::empty(coord_t index) const
{
    return _bit[*Color::Red][index] == 0 && _bit[*Color::Blue][index] == 0;
}

template<typename Test, coord_t size>
inline bool BoardT<Test, size>::empty(coord_t row, coord_t col) const
{
    coord_t index = _pos(row, col)->index;
    return empty(index);
}

template<typename Test, coord_t size>
inline coord_t BoardT<Test, size>::nBegin() const
{
    return Position::nBegin;
}

template<typename Test, coord_t size>
inline coord_t BoardT<Test, size>::nEnd() const
{
    return Position::nEnd;
}

template<typename Test, coord_t size>
inline bool BoardT<Test, size>::equal_to(const IBoard & rhs) const
{
    using namespace logger;
    const BoardT & ref = *dynamic_cast<const BoardT *>(&rhs);

    if (_bit[*Color::Red] != ref._bit[*Color::Red] ||
        _bit[*Color::Blue] != ref._bit[*Color::Blue])
        return false;

#if defined(DEBUG) || defined(_DEBUG)
    bool flag_r = _link[*Color::Red] != ref._link[*Color::Red];
    bool flag_b = _link[*Color::Blue] != ref._link[*Color::Blue];
    if (flag_r || flag_b)
    {
        // do something
        debug(Level::Info) << debug_link_str();
        debug(Level::Info) << ref.debug_link_str();

        return false;
    }
#endif // DEBUG

    return true;
}

template<typename Test, coord_t size>
inline std::set<coord_t>::iterator
BoardT<Test, size>::begin(const Color color, coord_t index) const
{
    return _link[*color][index].begin();
}

template<typename Test, coord_t size>
inline std::set<coord_t>::iterator
BoardT<Test, size>::end(const Color color, coord_t index) const
{
    return _link[*color][index].end();
}

template<typename Test, coord_t size>
inline IBoard * BoardT<Test, size>::copy() const
{
    return new BoardT(*this);
}

template<typename Test, coord_t size>
inline std::string BoardT<Test, size>::debug_state_str() const
{
    using namespace std;
    ostringstream oss;
    oss << endl;
    oss << "current turns: " << terns() << endl;
    oss << "current color: " << color() << endl;
    return oss.str();
}

template<typename Test, coord_t size>
inline std::string BoardT<Test, size>::debug_bit_str() const
{
    using namespace std;
    ostringstream oss;
    Color color = Color::Empty;
    oss << endl;
    oss << "©°©¤";
    for (auto i = 0; i < size * 2; ++i) oss << "©¤";
    oss << endl << "©¦" << "  ";
    for (coord_t i = 0; i < size; ++i)
    {
        char c = (i <= 9) ? (char)('0' + i) : (char)('A' + i - 10);
        oss << c << " ";
    }
    oss << " ---> col" << endl;
    for (coord_t row = 0; row < size; ++row)
    {
        char c = (row <= 9) ? (char)('0' + row) : (char)('A' + row - 10);
        oss << "©¦" << c << "  ";
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
    oss << "©¸©¤";
    for (auto i = 0; i < size * 2; ++i) oss << "©¤";
    return oss.str();
}

template<typename Test, coord_t size>
inline std::string BoardT<Test, size>::debug_link_str() const
{
    using namespace std;
    ostringstream oss;
    oss << endl;
    oss << __func__ << endl;
    const char c = '0';
    streamsize s = (streamsize)log10(size - 1) + 1;
    streamsize sp = 15;
    for (coord_t i = 0; i < size * size + 2; ++i)
    {
        auto link = _link[*Color::Red][i];
        oss << setw((streamsize)log10(size * size + 1) + 1) << i;
        switch (i)
        {
        case Position::nBegin:
            oss << " (" << setw(sp) << "begin): ";
            break;
        case Position::nEnd:
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
inline bool BoardT<Test, size>::operator==(const BoardT & rhs) const
{
    return equal_to(rhs);
}

template<typename Test, coord_t size>
inline bool BoardT<Test, size>::operator!=(const BoardT & rhs) const
{
    return !equal_to(rhs);
}

template<typename Test, coord_t size>
inline coord_t BoardT<Test, size>::buf_index() const
{
    return _pos(_rowBuf, _colBuf)->index;
}

template<typename Test, coord_t size>
inline void
BoardT<Test, size>::get_direct_capture_union(std::set<coord_t> & out,
                                             coord_t center, Color color,
                                             bool first_time/* = true*/) const
{
    out.insert(center);
    for (auto pos_adj : _pos(center)->adj())
    {
        const coord_t adj_index = pos_adj->index;
        // if own color captured
        if (_bit[*color][adj_index] &&
            out.end() == out.find(adj_index))
        {
            get_direct_capture_union(out, adj_index, color, false);
        }
    }
    //if (first_time)
    //    out.erase(center);
}

template<typename Test, coord_t size>
inline void
BoardT<Test, size>::get_direct_link_union(std::set<coord_t> & out,
                                          std::set<coord_t> & except,
                                          coord_t center, Color color,
                                          bool first_time/* = true*/) const
{
    using namespace std;
    using namespace logger;

    const pos_t *pos_center = _pos(center);
    // mark center index as excpet
    except.insert(center);

    // handle each adjance block around center
    for (auto pos_adj : pos_center->adj())
    {
        const coord_t adj_index = pos_adj->index;
        // if oppsite color captured
        if (_bit[*!color][adj_index])
        {
            // do nothing
        }
        // if own color captured
        else if (_bit[*color][adj_index])
        {
            // check if adj_index already searched
            if (except.end() == except.find(adj_index))
            {
                // if not, marked in except
                except.insert(adj_index);
                // recursion
                get_direct_link_union(out, except, adj_index, color, false);
            }
        }
        // empty block
        else
        {
            out.insert(adj_index); // link to empty block
        }
    }

    // link to begin/end point if own color captured
    if (pos_center->bAdjBegin[*color])
        out.insert(nBegin());
    if (pos_center->bAdjEnd[*color])
        out.insert(nEnd());

    // make sure do not link itself
    if (first_time)
        out.erase(center);
}

template<typename Test, coord_t size>
inline void BoardT<Test, size>::set_piece(const Color color)
{
    const auto center = buf_index();
    // make sure position empty
    assert(0 == _bit[*!color][center]);
    // current-color, set bitmap
    _bit[*color].set(center);
    // opposite-color, clear link
    for (auto adj_index : _link[*!color][center])
    {
        _link[*!color][adj_index].erase(center);
    }
    _link[*!color][center].clear();
    // current-color, link
    for (auto iter = _link[*color][center].begin();
         iter != _link[*color][center].end();
         ++iter)
    {
        // disconnect with center-point
        _link[*color][*iter].erase(center);
        // connect with point around center
        auto it_adj = iter;
        for (++it_adj; it_adj != _link[*color][center].end(); ++it_adj)
        {
            _link[*color][*iter].insert(*it_adj);
            _link[*color][*it_adj].insert(*iter);
        }
    }
    _link[*color][center].clear();
}

template<typename Test, coord_t size>
inline void BoardT<Test, size>::reset_piece()
{
    using namespace std;
    using namespace logger;
    // color of previous owner
    Color previous = *this;
    if (Color::Empty == previous)
        return;

    const coord_t center = buf_index();

    // reset bitmap for both color
    _bit[*Color::Red].reset(center);
    _bit[*Color::Blue].reset(center);

    // ******************** color of previous owner ********************
    set<coord_t> captured;
    get_direct_capture_union(captured, center, previous);
    // clear link state around captured block
    for (auto cap_index : captured)
    {
        for (auto pos_adj : _pos(cap_index)->adj())
        {
            const coord_t adj = pos_adj->index;
            if (!empty(adj))
                continue;

            for (auto adj_adj : _link[*previous][adj])
            {
                _link[*previous][adj_adj].erase(adj);
            }
            _link[*previous][adj].clear();
        }
    }
    // infer direct-link of each block around captured block, then relink it
    set<coord_t> out, except;
    for (auto cap_index : captured)
    {
        for (auto pos_adj : _pos(cap_index)->adj())
        {
            const coord_t adj = pos_adj->index;
            if (!empty(adj))
                continue;

            get_direct_link_union(out, except, adj, previous);
            for (auto link_index : out)
            {
                _link[*previous][adj].insert(link_index);
                _link[*previous][link_index].insert(adj);
            }
            out.clear();
            except.clear();
        }
    }

    // handle center block
    get_direct_link_union(out, except, center, previous);
    for (auto link_index : out)
    {
        _link[*previous][center].insert(link_index);
        _link[*previous][link_index].insert(center);
    }
    out.clear();
    except.clear();

    // ******************** color of opposite owner ********************
    // handle center block
    get_direct_link_union(out, except, center, !previous);
    for (auto link_index : out)
    {
        _link[*!previous][center].insert(link_index);
        _link[*!previous][link_index].insert(center);
    }

}

template<typename Test, coord_t size>
std::ostream& operator<< (std::ostream& stream, BoardT<Test, size> b)
{
    using namespace std;
    stream << typeid(b).name() << __func__ << endl;
    stream << b.debug_state_str();
    stream << b.debug_link_str();
    stream << b.debug_bit_str();
    return stream;
}

}
