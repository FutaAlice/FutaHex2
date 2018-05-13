#pragma once
#include <chrono>
#include <vector>
#include <mutex>
#include <fhutils/board.h>
#include <fhutils/iengine.h>

namespace fhcore {

namespace disjointset {
class IDisjointSet;
} // namespace disjointset

namespace mcts {

// Single-Thread MCTS search engine
class MCTSEngine : public fhutils::iengine::IEngine {
    class Node;
public:
    MCTSEngine(std::chrono::seconds timelimit = std::chrono::seconds(60));
    virtual ~MCTSEngine();
protected:
    virtual fhutils::board::pos_t calc_ai_move_sync();
    virtual fhutils::board::pos_t stop_calc_and_return();
private:
    void selection(Node *& current, disjointset::IDisjointSet *uf);
    void expansion(Node *& current, disjointset::IDisjointSet *uf);
    fhutils::board::Color simulation(Node *& current, disjointset::IDisjointSet *uf);
    void backpropagation(Node *& current, disjointset::IDisjointSet *uf,
        const fhutils::board::Color winner);
private:
    class Node {
    public:
        Node(Node *parent, size_t nChildren);
        ~Node();
        void lock() { _mutex.lock(); }
        void unlock() { _mutex.unlock(); }
        unsigned short nChildren{ 0 };
        unsigned short nExpanded{ 0 };
        unsigned short index{ 0 };
        unsigned int cntWin{ 0 };
        unsigned int cntTotal{ 0 };
        fhutils::board::Color winner{ fhutils::board::Color::Empty };
        Node *parent;
        std::vector<Node *> children;
    private:
        std::mutex _mutex;
    };
private:
    std::chrono::milliseconds _limit;
    size_t _arraysize{ 0 };
};

} // namespace mcts
} // namespace fhcore
