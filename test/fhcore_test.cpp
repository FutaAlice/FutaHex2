#include <string>
#include <list>
#include <thread>
#include <logger.h>
#include <board.h>
#include "fhcore_test.h"

int test_fhcore_all()
{
    if (0 != test_fhcore_logger())
        return -1;

    if (0 != test_fhcore_color())
        return -1;

    if (0 != test_fhcore_position())
        return -1;

    if (0 != test_fhcore_boardt())
        return -1;

    if (0 != test_fhcore_iboard())
        return -1;

    if (0 != test_fhcore_board())
        return -1;

    return 0;
}

int test_fhcore_logger()
{
    using namespace logger;
    debug(Level::Info) << __func__;
    record(Level::Info) << __func__;

    return 0;
}

#include <color.h>
int test_fhcore_color()
{
    using namespace color;
    using namespace logger;

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
    auto a_r = arr[*r];

    return 0;
}

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

int test_fhcore_boardt()
{
    using namespace std;
    using namespace board;
    using namespace logger;

    shared_ptr<Board<11>> pb = make_shared<Board<11>>();
    auto & b = *pb;

    b(1, 2) = Color::Red;
    b(10, 9) = b.color();

    debug() << b.debug_bit_str();

    return 0;
}

int test_fhcore_iboard()
{
    using namespace std;
    using namespace board;
    using namespace logger;

    IBoard &b_5 = *IBoard::create(5);
    b_5(2, 2) = Color::Red;

    IBoard &copy_b_5 = *b_5.copy();
    debug() << copy_b_5.debug_bit_str();

    delete &b_5;
    delete &copy_b_5;

    return 0;
}

int test_fhcore_board()
{
    using namespace std;
    using namespace board;
    using namespace logger;

    Board<5> b_5;
    IBoard &refb = b_5;

    refb(1, 2) = refb.color();
    refb(4, 3) = refb.color();

    // get link state
    auto tp = b_5.operator[](Color::Red);
    auto ps = get<0>(tp);
    auto len = get<1>(tp);

    for_each(ps, ps + len, [](auto v) {
        ostringstream oss;
        for (auto i : v)
        {
            oss << i << " ";
        }
        debug() << oss.str();
    });

    debug() << b_5.debug_bit_str();
    return 0;
}
