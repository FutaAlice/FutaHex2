#include <string>
#include <list>
#include <thread>
#include <logger.h>
#include <board.h>
#include "fhcore_test.h"
using namespace std;
using namespace logger;
using namespace board;

#define TEST_CASE_PASS do { \
    debug(Level::Info) << __func__ << " done."; \
} while (0)

#define TEST_CASE_FAIL do { \
    debug(Level::Info) << __func__ << " fail."; \
} while (0)

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

    if (test_fhcore_iboard_equal_to())
        return -1;

    if (0 != test_fhcore_board())
        return -1;

    if (0 != test_fhcore_efficiency())
        return -1;
    
    return 0;
}

int test_fhcore_logger()
{
    debug(Level::Info) << __func__;
    record(Level::Info) << __func__;

    TEST_CASE_PASS;
    return 0;
}

#include <color.h>
int test_fhcore_color()
{
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

    TEST_CASE_PASS;
    return 0;
}

int test_fhcore_position()
{
    auto& pt_11 = PositionT<11>::instance();
    auto* p_11 = pt_11(4, 4);

    debug() << *p_11;

    auto& pt_5 = PositionT<5>::instance();
    auto* p_5 = pt_5(4, 4);

    debug() << *p_5;
    
    TEST_CASE_PASS;
    return 0;
}

int test_fhcore_boardt()
{
    shared_ptr<Board<11>> pb = make_shared<Board<11>>();
    auto & b = *pb;

    b(1, 2) = Color::Red;
    b(10, 9) = b.color();

    debug() << b.debug_bit_str();

    TEST_CASE_PASS;
    return 0;
}

int test_fhcore_iboard()
{
    IBoard &b_5 = *IBoard::create(5);
    b_5(2, 2) = Color::Red;

    IBoard &copy_b_5 = *b_5.copy();
    debug() << copy_b_5.debug_bit_str();

    delete &b_5;
    delete &copy_b_5;

    TEST_CASE_PASS;
    return 0;
}

int test_fhcore_iboard_equal_to()
{
    auto print_link = [](auto & b1, auto & b2) {
        debug(Level::Info) << b1.debug_link_str();
        debug(Level::Info) << b2.debug_link_str();
    };

    // test case 1: 
    {
        IBoard & b1 = *IBoard::create(5);
        IBoard & b2 = *IBoard::create(5);

        b1(0, 2) = Color::Red;
        b1(0, 2) = Color::Empty;

        if (b1 != b2)
            return -1;
    }

    // test case 2: 
    {
        IBoard & b1 = *IBoard::create(5);
        IBoard & b2 = *IBoard::create(5);

        b1(0, 2) = Color::Red;
        b1(2, 2) = Color::Red;
        b1(0, 2) = Color::Empty;

        b2(2, 2) = Color::Red;

        if (b1 != b2)
            return -1;
    }

    // test case 3: 
    {
        IBoard & b1 = *IBoard::create(5);
        IBoard & b2 = *IBoard::create(5);

        b1(0, 2) = Color::Red;
        b1(1, 2) = Color::Red;
        b1(1, 2) = Color::Empty;

        b2(0, 2) = Color::Red;

        if (b1 != b2)
            return -1;
    }

    // test case 4: 
    {
        IBoard & b1 = *IBoard::create(5);
        IBoard & b2 = *IBoard::create(5);

        b1(0, 2) = Color::Red;
        b1(2, 2) = Color::Red;
        b1(1, 2) = Color::Red;
        b1(1, 2) = Color::Empty;

        b2(0, 2) = Color::Red;
        b2(2, 2) = Color::Red;

        if (b1 != b2)
        {
            TEST_CASE_FAIL;
            return -1;
        }
    }

    TEST_CASE_PASS;
    return 0;
}

int test_fhcore_board()
{
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

    TEST_CASE_PASS;
    return 0;
}

int test_fhcore_efficiency()
{
    using namespace board;
    using namespace logger;

    IBoard & origin = *IBoard::create(11);

    origin(6, 6) = origin.color();
    origin(6, 7) = origin.color();
    origin(5, 6) = origin.color();

    IBoard *copy = nullptr;
    for (size_t i = 0; i < 2000; ++i)
    {
        if (i % 100 == 0)
            debug(Level::Info) << i;
        copy = origin.create(11);
        delete copy;
    }

    return 0;
}
