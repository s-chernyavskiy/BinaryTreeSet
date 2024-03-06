#pragma once

#include <cstddef>
#include <cinttypes>

#include <lib/iterator/bst_iterator.hpp>

template<class T, class Traversal = Preorder, class Alloc = std::allocator<std::pair<T, Traversal>>>
class bst {
 private:
  size_t size_ = 0;
  Alloc allocator_{};

  struct Node {
    T value;

    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    bool visited = false;
    explicit Node(T value) : value(value), left(nullptr), right(nullptr), parent(nullptr), visited(false) {};
    ~Node() noexcept {
      left = nullptr;
      right = nullptr;
      parent = nullptr;
      visited = false;
    };
    inline bool operator==(const Node& other) { return (value == other.value); }
  };

  Node* root_ = nullptr;
  Node* last_ = nullptr;

  void del_(Node* current);
  Node* insert_(Node* current, T value);
  Node* extract_(Node* current, T value);
  Node* get_min_(Node* current);
  Node* find_(Node* current, T value);
  Node* copy(Node* other, Node* parent = nullptr);
 public:
  using allocator_type = typename std::allocator_traits<Alloc>::template rebind_alloc<T>;

  using iterator = bst_iterator<Node, Traversal>;
  using const_iterator = const iterator&;

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;

  using difference_type = iterator::difference_type;

  static_assert(std::is_same<typename allocator_type::value_type, value_type>::value,
                "bst must have the same value_type as its allocator");

  friend std::ostream& operator<<(std::ostream& os, bst<T, Traversal>::Node v) {
    os << v.value;
    return os;
  }

  explicit bst() noexcept: root_(nullptr), last_(nullptr) {}
  bst(std::initializer_list<T> initializer_list);
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
  void extract(value_type value) { root_ = extract_(root_, value); };

  bool contains(value_type value) { return find_(root_, value) != nullptr; }

  iterator begin() const { return iterator(root_); }
  iterator end() const { return iterator(last_); }

  const_iterator cbegin() const noexcept { return root_ ? const_iterator(root_) : const_iterator(nullptr); }
  const_iterator cend() const noexcept { return last_ ? const_iterator(last_) : const_iterator(nullptr); }

  size_t size() { return size_; }
  bool operator==(const bst& other) const noexcept;
  bool operator!=(const bst& other) const noexcept;

  void swap(bst<T, Traversal, Alloc>& other);
  iterator operator[](size_t i);
  const_iterator operator[](size_t i) const;
};

template<class T, class Traversal, class Alloc>
bst<T, Traversal, Alloc>::bst(std::initializer_list<T> initializer_list) {
  insert(initializer_list);
}

template<class T, class Traversal, class Alloc>
void bst<T, Traversal, Alloc>::insert(std::initializer_list<value_type> initializer_list) {
  for (auto item : initializer_list) {
    insert(item);
  }
}

template<class T, class Traversal, class Alloc>
bst<T, Traversal, Alloc>::iterator bst<T, Traversal, Alloc>::operator[](size_t i) {
  return iterator(begin() + i);
}

template<class T, class Traversal, class Alloc>
bst<T, Traversal, Alloc>::const_iterator bst<T, Traversal, Alloc>::operator[](size_t i) const {
  return *(cbegin() + i);
}

template<class T, class Traversal, class Alloc>
void bst<T, Traversal, Alloc>::del_(Node* current) {
  if (!current) return;
  del_(current->left);
  del_(current->right);
  delete current;
}

template<class T, class Traversal, class Alloc>
bst<T, Traversal, Alloc>::Node* bst<T, Traversal, Alloc>::insert_(bst::Node* current, T value) {
  if (current == nullptr) {
    last_ = new Node(value);
    ++size_;
    return last_;
  } else if (value < current->value) {
    current->left = insert_(current->left, value);
    current->left->parent = current;
  } else if (value > current->value) {
    current->right = insert_(current->right, value);
    current->right->parent = current;
  }
  return current;
}

template<class T, class Traversal, class Alloc>
bst<T, Traversal, Alloc>::Node* bst<T, Traversal, Alloc>::get_min_(bst::Node* current) {
  if (current != nullptr && current->left != nullptr) {
    return get_min_(current->left);
  }
  return current;
}

template<class T, class Traversal, class Alloc>
bst<T, Traversal, Alloc>::Node* bst<T, Traversal, Alloc>::extract_(bst::Node* current, T value) {
  if (!current) return nullptr;
  if (value < current->value) {
    current->left = extract_(current->left, value);
    if (current->left) current->left->parent = current;
  } else if (value > current->value) {
    current->right = extract_(current->right, value);
    if (current->right) current->right->parent = current;
  } else {
    if (!current->left) {
      Node* temp = current->right;
      delete current;
      --size_;
      return temp;
    } else if (!current->right) {
      Node* temp = current->left;
      delete current;
      --size_;
      return temp;
    }

    Node* minNode = get_min_(current->right);
    current->value = minNode->value;
    current->right = extract_(current->right, minNode->value);
    if (current->right) current->right->parent = current;
  }

  return current;
}

template<class T, class Traversal, class Alloc>
inline bst<T, Traversal, Alloc>::Node* bst<T, Traversal, Alloc>::find_(bst::Node* current, T value) {
  if (!current) { return nullptr; }
  if (value < current->value) {
    return find_(current->left, value);
  } else if (value > current->value) {
    return find_(current->right, value);
  }
  return current;
}

template<class T, class Traversal, class Alloc>
inline bool bst<T, Traversal, Alloc>::operator==(const bst& other) const noexcept {
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

template<class T, class Traversal, class Alloc>
inline bool bst<T, Traversal, Alloc>::operator!=(const bst& other) const noexcept {
  return !(this->operator==(other));
}

template<class T, class Traversal, class Alloc>
typename bst<T, Traversal, Alloc>::Node* bst<T, Traversal, Alloc>::copy(Node* other, Node* parent) {
  if (other == nullptr) {
    return nullptr;
  }
  Node* new_node = new Node(other->value);
  new_node->parent = parent;
  new_node->left = copy(other->left, new_node);
  new_node->right = copy(other->right, new_node);
  return new_node;
}

template<class T, class Traversal, class Alloc>
void bst<T, Traversal, Alloc>::swap(bst<T, Traversal, Alloc>& other) {
  if (*this == other) {
    return;
  }
  bst<T, Traversal, Alloc> tmp = other;
  other = *this;
  *this = tmp;
}