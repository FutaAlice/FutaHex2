#include <cmath>
#include <future>
#include <iomanip>
#include <memory>
#include <random>
#include <sstream>
#include <thread>
#include <typeinfo>
#include <vector>
#include "logger.h"
#include "mcts.h"

using namespace std;
using namespace chrono;
using namespace board;
using namespace logger;
using namespace disjointset;

#define BUFFER_SIZE 360

namespace engine
{

MCTSEngine::MCTSEngine(std::chrono::seconds timelimit)
{
    _limit = duration_cast<milliseconds>(timelimit);
    debug(Level::Info) << "set engine search time: "
        << timelimit.count() << " sec.";
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

    int size = (int)pBoard->boardsize();
    _arraysize = (int)pow(size, 2);
    if (0 == pBoard->rounds())
        return pos_t(size / 2, size / 2, size);

    delete _uf;
    _uf = IDisjointSet::create(pBoard);

    auto nChildren = _arraysize - pBoard->rounds();
    auto root = make_shared<Node>(nullptr, nChildren);
    auto nThread = thread::hardware_concurrency();

    auto mcts = [this](auto root) {
        const auto begin = system_clock::now();
        int percent = 0;
        size_t times = 0;
        for (;;)
        {
            if (0 == ++times % 1000)
            {
                auto cost = duration_cast<milliseconds>(system_clock::now() - begin);
                auto rate_of_progress = (int)(cost.count() * 100.0 / _limit.count());
                if (rate_of_progress > percent)
                {
                    percent = rate_of_progress;
                    debug(Level::Info) << setw(3) << percent << "% complate, "
                        << setw((streamsize)log10(9999999) + 1) << times << " / " << "¡Þ";
                    if (percent == 100)
                        break;
                }
            }

            auto current = root;
            _uf->revert();
            selection(current);
            _uf->ufinit();
            expansion(current);
            auto winner = simulation(current);
            backpropagation(current, winner);
        }
    };
    vector<future<void>> pool;
    for (int i = 0; i < 1; ++i)
    {
        pool.push_back(async(mcts, root.get()));
    }
    for (auto & f : pool)
        f.wait();

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

void MCTSEngine::selection(Node *& current)
{
    while (current->nExpanded == current->nChildren)
    {
        int select_index = 0;
        double max_score = 0;
        for (int i = 0; i < current->nChildren; i++)
        {
            auto winrate = (_uf->color_to_move() == colorAI) ?
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
        _uf->set(current->index);
    }
}

void MCTSEngine::expansion(Node *& current)
{
    if (0 == current->nChildren)
        return;
    auto expanded = current->nExpanded;
    bool buffer[BUFFER_SIZE] = { false };
    for (int i = 0; i < expanded; ++i)
    {
        buffer[current->children[i]->index] = true;
    }
    for (int i = 0; i < _arraysize; ++i)
    {
        if (!buffer[i] && Color::Empty == _uf->get(i))
        {
            current->children[expanded] = new Node(current, current->nChildren - 1);
            current->children[expanded]->index = i;
            current->nExpanded++;
            break;
        }
    }
    current = current->children[expanded];
}

Color MCTSEngine::simulation(Node *& current)
{
    Color color = _uf->color_to_move();
    _uf->set(current->index);
    if (_uf->check_after_set(current->index, color))
    {
        return color;
    }

    int upper_limit = 0;
    static int alternative[BUFFER_SIZE];
    for (int i = 0; i < _arraysize; ++i)
    {
        if (_uf->get(i) == Color::Empty)
        {
            alternative[upper_limit++] = i;
        }
    }

    static auto random = [](double end) {
        return (int)(end * rand() / (RAND_MAX + 1.0));
    };
    int pos, random_num;
    for (;;)
    {
        random_num = random(upper_limit);
        pos = alternative[random_num];
        alternative[random_num] = alternative[--upper_limit];

        color = _uf->color_to_move();
        _uf->set(pos);
        if (_uf->check_after_set(pos, color))
        {
            return color;
        }
        assert(upper_limit >= 0);
    }
}

void MCTSEngine::backpropagation(Node *& current, const Color winner)
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
