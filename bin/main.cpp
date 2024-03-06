#include <iostream>

#include <lib/bst.hpp>

int main() {
  bst<int, Postorder> a; // {}
  a.insert(2); // {2}
  a.insert(3); // {2, 3}

  bst<int, Inorder> b({1, 2, 3, 4, 5}); // {1, 2, 3, 4, 5}

  bst<std::string, Preorder> c = {"foo", "bar", "baz"}; // {"foo", "bar", "baz"}
  c.insert("bat"); // {"foo", "bar", "baz", "bat"}
  c.extract("bar"); // {"foo", "baz", "bat"}

  std::cout << std::boolalpha << c.contains("bat") << std::endl; // true
  std::cout << std::boolalpha << c.contains("bar") << std::endl; // false

  auto it = c.begin();

  std::cout << (*it) << std::endl; // foo

  ++it;

  std::cout << (*it) << std::endl; // baz

  --it;

  std::cout << (*it) << std::endl; // foo

  return 0;
}