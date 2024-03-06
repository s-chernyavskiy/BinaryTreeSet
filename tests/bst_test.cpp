#include <lib/bst.hpp>

#include <gtest/gtest.h>

TEST(BST_INIT, EMPTY_CONSTRUCTOR) {
  EXPECT_NO_FATAL_FAILURE(bst<int> a);
}

TEST(BST_INIT, INITIALIZER_LIST_CONSTRUCTOR) {
  EXPECT_NO_FATAL_FAILURE(bst<int> a({1, 2, 3, 4, 5}));
}

TEST(BST_INIT, OTHER_BST_CONSTRUCTOR) {
  bst<int> a({1, 2, 3, 4, 5});
  EXPECT_NO_FATAL_FAILURE(bst<int> b(a));
}

TEST(BST_INIT, OTHER_BST_COPY_INIT) {
  bst<int> a({1, 2, 3, 4, 5});
  EXPECT_NO_FATAL_FAILURE(bst<int> b = a);
}

TEST(BST_OPERATIONS, BEGIN_PREORDER) {
  bst<int, Preorder> b;
  b.insert(100);
  b.insert(20);
  b.insert(10);
  b.insert(200);
  b.insert(150);
  b.insert(300);
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300

  auto a = b.begin();
  bst<int, Preorder> c{100};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}


TEST(BST_OPERATIONS, BEGIN_INORDER) {
  bst<int, Inorder> b;
  b.insert(100);
  b.insert(20);
  b.insert(10);
  b.insert(200);
  b.insert(150);
  b.insert(300);
  //         100
  //
  //     20      200
  //
  //  10      150   300
  //  ^
  // curr
  auto a = b.begin();
  ++a;
  bst<int, Inorder> c{10};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}

TEST(BST_OPERATIONS, BEGIN_POSTORDER) {
  bst<int, Postorder> b;
  b.insert(100);
  b.insert(20);
  b.insert(10);
  b.insert(200);
  b.insert(150);
  b.insert(300);
  //         100
  //
  //     20      200
  //
  //  10      150   300
  //  ^
  // curr
  auto a = b.begin();
  ++a;
  bst<int, Postorder> c{10};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}

TEST(BST_OPERATIONS, END_PREORDER) {
  bst<int, Preorder> b;
  b.insert(100);
  b.insert(20);
  b.insert(10);
  b.insert(200);
  b.insert(150);
  b.insert(300);
  //         100
  //
  //     20      200
  //
  //  10      150    300
  //                  ^
  //                  curr

  auto a = b.end();
  bst<int, Preorder> c{300};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}

TEST(BST_OPERATIONS, END_INORDER) {
  bst<int, Inorder> b;
  b.insert(100);
  b.insert(20);
  b.insert(10);
  b.insert(200);
  b.insert(150);
  b.insert(300);
  //         100
  //
  //     20      200
  //
  //  10      150    300
  //                  ^
  //                  curr

  auto a = b.end();
  bst<int, Inorder> c{300};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}

TEST(BST_OPERATIONS, END_POSTORDER) {
  bst<int, Postorder> b;
  b.insert(100);
  b.insert(20);
  b.insert(10);
  b.insert(200);
  b.insert(150);
  b.insert(300);
  //         100
  //
  //     20      200
  //
  //  10      150    300
  //                  ^
  //                  curr
  auto a = b.end();
  bst<int, Postorder> c{300};
  auto d = c.begin();
  ASSERT_TRUE(a == d);
}

TEST(BST_OPERATIONS, TRAVERSAL_PRECREMENT) {
  bst<int> b;
  b.insert(100);
  b.insert(20);
  b.insert(10);
  b.insert(200);
  b.insert(150);
  b.insert(300);
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300
  bst<int> c{20};
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
  bst<int, Preorder> b;
  b.insert(100);
  b.insert(20);
  b.insert(10);
  b.insert(200);
  b.insert(150);
  b.insert(300);
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300
  bst<int, Preorder> c{20};
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
  bst<int, Preorder> b;
  b.insert(100);
  b.insert(20);
  b.insert(10);
  b.insert(200);
  b.insert(150);
  b.insert(300);
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300
  bst<int, Preorder> c{100};
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
  bst<int, Preorder> b;
  b.insert(100);
  b.insert(20);
  b.insert(10);
  b.insert(200);
  b.insert(150);
  b.insert(300);
  //         100
  //          ^
  //     20  curr    200
  //
  //  10      150   300
  bst<int, Preorder> c{100};
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
  bst<int> a;
  a.insert(2);

  bst<int> c{2};

  ASSERT_TRUE(a.begin() == c.begin());
}

TEST(BST_OPERATIONS, INSERT_INITIALIZER_LIST) {
  bst<int> a;
  a.insert({1, 2, 3});

  bst<int> c{1, 2, 3};

  ASSERT_TRUE(a == c);
}

TEST(BST_OPERATIONS, GET_SIZE) {
  bst<int> a{1, 2, 3, 4, 5};
  ASSERT_EQ(a.size(), 5);
}

TEST(BST_OPERATIONS, EXTRACT_VALUE) {
  bst<int> a{1, 2, 3};
  a.extract(2);
  bst<int> c{1, 3};

  ASSERT_TRUE(a == c);
}

TEST(BST_OPERATIONS, EXTRACT_VALUE_SIZE) {
  bst<int> a{1, 2, 3};
  a.extract(2);

  ASSERT_EQ(a.size(), 2);
}

TEST(BST_OPERATIONS, VALUE_CONTAINS) {
  bst<int> a{1, 2, 3};

  ASSERT_TRUE(a.contains(2));
}

TEST(BST_OPERATIONS, BST_EQUAL) {
  bst<int> a{1, 2, 3};
  bst<int> b{1, 2, 3};
  ASSERT_TRUE(a == b);
}

TEST(BST_OPERATIONS, NOT_EQUAL_SIZE) {
  bst<int> a{1, 2, 3};
  bst<int> b{1, 3};
  ASSERT_TRUE(a != b);
}

TEST(BST_OPERATIONS, NOT_EQUAL_ELEMENTS) {
  bst<int> a{1, 2, 3};
  bst<int> b{1, 2, 4};
  ASSERT_TRUE(a != b);
}

TEST(BST_OPERATIONS, SQUARE_BRACKETS) {
  bst<int> a{1, 2, 3};
  auto b = a[0];
  auto c = a.begin();
  ASSERT_EQ(b, c);
}
