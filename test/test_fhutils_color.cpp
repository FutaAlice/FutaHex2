#include <gtest/gtest.h>
#include <fhutils/logger.h>
#include <fhutils/color.h>

using namespace fhutils::logger;
using namespace fhutils::color;

TEST(fhutils_color, color) {
    EXPECT_EQ(0, *Color::Red);
    EXPECT_EQ(1, *Color::Blue);
    EXPECT_EQ(-1, *Color::Empty);
    EXPECT_EQ(Color::Blue, !Color::Red);
    EXPECT_EQ(Color::Red, !Color::Blue);
    msg(Level::Info) << Color::Red;
    msg(Level::Info) << Color::Blue;
    msg(Level::Info) << Color::Empty;
}
