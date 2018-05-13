#include <set>
#include <gtest/gtest.h>
#include <fhutils/logger.h>
#include <fhutils/board.h>

using namespace std;
using namespace fhutils::logger;
using namespace fhutils::board;

TEST(fhutils_board, BoardT) {
    shared_ptr<Board<11>> pb = make_shared<Board<11>>();
    auto & b = *pb;
    b(1, 2) = b.color();
    b(10, 9) = b.color();
    EXPECT_EQ(b(1, 2), Color::Red);
    EXPECT_EQ(b(10, 9), Color::Blue);
    msg() << b.debug_bit_str();
}

TEST(fhutils_board, IBoard) {
    IBoard &b_5 = *IBoard::create(5);
    b_5(2, 2) = Color::Red;
    IBoard &copy_b_5 = *b_5.copy();
    EXPECT_EQ(Color::Red, b_5(2, 2));
    EXPECT_EQ(Color::Red, copy_b_5(2, 2));
    EXPECT_EQ(b_5, copy_b_5);
    b_5(3, 3) = Color::Blue;
    EXPECT_NE(b_5, copy_b_5);
    delete &b_5;
    delete &copy_b_5;
}