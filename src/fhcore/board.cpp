#include <map>
#include <list>
#include "logger.h"
#include "board.h"
using namespace std;
using namespace logger;
namespace board
{

#define REGISTER_CREATE_BY_SIZE(container, size) do { \
    container[size] = Board<size>::create; \
} while (0)

typedef IBoard *(*PF_CREATE)(void);
static map<coord_t, PF_CREATE> s2t;

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

IBoard * IBoard::create(const coord_t size)
{
    auto iter = s2t.find(size);
    if (s2t.end() != iter)
    {
        auto func = iter->second;
        return func();
    }
    return nullptr;
}

IBoard * IBoard::create(const string & name)
{
    return nullptr;
}

bool IBoard::operator==(const IBoard & rhs) const
{
    assert(boardsize() == rhs.boardsize());
    return equal_to(rhs);
}

bool IBoard::operator!=(const IBoard & rhs) const
{
    assert(boardsize() == rhs.boardsize());
    return !equal_to(rhs);
}

}