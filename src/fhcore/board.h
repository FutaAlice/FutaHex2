#pragma once
#include <iostream>
#include <bitset>
#include <set>
#include <initializer_list>
#include "color.h"
#include "position.h"

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

namespace board
{
using namespace color;
using namespace position;

template<typename Test, coord_t size>
class BoardT
{
    friend Test;
public:
    BoardT() noexcept;
    BoardT<Test, size>& operator()(coord_t row, coord_t col);
    void operator=(const Color color);
    
    operator Color();
    
    size_t terns() const;
    Color color() const;
    Color winner() const;

public:
    static const coord_t nBegin { size };
    static const coord_t nEnd { size + 1 };
private:
    static void check_boardsize();
private:
    std::bitset<size * size> _bit[2];
    std::set<coord_t> _link[2][size * size];
    coord_t _rowBuf { 0 };
    coord_t _colBuf { 0 };
};

template<coord_t size>
using Board = BoardT<int, size>;

}

#include "board.inl"
