#include <future>
#include <random>
#include "board.h"
#include "logger.h"
#include "mcts.h"

using namespace std;
using namespace board;
using namespace logger;

namespace engine
{

MCTSEngine::MCTSEngine()
{
}

MCTSEngine::~MCTSEngine()
{
    debug(Level::Info) << "release engine <" << __func__ << ">";
    wait();
}

pos_t MCTSEngine::calc_ai_move_sync()
{
    this_thread::sleep_for(chrono::seconds(1));
    return pos_t(1, 1, 1);
}

}
