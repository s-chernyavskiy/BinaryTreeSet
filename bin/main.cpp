#include <iostream>

#include <lib/bst.hpp>

#define f(a) std::cout << (a) << std::endl;

int main() {
  bst<int, int, Postorder> b;
  b.insert(100, 1);
  b.insert(20, 1);
  b.insert(10, 1);
  b.insert(30, 1);
  b.insert(200, 1);
  b.insert(150, 1);
  b.insert(149, 1);
  b.insert(300, 1);

  auto a = b.cbegin();

  bst<int, int, Postorder> c;
  c.insert(100, 1);
  c.insert(20, 1);
  c.insert(10, 1);
  c.insert(30, 1);
  c.insert(200, 1);
  c.insert(150, 1);
  c.insert(149, 1);

  f(b == c)

  return 0;
}