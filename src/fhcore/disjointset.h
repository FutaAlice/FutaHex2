#pragma once
#include "position.h"
#include "color.h"

using namespace position;
using namespace color;
namespace board
{
class IBoard;
}

namespace disjointset
{

class IDisjointSet
{
public:
    virtual ~IDisjointSet() = default;
    static IDisjointSet * create(board::IBoard *pBoard);

public:
    virtual Color get(int index) = 0;
    virtual void set(int index) = 0;
    virtual Color & operator[](int index) = 0;
    virtual void revert() = 0;
    virtual void ufinit() = 0;
    virtual bool check_after_set(int center_index, const Color color) = 0;
public:
    Color color_to_move() { return static_cast<Color>(rounds & 1); }
protected:
    size_t rounds;
};

template<typename Test, coord_t size>
class DisjointSetT : public IDisjointSet
{
    friend Test;
    using Position = PositionT<size>;
public:
    DisjointSetT() = default;
    DisjointSetT(board::IBoard *pBoard);
public:
    static IDisjointSet * create(board::IBoard *pBoard);
public:
    virtual Color get(int index);
    virtual void set(int index);
    virtual Color & operator[](int index);
    virtual void revert();
    virtual void ufinit();
    virtual bool check_after_set(int center_index, const Color color);

private:
    int find(int i, const Color color);
    void merge(int center_index, Color color);

public:
    Color status[size * size];

private:
    const Position & _pos{ Position::instance() };
    int fa[2][size * size + 2];
    board::IBoard *pBoard;
};

template<coord_t size>
using DisjointSet = DisjointSetT<int, size>;

}

#include "disjointset.inl"