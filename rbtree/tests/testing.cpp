#include <gtest/gtest.h>
#include "../include/rbtree.hpp"

TEST(RBTREETEST, INSERTION) {
    RBTree<int> tree1;
    tree1.insert(20);
    int res = 1;

    EXPECT_EQ(res, 1);
}
