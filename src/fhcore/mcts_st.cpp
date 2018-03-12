#include <future>
#include <random>
#include "board.h"
#include "logger.h"
#include "mcts_st.h"

using namespace std;
using namespace board;

namespace engine
{

MCTS_ST::MCTS_ST()
{
}

MCTS_ST::~MCTS_ST()
{
    wait();
}

pos_t MCTS_ST::calc_ai_move_sync()
{
    this_thread::sleep_for(chrono::seconds(1));
    return pos_t(1, 1, 1);
}

}
