#pragma once
#include "iengine.h"

namespace engine
{

// Single-Thread MCTS search engine
class MCTSEngine : public IEngine
{
public:
    MCTSEngine();
    virtual ~MCTSEngine();
protected:
    virtual position::pos_t calc_ai_move_sync();
private:
};

}
