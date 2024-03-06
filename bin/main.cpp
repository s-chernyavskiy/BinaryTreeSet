#include <iostream>

#include <lib/bst.hpp>

#define f(a) std::cout << (a) << std::endl;

int main() {
//  bst<int, Postorder> b;
//  b.insert(100);
//  b.insert(20);
//  b.insert(10);
//  b.insert(30);
//  b.insert(200);
//  b.insert(150);
//  b.insert(149);
//  b.insert(300);
  bst<int> c({1, 2, 3, 4, 5});
  auto d = c.begin();
  f(*d)
  ++d;
  f(*d)
  bst<int> a({1, 2, 3, 4});
  auto b = a.begin();
  f(*b)
  ++b;
  f(*b)
  return 0;
}