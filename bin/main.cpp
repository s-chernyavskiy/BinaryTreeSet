#include <iostream>

#include <lib/bst.hpp>

int main() {
  bst<int, int, Postorder> a; // {}
  a.insert(std::make_pair(2, 1)); // {{2, 1}}
  a.insert({3, 1}); // {{2, 1}, {3, 1}}
  // inserting both ways is possible

  bst<int, int, Inorder> b({{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}});
  // {{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}}

  bst<std::string, int, Preorder> c = {{"foo", 1}, {"bar", 1}, {"baz", 1}}; // {{"foo", 1}, {"bar", 1}, {"baz", 1}}
  c.insert({"bat", 1}); // {{"foo", 1}, {"bar", 1}, {"baz", 1}, {"bat", 1}}
  c.extract("bar"); // {{"foo", 1}, {"baz", 1}, {"bat", 1}}

  std::cout << std::boolalpha << c.contains("bat") << std::endl; // true
  std::cout << std::boolalpha << c.contains("bar") << std::endl; // false

  auto it = c.begin();

  std::cout << (*it) << std::endl; // {foo, 1}

  ++it;

  std::cout << (*it) << std::endl; // {baz, 1}

  --it;

  std::cout << (*it) << std::endl; // {foo, 1}

  std::cout << c.size() << std::endl; // 3

  c.clear(); // {}

  std::cout << c.size() << std::endl; // 0

  return 0;
}