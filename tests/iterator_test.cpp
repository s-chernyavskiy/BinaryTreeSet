#include <lib/iterator/bst_iterator.hpp>

#include <gtest/gtest.h>

TEST(ITERATOR_INIT, EMPTY_CONSTRUCTOR) {
  EXPECT_NO_FATAL_FAILURE(bst_iterator<int> lhs);
}

TEST(ITERATOR_INIT, VALUE_CONSTRUCTOR) {
  int a = 4;
  int* b = &a;
  bst_iterator<int> lhs(b);
  EXPECT_EQ(*lhs, 4);
}

TEST(ITERATOR_INIT, OTHER_ITERATOR_CONSTRUCTOR) {
  int a = 4;
  int* b = &a;
  bst_iterator<int> lhs(b);
  bst_iterator<int> rhs(lhs);
  EXPECT_EQ(*rhs, *lhs);
}

TEST(ITERATOR_BASIC_OPERATORS, EQUALITY_OPERATOR) {
  int a = 4;
  int* b = &a;
  bst_iterator<int> lhs(b);
  bst_iterator<int> rhs(b);
  EXPECT_TRUE(lhs == rhs);
}

TEST(ITERATOR_BASIC_OPERATORS, INEQUALITY_OPERATOR) {
  int a = 4;
  int d = 6;
  int* b = &a;
  int* c = &d;
  bst_iterator<int> lhs(b);
  bst_iterator<int> rhs(c);
  EXPECT_TRUE(lhs != rhs);
}

TEST(ITERATOR_BASIC_OPERATORS, GREATER_THAN_OPERATOR) {
  int a = 4;
  int d = 6;
  int* b = &a;
  int* c = &d;
  bst_iterator<int> lhs(b);
  bst_iterator<int> rhs(c);
  EXPECT_TRUE(rhs > lhs);
}

TEST(ITERATOR_BASIC_OPERATORS, GREATER_THAN_OR_EQUAL_TO_OPERATOR) {
  int a = 4;
  int d = 6;
  int* b = &a;
  int* c = &d;
  bst_iterator<int> lhs(b);
  bst_iterator<int> rhs(c);
  EXPECT_TRUE(rhs >= lhs);
}

TEST(ITERATOR_BASIC_OPERATORS, LESS_THAN_OPERATOR) {
  int a = 4;
  int d = 6;
  int* b = &a;
  int* c = &d;
  bst_iterator<int> lhs(b);
  bst_iterator<int> rhs(c);
  EXPECT_TRUE(lhs < rhs);
}

TEST(ITERATOR_BASIC_OPERATORS, LESS_THAN_OR_EQUAL_TO_OPERATOR) {
  int a = 4;
  int d = 6;
  int* b = &a;
  int* c = &d;
  bst_iterator<int> lhs(b);
  bst_iterator<int> rhs(c);
  EXPECT_TRUE(lhs <= rhs);
}
