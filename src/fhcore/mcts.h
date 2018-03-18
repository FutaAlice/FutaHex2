#pragma once
#include <vector>
#include "board.h"
#include "disjointset.h"
#include "iengine.h"

namespace engine
{

// Single-Thread MCTS search engine
class MCTSEngine : public IEngine
{
public:
    MCTSEngine() = default;
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
        std::vector<Node *> children;
        Node(Node *parent, size_t nChildren);
        ~Node();
    } Node;
public:
    int _size { 0 };
    size_t _limit { 0 };
    Node *current;
    disjointset::IDisjointSet *uf;
};

}
