#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "crop/idp/utils/BitsetUtils.h"

#include <bitset>
using namespace std;


TEST(Bitset, smallerThan) {
    bitset<100> bitset1(5);
    bitset<100> bitset2(8);

    ASSERT_TRUE(bitset1 < bitset2);
}

TEST(Bitset, increment) {
    bitset<100> bitset1(5);
    incrementBitset(bitset1);

    bitset<100> target(6);
    ASSERT_EQ(bitset1, target);
}
