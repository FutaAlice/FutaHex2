#include <map>
#include "disjointset.h"
using namespace std;
using namespace board;
namespace disjointset
{

#define REGISTER_CREATE_BY_SIZE(container, size) do { \
    container[size] = DisjointSet<size>::create; \
} while (0)

typedef IDisjointSet *(*PF_CREATE)(IBoard *);
static map<size_t, PF_CREATE> s2t;

static struct initializer
{
    initializer();
    ~initializer() {};
} dummy;

initializer::initializer()
{
    REGISTER_CREATE_BY_SIZE(s2t, 4);
    REGISTER_CREATE_BY_SIZE(s2t, 5);
    REGISTER_CREATE_BY_SIZE(s2t, 6);
    REGISTER_CREATE_BY_SIZE(s2t, 7);
    REGISTER_CREATE_BY_SIZE(s2t, 8);
    REGISTER_CREATE_BY_SIZE(s2t, 9);
    REGISTER_CREATE_BY_SIZE(s2t, 10);
    REGISTER_CREATE_BY_SIZE(s2t, 11);
    REGISTER_CREATE_BY_SIZE(s2t, 12);
    REGISTER_CREATE_BY_SIZE(s2t, 13);
    REGISTER_CREATE_BY_SIZE(s2t, 14);
    REGISTER_CREATE_BY_SIZE(s2t, 15);
    REGISTER_CREATE_BY_SIZE(s2t, 16);
    REGISTER_CREATE_BY_SIZE(s2t, 17);
    REGISTER_CREATE_BY_SIZE(s2t, 18);
    REGISTER_CREATE_BY_SIZE(s2t, 19);
}

IDisjointSet * IDisjointSet::create(IBoard *pBoard)
{
    auto iter = s2t.find(pBoard->boardsize());
    if (s2t.end() != iter)
    {
        auto func = iter->second;
        return func(pBoard);
    }
    return nullptr;
}

}
