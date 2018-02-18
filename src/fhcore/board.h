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
#include <array>
#include <bitset>
#include <set>
#include <string>
#include <tuple>
#include <initializer_list>
#include "color.h"
#include "position.h"

namespace board
{
using namespace color;
using namespace position;

class IBoard
{
public:
    virtual IBoard & operator()(coord_t row, coord_t col) = 0;
    virtual IBoard & operator()(coord_t index) = 0;

    virtual std::tuple<const std::set<coord_t> *, size_t>
        operator[](const Color color) const = 0;
    virtual void operator=(const Color color) = 0;
    virtual operator Color() const = 0;

    virtual size_t boardsize() const = 0;
    virtual size_t terns() const = 0;
    virtual Color color() const = 0;
    virtual Color winner() const = 0;

    virtual std::set<coord_t>::iterator
        begin(const Color color, coord_t index) const = 0;
    virtual std::set<coord_t>::iterator
        end(const Color color, coord_t index) const = 0;

    virtual std::string debug_state_str() const = 0;
    virtual std::string debug_bit_str() const = 0;
    virtual std::string debug_link_str() const = 0;
};

template<typename Test, coord_t size>
class BoardT : public IBoard
{
    friend Test;
    using Position = PositionT<size>;
public:
    BoardT() noexcept;

public: // Interface
    virtual IBoard & operator()(coord_t row, coord_t col);
    virtual IBoard & operator()(coord_t index);

    virtual std::tuple<const std::set<coord_t> *, size_t>
        operator[](const Color color) const;
    virtual void operator=(const Color color);
    virtual operator Color() const;

    virtual size_t boardsize() const;
    virtual size_t terns() const;
    virtual Color color() const;
    virtual Color winner() const;

    virtual std::set<coord_t>::iterator
        begin(const Color color, coord_t index) const;
    virtual std::set<coord_t>::iterator
        end(const Color color, coord_t index) const;

    virtual std::string debug_state_str() const;
    virtual std::string debug_bit_str() const;
    virtual std::string debug_link_str() const;

private:
    coord_t buf_index() const;
    void set_piece(const Color color);
    void reset_piece();

private:
    coord_t _rowBuf { 0 };
    coord_t _colBuf { 0 };
    const Position & _pos { Position::instance() };
    std::bitset<size * size> _bit[2];
    std::array<std::set<coord_t>, size * size + 2> _link[2];
};

template<coord_t size>
using Board = BoardT<int, size>;

}

#include "board.inl"
