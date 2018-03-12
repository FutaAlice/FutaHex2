#pragma once
#include "iengine.h"

namespace engine
{

// Single-Thread MCTS search engine
class MCTS_ST : public IEngine
{
public:
    MCTS_ST();
    virtual ~MCTS_ST();
protected:
    virtual position::pos_t calc_ai_move_sync();
private:
};

}
