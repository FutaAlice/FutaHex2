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
    virtual position::pos_t stop_calc_and_return();
private:
    void selection(Node *& current, disjointset::IDisjointSet *uf);
    void expansion(Node *& current, disjointset::IDisjointSet *uf);
    color::Color simulation(Node *& current, disjointset::IDisjointSet *uf);
    void backpropagation(Node *& current, disjointset::IDisjointSet *uf, const color::Color winner);
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
        color::Color winner { color::Color::Empty };
        Node *parent;
        std::vector<Node *> children;
    private:
        std::mutex _mutex;
    };
private:
    std::chrono::milliseconds _limit;
    size_t _arraysize { 0 };
};

}
