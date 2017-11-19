#include <string>
#include <list>
#include <thread>
#include <logger.h>
#include "fhcore_test.h"
using namespace std;
using namespace logger;

int test_fhcore_all()
{
    if (0 != test_fhcore_logger())
        return -1;

    if (0 != test_fhcore_color())
        return -1;

    if (0 != test_fhcore_position())
        return -1;

    if (0 != test_fhcore_board())
        return -1;

    return 0;
}

int test_fhcore_logger()
{
    debug(Level::Info) << __func__;
    record(Level::Info) << __func__;

    return 0;
}

#include <color.h>
int test_fhcore_color()
{
    using namespace color;

    Color r = Color::Red, b = Color::Blue;
    auto not_r = !r;
    auto not_b = !b;

    auto func_output = [](auto a) {
        debug(Level::Info) << "Type: "<<typeid(a).name() << ", "
            << "Value: " << a;
    };
    func_output(r);
    func_output(not_r);
    func_output(b);
    func_output(not_b);

    if (typeid(Color).hash_code() != typeid(r).hash_code() ||
        typeid(Color).hash_code() != typeid(r).hash_code() ||
        typeid(Color).hash_code() != typeid(r).hash_code() ||
        typeid(Color).hash_code() != typeid(r).hash_code())
    {
        return -1;
    }

    if (r != not_b || b != not_r)
    {
        return -1;
    }

    const char *arr[2] = { "Red", "Blue" };
    auto a_r = arr[&r];

    return 0;
}

#include <board.h>
//class Test;
//
//template<int size>
//using BoardTest = board::BoardT<Test, size>;
//
//class Test
//{
//public:
//    template<int size>
//    void printPos(BoardTest<11> &b)
//    {
//        debug() << b._pos.row << " " << b._pos.col;
//    }
//};
#include <position.h>
int test_fhcore_position()
{
    using namespace position;
    using namespace logger;

    auto& pt_11 = PositionT<11>::instance();
    auto* p_11 = pt_11(4, 4);

    debug() << *p_11;

    auto& pt_5 = PositionT<5>::instance();
    auto* p_5 = pt_5(4, 4);

    debug() << *p_5;
    

    return 0;
}

int test_fhcore_board()
{
    using namespace board;

    // Board<11> _b;
    Board<11> b;

    b(1, 2) = Color::Red;
    b(10, 9) = b.color();

    debug() << b;

    return 0;
}
