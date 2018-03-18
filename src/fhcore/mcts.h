#pragma once
#include <chrono>
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
    class Node;
public:
    MCTSEngine(std::chrono::seconds timelimit = std::chrono::seconds(60));
    virtual ~MCTSEngine();
protected:
    virtual position::pos_t calc_ai_move_sync();
private:
    void selection(Node *& current);
    void expansion(Node *& current);
    color::Color simulation(Node *& current);
    void backpropagation(Node *& current, const color::Color winner);
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
private:
    disjointset::IDisjointSet *_uf { nullptr };
    std::chrono::milliseconds _limit;
    size_t _arraysize { 0 };
};

}
