#pragma once

#include <cstddef>
#include <cinttypes>

#include "iterator/iterator.hpp"

template<class Key, class Value, class Traversal = Preorder,
    class Alloc = std::allocator<std::tuple<Key, Value, Traversal>>>
class bst {
 private:
  struct Node {
    Key key;
    Value value;

    Node* left;
    Node* right;
    Node* parent;
    bool visited = false;
    Node(Key key, Value value) : key(key), value(value), left(nullptr), right(nullptr), parent(nullptr) {};
    ~Node() noexcept = default;
    inline bool operator==(const Node& other) { return (key == other.key) && (value == other.value); }
  };

  Node* root;
  Node* last;

  size_t size_ = 0;

  Alloc allocator_;

  void del(Node* current);

  Node* insert_(Node* current, Key key, Value value);
  Node* extract_(Node* current, Key key);
  Node* get_min_(Node* current);
  Node* find_(Node* current, Key key);
 public:
  typedef std::tuple<Key, Value, Traversal> value_type;
  typedef Key reference;
  typedef const Key& const_reference;
  typedef Key* pointer;
  typedef iterator<Node, Traversal> Iterator;
  typedef const iterator<Node, Traversal>& const_iterator;
  typedef Alloc allocator_type;

  static_assert(std::is_same<typename allocator_type::value_type, value_type>::value,
                "bst must have the same value_type as its allocator");
  friend std::ostream& operator<<(std::ostream& os, bst<Key, Value, Traversal>::Node v) {
    os << "{" << v.key << ", " << v.value << "}";
    return os;
  }

  explicit bst() noexcept: root(nullptr), last(nullptr) {}
  bst(const bst& other) : root(nullptr), last(nullptr) {
    *this = other;
  }

  ~bst() { del(root); }

  bst& operator=(const bst& other) noexcept {
    if (this != &other) {
      Node* new_root = cpy(other.root);
      delete root;
      root = new_root;
      last = other.last;
    }
    return *this;
  }

  void insert(Key key, Value value) { root = insert_(root, key, value); };
  void extract(Key key) { root = extract_(root, key); };

  bool contains(Key key) { return find_(root, key) != nullptr; }

  iterator<Node, Traversal> begin() const { return Iterator(root); }
  iterator<Node, Traversal> end() const { return Iterator(last); }

  const_iterator cbegin() const noexcept { return root ? const_iterator(root) : const_iterator(nullptr); }
  const_iterator cend() const noexcept { return last ? const_iterator(last) : const_iterator(nullptr); }

  size_t size() { return size_; }
  bool operator==(const bst& other) const noexcept;
  bool operator!=(const bst& other) const noexcept;
};

template<class Key, class Value, class Traversal, class Alloc>
void bst<Key, Value, Traversal, Alloc>::del(Node* current) {
  if (!current) return;
  del(current->left);
  del(current->right);
  delete current;
}

template<class Key, class Value, class Traversal, class Alloc>
bst<Key, Value, Traversal, Alloc>::Node* bst<Key, Value, Traversal, Alloc>::insert_(bst::Node* current,
                                                                                    Key key,
                                                                                    Value value) {
  if (current == nullptr) {
    last = new Node(key, value);
    ++size_;
    return last;
  } else if (key < current->key) {
    current->left = insert_(current->left, key, value);
    current->left->parent = current;
  } else if (key > current->key) {
    current->right = insert_(current->right, key, value);
    current->right->parent = current;
  }
  return current;
}

template<class Key, class Value, class Traversal, class Alloc>
bst<Key, Value, Traversal, Alloc>::Node* bst<Key, Value, Traversal, Alloc>::get_min_(bst::Node* current) {
  if (current != nullptr && current->left != nullptr) {
    return get_min_(current->left);
  }
  return current;
}

template<class Key, class Value, class Traversal, class Alloc>
bst<Key, Value, Traversal, Alloc>::Node* bst<Key, Value, Traversal, Alloc>::extract_(bst::Node* current, Key key) {
  if (!current) { return current; }
  if (key < current->key) {
    current->left = extract_(current->left, key);
  } else if (key > current->key) {
    current->right = extract_(current->right, key);
  } else if (current->right && current->left) {
    current->key = get_min_(current->right)->key;
    current->right = extract_(current->right, current->key);
  } else {
    if (current->left) {
      current = current->left;
    } else if (current->right) {
      current = current->right;
    } else {
      current = nullptr;
    }
  }

  return current;
}

template<class Key, class Value, class Traversal, class Alloc>
bst<Key, Value, Traversal, Alloc>::Node* bst<Key, Value, Traversal, Alloc>::find_(bst::Node* current, Key key) {
  if (!current || current->key == key) {
    return current;
  }
  if (key < current->key) {
    return find_(current->left, key);
  } else {
    return find_(current->right, key);
  }
}

template<class Key, class Value, class Traversal, class Alloc>
inline bool bst<Key, Value, Traversal, Alloc>::operator==(const bst& other) const noexcept {
  if (size_ != other.size_) {
    return false;
  }
  iterator<Node, Traversal> it_1 = begin();
  iterator<Node, Traversal> it_2 = other.begin();
  for (int i = 0; i < size_; i++) {
    if (*it_1 != *it_2) {
      return false;
    }
    it_1++;
    it_2++;
  }

  return true;
}

template<class Key, class Value, class Traversal, class Alloc>
inline bool bst<Key, Value, Traversal, Alloc>::operator!=(const bst& other) const noexcept {
  return !(this->operator==(other));
}
