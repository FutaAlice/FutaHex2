#include <set>
#include <gtest/gtest.h>
#include <fhutils/logger.h>
#include <fhutils/color.h>
#include <fhutils/position.h>

using namespace std;
using namespace fhutils::logger;
using namespace fhutils::color;
using namespace fhutils::position;

TEST(fhutils_position, position_11) {
    auto& inst = PositionT<11>::instance();
    const pos_t *pt = inst(4, 4);
    EXPECT_EQ(11, pt->size);
    EXPECT_EQ(48, pt->index);
    EXPECT_EQ(4, pt->row);
    EXPECT_EQ(4, pt->col);
    EXPECT_FALSE(pt->bAdjBegin[*Color::Red]);
    EXPECT_FALSE(pt->bAdjBegin[*Color::Blue]);
    EXPECT_FALSE(pt->bAdjEnd[*Color::Red]);
    EXPECT_FALSE(pt->bAdjEnd[*Color::Blue]);

    set<coord_t> adjIndex;
    for (auto x : pt->adj())
        adjIndex.insert(x->index);

    EXPECT_EQ(adjIndex.size(), 6);
    EXPECT_NE(adjIndex.end(), adjIndex.find(47));
    EXPECT_NE(adjIndex.end(), adjIndex.find(49));
    EXPECT_NE(adjIndex.end(), adjIndex.find(37));
    EXPECT_NE(adjIndex.end(), adjIndex.find(59));
    EXPECT_NE(adjIndex.end(), adjIndex.find(38));
    EXPECT_NE(adjIndex.end(), adjIndex.find(58));

    msg(Level::Info) << *pt;
}

TEST(fhutils_position, position_5) {
    auto& inst = PositionT<5>::instance();
    const pos_t *pt = inst(4, 4);
    EXPECT_EQ(5, pt->size);
    EXPECT_EQ(24, pt->index);
    EXPECT_EQ(4, pt->row);
    EXPECT_EQ(4, pt->col);
    EXPECT_FALSE(pt->bAdjBegin[*Color::Red]);
    EXPECT_FALSE(pt->bAdjBegin[*Color::Blue]);
    EXPECT_TRUE(pt->bAdjEnd[*Color::Red]);
    EXPECT_TRUE(pt->bAdjEnd[*Color::Blue]);

    set<coord_t> adjIndex;
    for (auto x : pt->adj())
        adjIndex.insert(x->index);

    EXPECT_EQ(adjIndex.size(), 2);
    EXPECT_NE(adjIndex.end(), adjIndex.find(23));
    EXPECT_NE(adjIndex.end(), adjIndex.find(19));

    msg(Level::Info) << *pt;
}
