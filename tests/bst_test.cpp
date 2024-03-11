#include <lib/bst.hpp>

#include <gtest/gtest.h>

TEST(BST_INIT, EMPTY_CONSTRUCTOR) {
  bst<int, int> a;
  EXPECT_NO_FATAL_FAILURE(a);
}

TEST(BST_INIT, INITIALIZER_LIST_CONSTRUCTOR) {
  bst<int, int> a{{1, 2}, {3, 4}};
  EXPECT_NO_FATAL_FAILURE(a);
}

TEST(BST_INIT, OTHER_BST_CONSTRUCTOR) {
  bst<int, int> a({{1, 2}, {3, 4}});
  bst<int, int> b(a);
  EXPECT_NO_FATAL_FAILURE(b);
}

TEST(BST_INIT, OTHER_BST_COPY_INIT) {
  bst<int, int> a({{1, 2}, {3, 4}});
  bst<int, int> b = a;
  EXPECT_NO_FATAL_FAILURE(b);
}

TEST(BST_OPERATIONS, BEGIN_PREORDER) {
  bst<int, int, Preorder> b;
  b.insert({100, 1});
  b.insert({20, 1});
  b.insert({10, 1});
  b.insert({200, 1});
  b.insert({150, 1});
  b.insert({300, 1});
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300

  auto a = b.begin();
  bst<int, int, Preorder> c{{100, 1}};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}


TEST(BST_OPERATIONS, BEGIN_INORDER) {
  bst<int, int, Inorder> b;
  b.insert({100, 1});
  b.insert({20, 1});
  b.insert({10, 1});
  b.insert({200, 1});
  b.insert({150, 1});
  b.insert({300, 1});
  //         100
  //
  //     20      200
  //
  //  10      150   300
  //  ^
  // curr
  auto a = b.begin();
  ++a;
  bst<int, int, Inorder> c{{10, 1}};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}

TEST(BST_OPERATIONS, BEGIN_POSTORDER) {
  bst<int, int, Postorder> b;
  b.insert({100, 1});
  b.insert({20, 1});
  b.insert({10, 1});
  b.insert({200, 1});
  b.insert({150, 1});
  b.insert({300, 1});
  //         100
  //
  //     20      200
  //
  //  10      150   300
  //  ^
  // curr
  auto a = b.begin();
  ++a;
  bst<int, int, Postorder> c{{10, 1}};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}

TEST(BST_OPERATIONS, END_PREORDER) {
  bst<int, int, Preorder> b;
  b.insert({100, 1});
  b.insert({20, 1});
  b.insert({10, 1});
  b.insert({200, 1});
  b.insert({150, 1});
  b.insert({300, 1});
  //         100
  //
  //     20      200
  //
  //  10      150    300
  //                  ^
  //                  curr

  auto a = b.end();
  bst<int, int, Preorder> c{{300, 1}};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}

TEST(BST_OPERATIONS, END_INORDER) {
  bst<int, int, Inorder> b;
  b.insert({100, 1});
  b.insert({20, 1});
  b.insert({10, 1});
  b.insert({200, 1});
  b.insert({150, 1});
  b.insert({300, 1});
  //         100
  //
  //     20      200
  //
  //  10      150    300
  //                  ^
  //                  curr

  auto a = b.end();
  bst<int, int, Inorder> c{{300, 1}};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}

TEST(BST_OPERATIONS, END_POSTORDER) {
  bst<int, int, Postorder> b;
  b.insert({100, 1});
  b.insert({20, 1});
  b.insert({10, 1});
  b.insert({200, 1});
  b.insert({150, 1});
  b.insert({300, 1});
  //         100
  //
  //     20      200
  //
  //  10      150    300
  //                  ^
  //                  curr
  auto a = b.end();
  bst<int, int, Postorder> c{{300, 1}};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}

TEST(BST_OPERATIONS, TRAVERSAL_PRECREMENT) {
  bst<int, int> b;
  b.insert({100, 1});
  b.insert({20, 1});
  b.insert({10, 1});
  b.insert({200, 1});
  b.insert({150, 1});
  b.insert({300, 1});
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300
  bst<int, int> c{{20, 1}};
  auto a = b.begin();
  ++a;
  //         100
  //
  //     20       200
  //     ^
  //    curr
  //  10      150   300
  ASSERT_EQ(a, c.begin());
}

TEST(BST_OPERATIONS, TRAVERSAL_POSTCREMENT) {
  bst<int, int, Preorder> b;
  b.insert({100, 1});
  b.insert({20, 1});
  b.insert({10, 1});
  b.insert({200, 1});
  b.insert({150, 1});
  b.insert({300, 1});
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300
  bst<int, int, Preorder> c{{20, 1}};
  auto a = b.begin();
  a++;
  //         100
  //
  //     20       200
  //     ^
  //    curr
  //  10      150   300
  ASSERT_EQ(a, c.begin());
}

TEST(BST_OPERATIONS, TRAVERSAL_PREDECREMENT) {
  bst<int, int, Preorder> b;
  b.insert({100, 1});
  b.insert({20, 1});
  b.insert({10, 1});
  b.insert({200, 1});
  b.insert({150, 1});
  b.insert({300, 1});
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300
  bst<int, int, Preorder> c{{100, 1}};
  auto a = b.begin();
  ++a;
  //         100
  //
  //     20       200
  //     ^
  //    curr
  //  10      150   300
  --a;
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300
  ASSERT_EQ(a, c.begin());
}

TEST(BST_OPERATIONS, TRAVERSAL_POSTDECREMENT) {
  bst<int, int, Preorder> b;
  b.insert({100, 1});
  b.insert({20, 1});
  b.insert({10, 1});
  b.insert({200, 1});
  b.insert({150, 1});
  b.insert({300, 1});
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300
  bst<int, int, Preorder> c{{100, 1}};
  auto a = b.begin();
  ++a;
  //         100
  //
  //     20       200
  //     ^
  //    curr
  //  10      150   300
  --a;
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300
  ASSERT_EQ(a, c.begin());
}

TEST(BST_OPERATIONS, INSERT_SINGLE_VALUE) {
  bst<int, int> a;
  a.insert({2, 1});

  bst<int, int> c{{2, 1}};

  ASSERT_TRUE(a.begin() == c.begin());
}

TEST(BST_OPERATIONS, INSERT_INITIALIZER_LIST) {
  bst<int, int> a;
  a.insert({{1, 2}, {3, 4}});

  bst<int, int> c{{1, 2}, {3, 4}};

  ASSERT_TRUE(a == c);
}

TEST(BST_OPERATIONS, GET_SIZE) {
  bst<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  ASSERT_EQ(a.size(), 5);
}

TEST(BST_OPERATIONS, EXTRACT_VALUE) {
  bst<int, int> a{{1, 1}, {2, 3}};
  a.extract(2);
  bst<int, int> c{{1, 1}};

  ASSERT_TRUE(a == c);
}

TEST(BST_OPERATIONS, EXTRACT_VALUE_SIZE) {
  bst<int, int> a{{1, 1}, {2, 3}};
  a.extract(2);

  ASSERT_EQ(a.size(), 1);
}

TEST(BST_OPERATIONS, VALUE_CONTAINS) {
  bst<int, int> a{{1, 1}, {2, 3}};

  ASSERT_TRUE(a.contains(2));
}

TEST(BST_OPERATIONS, BST_EQUAL) {
  bst<int, int> a{{1, 1}, {2, 3}};
  bst<int, int> b{{1, 1}, {2, 3}};
  ASSERT_TRUE(a == b);
}

TEST(BST_OPERATIONS, NOT_EQUAL_SIZE) {
  bst<int, int> a{{1, 1}, {2, 3}};
  bst<int, int> b{{1, 1}};
  ASSERT_TRUE(a != b);
}

TEST(BST_OPERATIONS, NOT_EQUAL_ELEMENTS) {
  bst<int, int> a{{1, 1}, {2, 3}};
  bst<int, int> b{{1, 1}, {2, 4}};
  ASSERT_TRUE(a != b);
}

TEST(BST_OPERATIONS, SQUARE_BRACKETS) {
  bst<int, int> a{{1, 1}, {2, 3}};
  auto b = a[0];
  auto c = a.begin();
  ASSERT_EQ(b, c);
}
