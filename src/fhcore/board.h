#pragma once
#include <ostream>
#include <bitset>
#include <set>
#include <initializer_list>
#include "color.h"

namespace board
{
using namespace color;
using coord_t = unsigned short;

template<typename Test, int size>
class BoardT
{
    friend Test;
public:
    BoardT() noexcept;
    BoardT<Test, size>& operator()(int row, int col);
    void operator=(const Color color);
    
    operator Color();

private:
    static void check_boardsize();
private:
    std::bitset<size * size> _bit[2];
    std::set<coord_t> _link[2];
    coord_t _rowBuf { 0 };
    coord_t _colBuf { 0 };
};

template<int size>
using Board = BoardT<int, size>;

}

#include "board.inl"
