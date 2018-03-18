#pragma once
#include <vector>
#include <mutex>
#include "board.h"
#include "disjointset.h"
#include "iengine.h"

namespace engine
{

// Single-Thread MCTS search engine
class MCTSEngine : public IEngine
{
    struct Node;
public:
    MCTSEngine() = default;
    virtual ~MCTSEngine();
protected:
    virtual position::pos_t calc_ai_move_sync();
private:
    void selection(Node *current);
    void expansion(Node *current);
    color::Color simulation(Node *current);
    void backpropagation(Node *current, const color::Color winner);
private:
    class Node
    {
    public:
        Node(Node *parent, size_t nChildren);
        ~Node();
        void lock() { _mutex.lock(); }
        void unlock() { _mutex.unlock(); }
        unsigned short nChildren { 0 };
        unsigned short nExpanded { 0 };
        unsigned short index { 0 };
        unsigned int cntWin { 0 };
        unsigned int cntTotal { 0 };
        Node *parent;
        std::vector<Node *> children;
    private:
        std::mutex _mutex;
    };
public:
    int _size { 0 };
    size_t _limit { 0 };
    disjointset::IDisjointSet *uf;
};

}
