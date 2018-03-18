#include "board.h"
#include "disjointset.h"

namespace disjointset
{

template<typename Test, coord_t size>
inline DisjointSetT<Test, size>::DisjointSetT(board::IBoard * pBoard)
    : pBoard(pBoard)
{
}

template<typename Test, coord_t size>
inline IDisjointSet * DisjointSetT<Test, size>::create(board::IBoard *pBoard)
{
    return new DisjointSetT(pBoard);
}

template<typename Test, coord_t size>
inline color::Color DisjointSetT<Test, size>::get(int index)
{
    return status[index];
}

template<typename Test, coord_t size>
inline void DisjointSetT<Test, size>::set(int index)
{
    status[index] = color_to_move();
    ++rounds;
}

template<typename Test, coord_t size>
inline color::Color & DisjointSetT<Test, size>::operator[](int index)
{
    return status[index];
}

template<typename Test, coord_t size>
inline void DisjointSetT<Test, size>::revert()
{
    rounds = pBoard->rounds();
    for (int i = 0; i < size * size; ++i)
    {
        status[i] = (*pBoard)(i);
    }
    for (int i = 0; i < size * size + 2; ++i)
    {
        fa[*Color::Red][i] = i;
        fa[*Color::Blue][i] = i;
    }
}

template<typename Test, coord_t size>
inline void DisjointSetT<Test, size>::ufinit()
{
    for (int center_index = 0; center_index < size * size; ++center_index)
    {
        if (Color::Empty != status[center_index])
            merge(center_index, status[center_index]);
    }
}

template<typename Test, coord_t size>
inline bool DisjointSetT<Test, size>::check_after_set(int center_index, const Color color)
{
    merge(center_index, color);
    return find(pBoard->nBegin(), color) == find(pBoard->nEnd(), color);
}

template<typename Test, coord_t size>
inline int DisjointSetT<Test, size>::find(int i, const Color color)
{
    while (i != fa[*color][i])
        i = fa[*color][i];
    return i;
}

template<typename Test, coord_t size>
inline void DisjointSetT<Test, size>::merge(int center_index, Color color)
{
    const pos_t &center_pos = *_pos(center_index);
    for (auto adj_pos : center_pos.adj())
    {
        const auto adj_index = adj_pos->index;
        if (color == status[adj_index])
        {
            int comp_c = find(center_index, color);
            int comp_a = find(adj_index, color);
            if (comp_a != comp_c)
            {
                fa[*color][comp_a] = comp_c;
            }
        }
    }
    if (center_pos.bAdjBegin[*color])
    {
        fa[*color][find(_pos.nBegin, color)] = find(center_index, color);
    }
    if (center_pos.bAdjEnd[*color])
    {
        fa[*color][find(_pos.nEnd, color)] = find(center_index, color);
    }
}

}