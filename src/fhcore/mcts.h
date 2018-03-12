#pragma once
#include "board.h"
#include "disjointset.h"
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
    void selection();
    void expansion();
    color::Color simulation();
    void backpropagation(const color::Color winner);
private:
    typedef struct Node
    {
        unsigned short nChildren { 0 };
        unsigned short nExpanded { 0 };
        unsigned short index { 0 };
        unsigned int cntWin { 0 };
        unsigned int cntTotal { 0 };
        Node *parent;
        Node *children[122] { nullptr };
        Node(Node *parent);
        ~Node();
    } Node;
public:
    int _size { 0 };
    int _limit { 0 };
    Node *current;
    disjointset::IDisjointSet *uf;
};

}
