#include "lib/iterator/iterator.hpp"

#include <gtest/gtest.h>

TEST(ITERATOR_INIT, EMPTY) {
  EXPECT_NO_FATAL_FAILURE(iterator<int> lhs);
}

