#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "crop/idp/utils/SubsetUtils.h"

#include <bitset>
using namespace std;

void assertArrayEqual(int* source, int* expect, int length) {
    for(int i = 0; i < length; i++) {
        ASSERT_EQ(source[i], expect[i]);
    }
}

TEST(SubSetUtils, firstKSubset) {
    KSubset* subset = new KSubset(4, 10);
    assertArrayEqual(subset->set,
                     new int[]{0, 1, 2, 3},
                     4
                     );
}

TEST(SubSetUtils, kSubsetLexSuccessor) {
    KSubset* subset = new KSubset(3, 5);

    int expected[10][3] = {
            {0, 1, 2},
            {0, 1, 3},
            {0, 1, 4},
            {0, 2, 3},
            {0, 2, 4},
            {0, 3, 4},
            {1, 2, 3},
            {1, 2, 4},
            {1, 3, 4},
            {2, 3, 4}
    };

    for (int i = 0; i < 10; i++) {
        assertArrayEqual(subset->set, expected[i], 3);
        subset->lexSuccessor();
    }

    EXPECT_EQ(subset->set, nullptr);
}

TEST(SubSetUtils, mask) {
    KSubset* subset1 = new KSubset(
            100,
            new int[]{},
            0
    );
    KSubset* subset2 = new KSubset(
            100,
            new int[]{0},
            1
    );
    KSubset* subset3 = new KSubset(
            100,
            new int[]{0, 1, 2},
            3
    );
    KSubset* subset4 = new KSubset(
            100,
            new int[]{0, 3, 8, 9},
            4
    );

    EXPECT_EQ(subset1->mask(), bitset<100>(0));
    EXPECT_EQ(subset2->mask(), bitset<100>(1));
    EXPECT_EQ(subset3->mask(), bitset<100>(0b111));
    EXPECT_EQ(subset4->mask(), bitset<100>(0b1100001001));
}

TEST(SubSetUtils, maskWithCollection) {
    int* c = new int[]{ 3, 2, 1, 4, 8, 5, 6, 7, 0 };

    KSubset* subset1 = new KSubset(
            100,
            new int[]{},
            0
    );
    KSubset* subset2 = new KSubset(
            100,
            new int[]{0},
            1
    );
    KSubset* subset3 = new KSubset(
            100,
            new int[]{0, 1, 2},
            3
    );
    KSubset* subset4 = new KSubset(
            100,
            new int[]{0, 3, 7, 8},
            4
    );

    EXPECT_EQ(subset1->maskWithCollection(c), bitset<100>(0));
    EXPECT_EQ(subset2->maskWithCollection(c), bitset<100>(0b1000));
    EXPECT_EQ(subset3->maskWithCollection(c), bitset<100>(0b1110));
    EXPECT_EQ(subset4->maskWithCollection(c), bitset<100>(0b10011001));
}

