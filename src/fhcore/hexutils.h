#pragma once
#include <string>
#include <tuple>
#include "position.h"
namespace hexutils
{

std::string xy2symbol(int row, int col, int size);

uint8_t xy2gamlocate(int row, int col, int size);
std::tuple<int, int> gamlocate2xy(int gamlocate, int size);

template<typename Iter>
void new_gam_from_rec(Iter begin, Iter end, size_t boardsize, bool swap,
                      __out unsigned char **pbuffer,
                      __out size_t *buffer_size)
{
    *buffer_size = 128;
    *pbuffer = new unsigned char[128] { 0 };
    auto buffer = *pbuffer;
    size_t i = 0;
    buffer[i++] = 0xff & boardsize; // boardsize
    buffer[i++] = 0xff & (int)swap; // swap rule
    for (auto iter = begin; iter != end; ++iter)
    {
        buffer[i++] = xy2gamlocate(iter->row, iter->col, boardsize);
    }
}

template<typename T>
bool get_rec_from_gam(unsigned char *buffer, size_t buffer_size,
                      __out T & rec, __out size_t & boardsize)
{
    size_t i = 0;
    boardsize = buffer[i++];
    auto swap = buffer[i++];
    if (buffer_size != 128 || boardsize % 2 == 0 ||
        (swap != 0 && swap != 1))
        return false;
    rec.clear();
    for (;;)
    {
        auto gamlocate = buffer[i++];
        if (!gamlocate)
            break;
        auto xy = gamlocate2xy(gamlocate, boardsize);
        auto row = get<0>(xy);
        auto col = get<1>(xy);
        rec.push_back(position::pos_t(row, col, boardsize));
    }
    return true;
}

}