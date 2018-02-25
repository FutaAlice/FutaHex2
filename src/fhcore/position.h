#pragma once
#include <array>
#include <ostream>

namespace position
{
using coord_t = unsigned short;

class pos_t;
template<coord_t size>
class PositionT;

class pos_t
{
public:
    pos_t(coord_t row, coord_t col, coord_t size);
    const std::array<pos_t *, 6> & adj() const;
    const pos_t *adj(int dir) const;
    void setAdj(int dir, pos_t *adj);
    friend std::ostream& operator<< (std::ostream& stream, const pos_t pt);
public:
    coord_t row;
    coord_t col;
    coord_t size;
    coord_t index;
    bool bAdjBegin[2];
    bool bAdjEnd[2];
private:
    std::array<pos_t *, 6> _adjacent;
};

template<coord_t size>
class PositionT
{
public:
    static const PositionT<size> & instance();
    const pos_t *operator()(coord_t index) const;
    const pos_t *operator()(coord_t row, coord_t col) const;
public:
    static const coord_t nBegin { size * size };
    static const coord_t nEnd { size * size + 1 };
private:
    PositionT();
    ~PositionT();
    PositionT(const PositionT<size> &) = delete;
    PositionT(PositionT<size> &&) = delete;
    PositionT<size> & operator=(const PositionT<size> &) = delete;
private:
    std::array<pos_t *, size * size> _container;
};

}

#include "position.inl"
