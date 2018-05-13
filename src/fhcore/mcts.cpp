#include <cmath>
#include <future>
#include <iomanip>
#include <memory>
#include <random>
#include <sstream>
#include <thread>
#include <typeinfo>
#include <vector>
#include <fhutils/logger.h>
#include "disjointset.h"
#include "mcts.h"

using namespace std;
using namespace chrono;
using namespace fhutils::logger;

#define BUFFER_SIZE 360

namespace fhcore {
namespace mcts {

using namespace disjointset;

MCTSEngine::MCTSEngine(std::chrono::seconds timelimit) {
    _limit = duration_cast<milliseconds>(timelimit);
    msg(Level::Info) << "set engine search time: "
        << timelimit.count() << " sec.";
}

MCTSEngine::~MCTSEngine() {
    msg(Level::Info) << "release engine <" << typeid(*this).name() << ">";
    wait();
}

auto ucb = [](auto xChild, auto nParent, auto nChild) {
    static const double c = 0.9;
    return (double)xChild + c * sqrt(log((double)nParent) / (double)nChild);
};

pos_t MCTSEngine::calc_ai_move_sync() {
    this_thread::sleep_for(chrono::seconds(1));

    int size = (int)pBoard->boardsize();
    _arraysize = (int)pow(size, 2);
    if (0 == pBoard->rounds())
        return pos_t(size / 2, size / 2, size);

    auto nChildren = _arraysize - pBoard->rounds();
    auto root = make_shared<Node>(nullptr, nChildren);
    auto nThread = thread::hardware_concurrency();

    auto mcts = [this](auto root, int thread_id) {
        auto union_find_set = IDisjointSet::create(pBoard);
        const auto begin = system_clock::now();
        int percent = 0;
        size_t times = 0;
        for (;;) {
            if (0 == ++times % 1000) {
                auto cost = duration_cast<milliseconds>(system_clock::now() - begin);
                auto rate_of_progress = (int)(cost.count() * 100.0 / _limit.count());
                if (rate_of_progress > percent) {
                    percent = rate_of_progress;
                    msg(Level::Info) << "thread<" << thread_id << ">"
                        << setw(3) << percent << "% complate, "
                        << setw((streamsize)log10(9999999) + 1) << times << " / " << "¡Þ";
                    if (percent >= 100) {
                        delete union_find_set;
                        break;
                    }
                }
            }

            auto current = root;
            union_find_set->revert();
            selection(current, union_find_set);
            union_find_set->ufinit();
            expansion(current, union_find_set);
            auto winner = simulation(current, union_find_set);
            backpropagation(current, union_find_set, winner);
        }
    };
    vector<future<void>> pool;
    for (int i = 0; i < 1; ++i) {
        pool.push_back(async(mcts, root.get(), i));
    }
    for (auto & f : pool)
        f.wait();

    unsigned int max_cnt = 0;
    int child_index = 0;
    for (int i = 0; i < root->nExpanded; ++i) {
        if (max_cnt < root->children[i]->cntTotal) {
            max_cnt = root->children[i]->cntTotal;
            child_index = i;
        }
        ostringstream oss;
        oss << setw(3) << root->children[i]->index << ": "
            << setw(7) << root->children[i]->cntWin << "/"
            << setw(7) << root->children[i]->cntTotal << ", "
            << root->children[i]->cntWin * 1.0 / root->children[i]->cntTotal
            << "%";
        msg() << oss.str();
    }

    (*pBoard)(root->children[child_index]->index);
    return pos_t(*pBoard->pos());
}

pos_t MCTSEngine::stop_calc_and_return() {
    msg(Level::Warning) << "TODO";
    return pos_t();
}

void MCTSEngine::selection(Node *& current, disjointset::IDisjointSet *uf) {
    if (Color::Empty != current->winner)
        return;
    while (current->nExpanded == current->nChildren) {
        int select_index = 0;
        double max_score = 0;
        for (int i = 0; i < current->nChildren; i++) {
            auto winrate = (uf->color_to_move() == colorAI) ?
                (double)current->children[i]->cntWin / current->children[i]->cntTotal :
                1 - (double)current->children[i]->cntWin / current->children[i]->cntTotal;
            auto new_score = ucb(winrate, current->cntTotal, current->children[i]->cntTotal);
            if (max_score < new_score) {
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

void MCTSEngine::expansion(Node *& current, disjointset::IDisjointSet *uf) {
    if (Color::Empty != current->winner || 0 == current->nChildren)
        return;
    auto expanded = current->nExpanded;
    bool buffer[BUFFER_SIZE] = { false };
    for (int i = 0; i < expanded; ++i) {
        buffer[current->children[i]->index] = true;
    }
    for (int i = 0; i < _arraysize; ++i) {
        if (!buffer[i] && Color::Empty == uf->get(i)) {
            current->children[expanded] = new Node(current, current->nChildren - 1);
            current->children[expanded]->index = i;
            current->nExpanded++;
            break;
        }
    }
    current = current->children[expanded];
}

Color MCTSEngine::simulation(Node *& current, disjointset::IDisjointSet *uf) {
    if (Color::Empty != current->winner || 0 == current->nChildren)
        return current->winner;
    Color color = uf->color_to_move();
    uf->set(current->index);
    if (uf->check_after_set(current->index, color)) {
        current->winner = color;
        return color;
    }

    int upper_limit = 0;
    int alternative[BUFFER_SIZE];
    for (int i = 0; i < _arraysize; ++i) {
        if (uf->get(i) == Color::Empty) {
            alternative[upper_limit++] = i;
        }
    }

    static auto random = [](double end) {
        return (int)(end * rand() / (RAND_MAX + 1.0));
    };
    int pos, random_num;
    for (;;) {
        random_num = random(upper_limit);
        pos = alternative[random_num];
        alternative[random_num] = alternative[--upper_limit];

        color = uf->color_to_move();
        uf->set(pos);
        if (uf->check_after_set(pos, color)) {
            return color;
        }
        assert(upper_limit >= 0);
    }
}

void MCTSEngine::backpropagation(Node *& current, disjointset::IDisjointSet *uf, const Color winner) {
    while (current) {
        if (colorAI == winner)
            current->cntWin++;
        current->cntTotal++;
        current = current->parent;
    }
}

MCTSEngine::Node::Node(Node * parent, size_t nChildren)
    : parent(parent)
    , nChildren((unsigned short)nChildren) {
    children.resize(nChildren, nullptr);
}

MCTSEngine::Node::~Node() {
    for (int i = 0; i < nExpanded; ++i) {
        delete children[i];
    }
}

} // namespace mcts
} // namespace fhcore
