#pragma once

#include <cstddef>
#include <cinttypes>

#include <lib/iterator/bst_iterator.hpp>

template<class Key, class Value, class Traversal = Preorder, class Alloc = std::allocator<std::pair<Key, Value>>>
class bst {
 private:
  size_t size_ = 0;
  Alloc allocator_{};

  struct Node {
    std::pair<Key, Value> value;

    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    bool visited = false;
    explicit Node(std::pair<Key, Value> value) : value(value), left(nullptr), right(nullptr), parent(nullptr), visited(false) {};
    ~Node() noexcept {
      left = nullptr;
      right = nullptr;
      parent = nullptr;
      visited = false;
    };
    bool operator==(const Node& other) const { return (value == other.value); }
  };

  Node* root_ = nullptr;
  Node* last_ = nullptr;

  void del_(Node* current);
  Node* insert_(Node* current, std::pair<Key, Value> value);
  Node* extract_(Node* current, Key value);
  Node* get_min_(Node* current);
  Node* get_max_(Node* current);
  Node* find_(Node* current, Key value);
  Node* copy(Node* other, Node* parent = nullptr);
 public:
  using allocator_type = typename std::allocator_traits<Alloc>::template rebind_alloc<std::pair<Key, Value>>;
  using allocator_traits = typename std::allocator_traits<Alloc>::template rebind_traits<std::pair<Key, Value>>;

  using iterator = bst_iterator<Node, Traversal>;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  using key_type = Key;
  using mapped_type = Value;

  using size_type = std::size_t;
  using node_type = Node;

  using value_type = std::pair<Key, Value>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = std::allocator_traits<Alloc>::pointer;
  using const_pointer = std::allocator_traits<Alloc>::const_pointer;

  using difference_type = iterator::difference_type;

  static_assert(std::is_same<typename allocator_type::value_type, value_type>::value,
                "bst must have the same value_type as its allocator");

  friend std::ostream& operator<<(std::ostream& os, bst<Key, Value, Traversal>::Node v) {
    os << '{' << v.value.first << ", " << v.value.second << '}';
    return os;
  }

  explicit bst() noexcept: root_(nullptr), last_(nullptr) {}
  bst(std::initializer_list<value_type> initializer_list);
  bst(const bst& other) : root_(nullptr), last_(nullptr) { *this = other; }
  ~bst() { del_(root_); }

  bst& operator=(const bst& other) noexcept {
    if (this != &other) {
      Node* new_root = copy(other.root_);
      delete root_;
      root_ = new_root;
      last_ = other.last_;
      size_ = other.size_;
    }
    return *this;
  }

  void insert(value_type value) { root_ = insert_(root_, value); };
  void insert(std::initializer_list<value_type> initializer_list);
  void extract(key_type value) { root_ = extract_(root_, value); if (root_ == nullptr) last_ == nullptr; };

  bool contains(key_type value) { return find_(root_, value) != nullptr; }

  iterator begin() const { return iterator(root_); }
  iterator end() const { return iterator(last_); }

  const_iterator cbegin() const noexcept { return root_ ? const_iterator(root_) : const_iterator(nullptr); }
  const_iterator cend() const noexcept { return last_ ? const_iterator(last_) : const_iterator(nullptr); }

  reverse_iterator rbegin() const noexcept { return last_ ? reverse_iterator(last_) : reverse_iterator(nullptr); }
  reverse_iterator rend() const noexcept { return root_ ? reverse_iterator(root_) : reverse_iterator(nullptr); }

  const_reverse_iterator crbegin() const noexcept { return last_ ? const_reverse_iterator(last_) : const_reverse_iterator(nullptr); }
  const_reverse_iterator crend() const noexcept { return root_ ? const_reverse_iterator(root_) : const_reverse_iterator(nullptr); }

  void clear();

  size_t size() { return size_; }
  bool operator==(const bst& other) const noexcept;
  bool operator!=(const bst& other) const noexcept;

  void swap(bst& other);
  iterator operator[](size_t i);
  const_iterator operator[](size_t i) const;
};

template<class Key, class Value, class Traversal, class Alloc>
void bst<Key, Value, Traversal, Alloc>::clear() {
  size_t bst_size = size_;
  for (size_t i = 0; i < bst_size; i++) {
    extract((*operator[](0)).value.first);
  }
}

template<class Key, class Value, class Traversal, class Alloc>
bst<Key, Value, Traversal, Alloc>::bst(std::initializer_list<value_type> initializer_list) {
  insert(initializer_list);
}

template<class Key, class Value, class Traversal, class Alloc>
void bst<Key, Value, Traversal, Alloc>::insert(std::initializer_list<value_type> initializer_list) {
  for (auto item : initializer_list) {
    insert(item);
  }
}

template<class Key, class Value, class Traversal, class Alloc>
bst<Key, Value, Traversal, Alloc>::iterator bst<Key, Value, Traversal, Alloc>::operator[](size_t i) {
  return iterator(begin() + i);
}

template<class Key, class Value, class Traversal, class Alloc>
bst<Key, Value, Traversal, Alloc>::const_iterator bst<Key, Value, Traversal, Alloc>::operator[](size_t i) const {
  return iterator(cbegin() + i);
}

template<class Key, class Value, class Traversal, class Alloc>
void bst<Key, Value, Traversal, Alloc>::del_(Node* current) {
  if (!current) return;
  del_(current->left);
  del_(current->right);
  delete current;
}

template<class Key, class Value, class Traversal, class Alloc>
bst<Key, Value, Traversal, Alloc>::Node* bst<Key, Value, Traversal, Alloc>::insert_(bst::Node* current, std::pair<Key, Value> value) {
  if (current == nullptr) {
    last_ = new Node(value);
    ++size_;
    return last_;
  } else if (value.first < current->value.first) {
    current->left = insert_(current->left, value);
    current->left->parent = current;
  } else if (value.first > current->value.first) {
    current->right = insert_(current->right, value);
    current->right->parent = current;
  } else if (value.second < current->value.second) {
    current->value.second = value.second;
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
bst<Key, Value, Traversal, Alloc>::Node* bst<Key, Value, Traversal, Alloc>::get_max_(bst::Node* current) {
  if (current != nullptr && current->right != nullptr) {
    return get_min_(current->right);
  }
  return current;
}

template<class Key, class Value, class Traversal, class Alloc>
typename bst<Key, Value, Traversal, Alloc>::Node* bst<Key, Value, Traversal, Alloc>::extract_(bst::Node* current, Key value) {
  if (!current) return nullptr;

  if (value < current->value.first) {
    current->left = extract_(current->left, value);
    if (current->left) current->left->parent = current;
  } else if (value > current->value.first) {
    current->right = extract_(current->right, value);
    if (current->right) current->right->parent = current;
  } else {
    if (!current->left) {
      Node* temp = current->right;
      delete current;
      --size_;
      if (last_ == current) {
        last_ = (temp) ? get_max_(temp) : nullptr;
      }
      return temp;
    } else if (!current->right) {
      Node* temp = current->left;
      delete current;
      --size_;
      if (last_ == current) {
        last_ = get_max_(temp);
      }
      return temp;
    }

    Node* successor = get_min_(current->right);
    current->value = successor->value;
    current->right = extract_(current->right, successor->value.first);
  }

  return current;
}

template<class Key, class Value, class Traversal, class Alloc>
inline bst<Key, Value, Traversal, Alloc>::Node* bst<Key, Value, Traversal, Alloc>::find_(bst::Node* current, key_type value) {
  if (!current) { return nullptr; }
  if (value < current->value.first) {
    return find_(current->left, value);
  } else if (value > current->value.first) {
    return find_(current->right, value);
  }
  return current;
}

template<class Key, class Value, class Traversal, class Alloc>
inline bool bst<Key, Value, Traversal, Alloc>::operator==(const bst& other) const noexcept {
  if (size_ != other.size_) {
    return false;
  }
  iterator it_1 = begin();
  iterator it_2 = other.begin();
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

template<class Key, class Value, class Traversal, class Alloc>
typename bst<Key, Value, Traversal, Alloc>::Node* bst<Key, Value, Traversal, Alloc>::copy(Node* other, Node* parent) {
  if (other == nullptr) {
    return nullptr;
  }
  Node* new_node = new Node(other->value);
  new_node->parent = parent;
  new_node->left = copy(other->left, new_node);
  new_node->right = copy(other->right, new_node);
  return new_node;
}

template<class Key, class Value, class Traversal, class Alloc>
void bst<Key, Value, Traversal, Alloc>::swap(bst& other) {
  if (*this == other) {
    return;
  }
  bst<Key, Value, Traversal, Alloc> tmp = other;
  other = *this;
  *this = tmp;
}