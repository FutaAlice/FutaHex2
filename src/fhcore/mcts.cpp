#include <cmath>
#include <future>
#include <iomanip>
#include <memory>
#include <random>
#include <sstream>
#include <typeinfo>
#include "logger.h"
#include "mcts.h"

using namespace std;
using namespace board;
using namespace logger;
using namespace disjointset;

#define BUFFER_SIZE 360

namespace engine
{

MCTSEngine::MCTSEngine()
{
}

MCTSEngine::~MCTSEngine()
{
    debug(Level::Info) << "release engine <" << typeid(*this).name() << ">";
    wait();
}

auto ucb = [](auto xChild, auto nParent, auto nChild) {
    static const double c = 0.9;
    return (double)xChild + c * sqrt(log((double)nParent) / (double)nChild);
};

pos_t MCTSEngine::calc_ai_move_sync()
{
    this_thread::sleep_for(chrono::seconds(1));

    _size = (int)pBoard->boardsize();
    _limit = (int)pow(_size, 2);

    uf = IDisjointSet::create(pBoard);

    auto nChildren = _limit - pBoard->rounds();
    auto root = make_shared<Node>(nullptr, nChildren);

    const size_t times { 3000000 };
    const size_t parts = times / 100;
    int percent = 0;
    for (int i = 0; i < times; ++i)
    {
        if (percent < i / parts)
        {
            percent = i / parts;
            debug(Level::Info) << setw(3) << percent << "% complate, "
                << setw((streamsize)log10(times) + 1) << i << "/" << times;
        }

        current = root.get();
        uf->revert();
        selection();
        uf->ufinit();
        expansion();
        auto winner = simulation();
        backpropagation(winner);
    }

    unsigned int max_cnt = 0;
    int child_index = 0;
    for (int i = 0; i < root->nExpanded; ++i)
    {
        if (max_cnt < root->children[i]->cntTotal)
        {
            max_cnt = root->children[i]->cntTotal;
            child_index = i;
        }
        ostringstream oss;
        oss << setw(3) << root->children[i]->index << ": "
            << setw(7) << root->children[i]->cntWin << "/"
            << setw(7) << root->children[i]->cntTotal << ", "
            << root->children[i]->cntWin * 1.0 / root->children[i]->cntTotal
            << "%";
        debug() << oss.str();
    }

    (*pBoard)(root->children[child_index]->index);
    return pos_t(*pBoard->pos());
}

void MCTSEngine::selection()
{
    while (current->nExpanded == current->nChildren)
    {
        int select_index = 0;
        double max_score = 0;
        for (int i = 0; i < current->nChildren; i++)
        {
            auto winrate = (uf->color_to_move() == colorAI) ?
                (double)current->children[i]->cntWin / current->children[i]->cntTotal :
                1 - (double)current->children[i]->cntWin / current->children[i]->cntTotal;
            auto new_score = ucb(winrate, current->cntTotal, current->children[i]->cntTotal);
            if (max_score < new_score)
            {
                max_score = new_score;
                select_index = i;
            }
        }
        current = current->children[select_index];

        if (0 == current->nChildren)
            break;
        uf->set(current->index);
    }
}

void MCTSEngine::expansion()
{
    if (0 == current->nChildren)
        return;
    auto expanded = current->nExpanded;
    bool buffer[BUFFER_SIZE] = { false };
    for (int i = 0; i < expanded; ++i)
    {
        buffer[current->children[i]->index] = true;
    }
    for (int i = 0; i < _limit; ++i)
    {
        if (!buffer[i] && Color::Empty == uf->get(i))
        {
            current->children[expanded] = new Node(current, current->nChildren - 1);
            current->children[expanded]->index = i;
            current->nExpanded++;
            break;
        }
    }
    current = current->children[expanded];
}

Color MCTSEngine::simulation()
{
    Color color = uf->color_to_move();
    uf->set(current->index);
    if (uf->check_after_set(current->index, color))
    {
        return color;
    }

    int j = 0, k;
    static int alternative[BUFFER_SIZE];
    for (int i = 0; i < _limit; ++i)
    {
        if (uf->get(i) == Color::Empty)
        {
            alternative[j++] = i;
        }
    }

    static auto random = [](double end) {
        return (int)(end * rand() / (RAND_MAX + 1.0));
    };
    int rand_num;
    for (;;)
    {
        k = random(j);
        rand_num = alternative[k];
        alternative[k] = alternative[--j];

        color = uf->color_to_move();
        uf->set(rand_num);
        if (uf->check_after_set(rand_num, color))
        {
            return color;
        }
        assert(j >= 0);
    }
}

void MCTSEngine::backpropagation(const Color winner)
{
    while (current)
    {
        if (colorAI == winner)
            current->cntWin++;
        current->cntTotal++;
        current = current->parent;
    }
}

MCTSEngine::Node::Node(Node * parent, size_t nChildren)
    : parent(parent)
    , nChildren((unsigned short)nChildren)
{
    children.resize(nChildren, nullptr);
}

MCTSEngine::Node::~Node()
{
    for (int i = 0; i < nExpanded; ++i)
    {
        delete children[i];
    }
}

}
