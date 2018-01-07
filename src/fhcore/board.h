/*
Sample for 5x5 board:

                    Red-Begin

            00    01    02    03    04

                05    06    07    08    09         Blue-End

                   10    11    12    13    14

    Blue-Begin        15    16    17    18    19

                          20    21    22    23    24

                                    Red-End
                     
*/

#pragma once
#include <iostream>
#include <bitset>
#include <array>
#include <set>
#include <string>
#include <initializer_list>
#include "color.h"
#include "position.h"

namespace board
{
using namespace color;
using namespace position;

template<typename Test, coord_t size>
class BoardT
{
    friend Test;
    using Position = PositionT<size>;
public:
    BoardT() noexcept;
    BoardT<Test, size>& operator()(coord_t row, coord_t col);
    BoardT<Test, size>& operator()(coord_t index);
    const std::array<std::set<coord_t>, size * size + 2> &
        operator[](Color color) const;
    void operator=(const Color color);
    
    operator Color();
    
    size_t terns() const;
    Color color() const;
    Color winner() const;
    coord_t index() const;
    coord_t index(coord_t row, coord_t col) const;
    std::set<coord_t>::iterator begin(Color color, coord_t index) const
    {
        return _link[&color][index].begin();
    }
    std::set<coord_t>::iterator end(Color color, coord_t index) const
    {
        return _link[&color][index].end();
    }

    std::string debug_state_info() const;
    std::string debug_bit_info() const;
    std::string debug_link_info() const;

private:
    void set_piece(const Color color);
    void reset_piece();

private:
    coord_t _rowBuf { 0 };
    coord_t _colBuf { 0 };
    const Position & _pos { Position::instance() };
    std::bitset<size * size> _bit[2];
    // std::set<coord_t> _link[2][size * size];
    std::array<std::set<coord_t>, size * size + 2> _link[2];
    std::array<std::set<coord_t>, 123> _test[2];
};

template<coord_t size>
using Board = BoardT<int, size>;

}

#include "board.inl"
