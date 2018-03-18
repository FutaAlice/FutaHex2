#pragma once
#include <string>
#include <tuple>
#include "position.h"
#include "record.h"
namespace hexutils
{

std::string xy2symbol(int row, int col, int size);

uint8_t xy2gamlocate(int row, int col, int size);
std::tuple<int, int> gamlocate2xy(int gamlocate, int size);


}