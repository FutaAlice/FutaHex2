#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <fhutils/texthelper.h>

using namespace std;
using namespace fhutils::texthelper;

TEST(fhutils_texthelper, toString) {
    EXPECT_STREQ("11", toString(11).c_str());
    EXPECT_STREQ("1.1", toString(1.1f).c_str());
    EXPECT_STREQ("1.1", toString(1.1).c_str());
}

TEST(fhutils_texthelper, fromString) {
    EXPECT_EQ(11, fromString<int>("11"));
    EXPECT_FLOAT_EQ(1.1f, fromString<float>("1.1"));
    EXPECT_DOUBLE_EQ(1.1, fromString<double>("1.1"));
}

TEST(fhutils_texthelper, splitNotEmpty) {
    vector<string> src = { "What", "The", "Fuck" };
    string str;
    for (auto s : src) {
        str += s;
        str += " ";
    }
    auto ret = splitNotEmpty(str, " ");
    ASSERT_EQ(src.size(), ret.size());
    for (size_t i = 0; i < src.size(); ++i) {
        ASSERT_EQ(src[0], ret[0]);
    }
}

TEST(fhutils_texthelper, trimEmpty) {
    EXPECT_STREQ(trim(" FUCK").c_str(), "FUCK");
    EXPECT_STREQ(trim("FUCK ").c_str(), "FUCK");
    EXPECT_STREQ(trim("\nFUCK ").c_str(), "FUCK");
    EXPECT_STREQ(trim(" FUCK\n").c_str(), "FUCK");
}
