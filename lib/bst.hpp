#pragma once

#include <cstddef>
#include <cinttypes>

#include <lib/iterator/bst_iterator.hpp>

template<class Key, class Value, class Traversal = Preorder,
    class Compare = std::less<Key>,
    class Alloc = std::allocator<std::pair<Key, Value>>>
class bst {
 private:
  struct Node {
    std::pair<Key, Value> value;

    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    bool visited = false;
    explicit Node(std::pair<Key, Value> value)
        : value(value), left(nullptr), right(nullptr), parent(nullptr), visited(false) {};
    ~Node() noexcept {
      left = nullptr;
      right = nullptr;
      parent = nullptr;
      visited = false;
    };
    bool operator==(const Node& other) const { return (value == other.value); }
    friend std::ostream& operator<<(std::ostream& os, Node v) {
      os << '{' << v.value.first << ", " << v.value.second << '}';
      return os;
    }
  };
  size_t size_ = 0;

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
  using allocator_type = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
  using allocator_traits = typename std::allocator_traits<allocator_type>;

  allocator_type allocator_;

  using iterator = bst_iterator<Node, Traversal>;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  using key_type = Key;
  using mapped_type = Value;
  using key_compare = Compare;
  using size_type = std::size_t;
  using node_type = Node;

  using value_type = std::pair<Key, Value>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = std::allocator_traits<Alloc>::pointer;
  using const_pointer = std::allocator_traits<Alloc>::const_pointer;

  using difference_type = iterator::difference_type;

  static_assert(std::is_same<typename allocator_type::value_type, node_type>::value,
                "bst must have the same value_type as its allocator");

  explicit bst() noexcept: root_(nullptr), last_(nullptr) {}
  bst(std::initializer_list<value_type> initializer_list);
  bst(const bst& other) : size_(other.size_), allocator_(other.allocator_) {
    root_ = copy(other.root_, nullptr);
    last_ = other.last_;
  }
  ~bst() { del_(root_); }

  void insert(value_type value) { root_ = insert_(root_, value); };
  void insert(std::initializer_list<value_type> initializer_list);
  void insert(iterator i, iterator j);

  size_t count(key_type key) const noexcept;

  void extract(key_type value) {
    root_ = extract_(root_, value);
    if (root_ == nullptr) last_ == nullptr;
  };

  bool contains(key_type value) { return find_(root_, value) != nullptr; }

  iterator begin() const { return iterator(root_); }
  iterator end() const { return iterator(last_); }

  const_iterator cbegin() const noexcept { return root_ ? const_iterator(root_) : const_iterator(nullptr); }
  const_iterator cend() const noexcept { return last_ ? const_iterator(last_) : const_iterator(nullptr); }

  reverse_iterator rbegin() const noexcept { return last_ ? reverse_iterator(last_) : reverse_iterator(nullptr); }
  reverse_iterator rend() const noexcept { return root_ ? reverse_iterator(root_) : reverse_iterator(nullptr); }

  const_reverse_iterator crbegin() const noexcept {
    return last_ ? const_reverse_iterator(last_) : const_reverse_iterator(nullptr);
  }
  const_reverse_iterator crend() const noexcept {
    return root_ ? const_reverse_iterator(root_) : const_reverse_iterator(nullptr);
  }

  size_t erase(Key value) noexcept;
  iterator erase(iterator p) noexcept;
  const_iterator erase(const_iterator& r) noexcept;
  iterator erase(iterator q1, iterator q2) noexcept;

  void clear();

  size_t size() { return size_; }
  bool operator==(const bst& other) const noexcept;
  bool operator!=(const bst& other) const noexcept;

  void swap(bst& other);
  iterator operator[](size_t i);
  const_iterator operator[](size_t i) const;

  void merge(const bst& other) { return insert(other.begin(), other.end()); }
};

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bst<Key, Value, Traversal, Compare, Alloc>::iterator bst<Key, Value, Traversal, Compare, Alloc>::erase(bst::iterator q1,
                                                                                                       bst::iterator q2) noexcept {
  auto it = q1;
  for (; it != q1; it++) {
    extract((*it).value.first);
  }
  return it;
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bst<Key, Value, Traversal, Compare, Alloc>::const_iterator bst<Key,
                                                               Value,
                                                               Traversal,
                                                               Compare,
                                                               Alloc>::erase(bst::const_iterator& r) noexcept {
  auto it = cbegin();
  for (; it != cend(); it++) {
    if (it == r) {
      break;
    }
  }
  extract((*it).value.first);
  return it;
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bst<Key, Value, Traversal, Compare, Alloc>::iterator bst<Key,
                                                         Value,
                                                         Traversal,
                                                         Compare,
                                                         Alloc>::erase(bst::iterator p) noexcept {
  auto it = begin();
  for (; it != end(); it++) {
    if (it == p) {
      break;
    }
  }
  extract((*it).value.first);
  return it;
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
size_t bst<Key, Value, Traversal, Compare, Alloc>::erase(Key value) noexcept {
  size_t count = 0;
  for (auto it = begin(); it != end(); it++) {
    if ((*it).value.first == value) {
      count++;
      extract(value);
    }
  }
  return count;
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
size_t bst<Key, Value, Traversal, Compare, Alloc>::count(key_type key) const noexcept {
  size_t count = 0;
  for (auto it = begin(); it != end(); it++) {
    if ((*it).value.first == key) {
      count++;
    }
  }
  return count;
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
void bst<Key, Value, Traversal, Compare, Alloc>::insert(bst::iterator i, bst::iterator j) {
  for (; i != j; i++) {
    insert((*i).value);
  }
  insert((*i).value);
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
void bst<Key, Value, Traversal, Compare, Alloc>::clear() {
  size_t bst_size = size_;
  for (size_t i = 0; i < bst_size; i++) {
    extract((*operator[](0)).value.first);
  }
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bst<Key, Value, Traversal, Compare, Alloc>::bst(std::initializer_list<value_type> initializer_list) {
  insert(initializer_list);
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
void bst<Key, Value, Traversal, Compare, Alloc>::insert(std::initializer_list<value_type> initializer_list) {
  for (auto item : initializer_list) {
    insert(item);
  }
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bst<Key, Value, Traversal, Compare, Alloc>::iterator bst<Key, Value, Traversal, Compare, Alloc>::operator[](size_t i) {
  return iterator(begin() + i);
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bst<Key, Value, Traversal, Compare, Alloc>::const_iterator bst<Key,
                                                               Value,
                                                               Traversal,
                                                               Compare,
                                                               Alloc>::operator[](size_t i) const {
  return iterator(cbegin() + i);
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
void bst<Key, Value, Traversal, Compare, Alloc>::del_(Node* current) {
  if (!current) return;
  del_(current->left);
  del_(current->right);
  allocator_traits::destroy(allocator_, current);
  allocator_traits::deallocate(allocator_, current, 1);
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bst<Key, Value, Traversal, Compare, Alloc>::Node* bst<Key,
                                                      Value,
                                                      Traversal,
                                                      Compare,
                                                      Alloc>::insert_(bst::Node* current,
                                                                      std::pair<Key, Value> value) {
  if (current == nullptr) {
    Node* new_node = allocator_traits::allocate(allocator_, 1);
    allocator_traits::construct(allocator_, new_node, value);
    last_ = new_node;
    ++size_;
    return last_;
  } else if (key_compare{}(value.first, current->value.first)) {
    current->left = insert_(current->left, value);
    current->left->parent = current;
  } else if (key_compare{}(current->value.first, value.first)) {
    current->right = insert_(current->right, value);
    current->right->parent = current;
  } else if (value.second < current->value.second) {
    current->value.second = value.second;
  }
  return current;
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bst<Key, Value, Traversal, Compare, Alloc>::Node* bst<Key,
                                                      Value,
                                                      Traversal,
                                                      Compare,
                                                      Alloc>::get_min_(bst::Node* current) {
  if (current != nullptr && current->left != nullptr) {
    return get_min_(current->left);
  }
  return current;
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bst<Key, Value, Traversal, Compare, Alloc>::Node* bst<Key,
                                                      Value,
                                                      Traversal,
                                                      Compare,
                                                      Alloc>::get_max_(bst::Node* current) {
  if (current != nullptr && current->right != nullptr) {
    return get_min_(current->right);
  }
  return current;
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bst<Key, Value, Traversal, Compare, Alloc>::Node* bst<Key,
                                                      Value,
                                                      Traversal,
                                                      Compare,
                                                      Alloc>::extract_(bst::Node* current, Key value) {
  if (!current) return nullptr;

  if (key_compare{}(value, current->value.first)) {
    current->left = extract_(current->left, value);
    if (current->left) current->left->parent = current;
  } else if (key_compare{}(current->value.first, value)) {
    current->right = extract_(current->right, value);
    if (current->right) current->right->parent = current;
  } else {
    if (!current->left) {
      Node* temp = current->right;
      allocator_traits::destroy(allocator_, current);
      allocator_traits::deallocate(allocator_, current, 1);
      --size_;
      if (last_ == current) {
        last_ = (temp) ? get_max_(temp) : nullptr;
      }
      return temp;
    } else if (!current->right) {
      Node* temp = current->left;
      allocator_traits::destroy(allocator_, current);
      allocator_traits::deallocate(allocator_, current, 1);
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

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bst<Key, Value, Traversal, Compare, Alloc>::Node* bst<Key, Value, Traversal, Compare, Alloc>::find_(bst::Node* current,
                                                                                                    key_type value) {
  if (!current) { return nullptr; }
  if (key_compare{}(value, current->value.first)) {
    return find_(current->left, value);
  } else if (key_compare{}(current->value.first, value)) {
    return find_(current->right, value);
  }
  return current;
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bool bst<Key, Value, Traversal, Compare, Alloc>::operator==(const bst& other) const noexcept {
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

template<class Key, class Value, class Traversal, class Compare, class Alloc>
bool bst<Key, Value, Traversal, Compare, Alloc>::operator!=(const bst& other) const noexcept {
  return !(this->operator==(other));
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
typename bst<Key, Value, Traversal, Compare, Alloc>::Node* bst<Key, Value, Traversal, Compare, Alloc>::copy(Node* other,
                                                                                                            Node* parent) {
  if (other == nullptr) {
    return nullptr;
  }
  Node* new_node = allocator_traits::allocate(allocator_, 1);
  allocator_traits::construct(allocator_, new_node, other->value);
  new_node->parent = parent;
  new_node->left = copy(other->left, new_node);
  new_node->right = copy(other->right, new_node);
  return new_node;
}

template<class Key, class Value, class Traversal, class Compare, class Alloc>
void bst<Key, Value, Traversal, Compare, Alloc>::swap(bst& other) {
  if (*this == other) {
    return;
  }
  bst<Key, Value, Traversal, Alloc> tmp = other;
  other = *this;
  *this = tmp;
}