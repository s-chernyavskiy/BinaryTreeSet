#pragma once

#include <iterator>
#include <iostream>

struct Preorder {};
struct Inorder {};
struct Postorder {};

template<class T, typename Tag = Preorder>
class bst_iterator {
 private:
  T* current{};

 public:
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using const_reference = const T&;
  using traversal = Tag;

  bst_iterator() : current() {};
  bst_iterator(const bst_iterator<value_type>& _x) : current(_x.current) {};
  bst_iterator(pointer current) : current(current) {};

  explicit operator bst_iterator<const T>() requires (std::is_const_v<T> == false);

  ~bst_iterator() noexcept = default;

  reference operator*() const noexcept;
  reference operator->() const noexcept;

  bst_iterator& operator++() noexcept;
  const bst_iterator operator++(int) noexcept;

  bst_iterator& operator--() noexcept;
  const bst_iterator operator--(int) noexcept;

  bst_iterator operator+(int n) noexcept;
  bst_iterator operator-(int n) noexcept;

  bst_iterator operator+=(int n) noexcept;
  bst_iterator operator-=(int n) noexcept;

  bool operator==(const bst_iterator& _x) const noexcept;
  bool operator!=(const bst_iterator& _x) const noexcept;

  bool operator>(const bst_iterator& _x) const noexcept;
  bool operator>=(const bst_iterator& _x) const noexcept;

  bool operator<(const bst_iterator& _x) const noexcept;
  bool operator<=(const bst_iterator& _x) const noexcept;
};

template<class T, typename Tag>
inline bst_iterator<T, Tag>::operator bst_iterator<const T>() requires (std::is_const_v<T> == false) {
  return bst_iterator<const T>(current);
}

template<class T, typename Tag>
inline bst_iterator<T, Tag>::reference bst_iterator<T, Tag>::operator*() const noexcept {
  return *current;
}

template<class T, typename Tag>
inline T& bst_iterator<T, Tag>::operator->() const noexcept {
  return current;
}

template<class T, typename Tag>
inline bst_iterator<T, Tag>& bst_iterator<T, Tag>::operator++() noexcept {
  try {
    if constexpr (std::is_same_v<Tag, Preorder>) {
      if (current->left && !current->left->visited) {
        current = current->left;
      } else {
        while (current->parent) {
          if (!current->right && !current->left) {
            if (!current->parent->visited) {
              current->visited = true;
              current = current->parent;
              break;
            } else {
              current->visited = true;
              current = current->parent;
              while (current->parent && !current->parent->visited) {
                current->visited = true;
                current = current->parent;
              }
            }
          } else if (current->right && !current->right->visited) {
            current->visited = true;
            current = current->right;
            break;
          } else {
            throw std::runtime_error("Out of bounds.");
          }
        }
        if (current->right != nullptr) {
          current->visited = true;
          current = current->right;
        }
      }
    } else if constexpr (std::is_same_v<Tag, Inorder>) {
      while (current->left && !current->left->visited) {
        current = current->left;
      }
      if (current->right) {
        current = current->right;
        while (current->left) {
          current = current->left;
        }
      }
      if (!current->visited) {
        current->visited = true;
      } else {
        while (current->visited) {
          if (current->parent) {
            current = current->parent;
          } else {
            throw std::runtime_error("Out of bounds.");
          }
        }
        current->visited = true;
      }
    } else if constexpr (std::is_same_v<Tag, Postorder>) {
      while (current->parent) {
        current = current->parent;
      }
      if (current->visited) {
        throw std::runtime_error("Out of bounds.");
      }
      while (current && !current->visited) {
        if (current->left && !current->left->visited) {
          current = current->left;
        } else if (current->right && !current->right->visited) {
          current = current->right;
        } else {
          if (!current->visited) {
            current->visited = true;
          }
        }
      }
    }
  } catch (std::runtime_error& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
  }
  return *this;
}

template<class T, typename Tag>
inline const bst_iterator<T, Tag> bst_iterator<T, Tag>::operator++(int) noexcept {
  bst_iterator<T, Tag> _tmp = *this;
  this->operator++();
  return _tmp;
}

template<class T, typename Tag>
inline bst_iterator<T, Tag>& bst_iterator<T, Tag>::operator--() noexcept {
  try {
    if constexpr (std::is_same_v<Tag, Preorder>) {
      if (current->parent) {
        current = current->parent;
      } else {
        throw std::runtime_error("Out of bounds.");
      }
    } else if constexpr (std::is_same_v<Tag, Inorder>) {
      if (current->left) {
        current = current->left;
      } else if (current == current->parent->right) {
        current = current->parent;
      } else {
        throw std::runtime_error("Out of bounds.");
      }
    } else if constexpr (std::is_same_v<Tag, Postorder>) {
      if (current->right) {
        current = current->right;
      } else if (current == current->parent->right) {
        current = current->parent->left;
      } else if (current->parent) {
        current = current->parent;
      } else {
        throw std::runtime_error("Out of bounds.");
      }
    }
  } catch (std::runtime_error& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  return *this;
}

template<class T, typename Tag>
inline const bst_iterator<T, Tag> bst_iterator<T, Tag>::operator--(int) noexcept {
  bst_iterator<T, Tag> _tmp = *this;
  this->operator--();
  return _tmp;
}

template<class T, typename Tag>
inline bst_iterator<T, Tag> bst_iterator<T, Tag>::operator+(int n) noexcept {
  if (n == 0) { return *this; }
  if (n < 0) { return operator-(-n); }
  for (int i = 0; i < n; i++) {
    operator++();
  }
  return *this;
}

template<class T, typename Tag>
inline bst_iterator<T, Tag> bst_iterator<T, Tag>::operator-(int n) noexcept {
  if (n <= 0) { return operator+(-n); }
  for (int i = 0; i < n; i++) {
    operator--();
  }
  return *this;
}

template<class T, typename Tag>
inline bst_iterator<T, Tag> bst_iterator<T, Tag>::operator+=(int n) noexcept {
  current += n;
  return *this;
}

template<class T, typename Tag>
inline bst_iterator<T, Tag> bst_iterator<T, Tag>::operator-=(int n) noexcept {
  current -= n;
  return *this;
}

template<class T, typename Tag>
inline bool bst_iterator<T, Tag>::operator==(const bst_iterator& _x) const noexcept {
  return *current == *_x.current;
}

template<class T, typename Tag>
inline bool bst_iterator<T, Tag>::operator!=(const bst_iterator& _x) const noexcept {
  return !(this->operator==(_x));
}

template<class T, typename Tag>
inline bool bst_iterator<T, Tag>::operator>(const bst_iterator& _x) const noexcept {
  return *current > *_x.current;
}

template<class T, typename Tag>
inline bool bst_iterator<T, Tag>::operator>=(const bst_iterator& _x) const noexcept {
  return *current >= *_x.current;
}

template<class T, typename Tag>
inline bool bst_iterator<T, Tag>::operator<(const bst_iterator& _x) const noexcept {
  return *current < *_x.current;
}

template<class T, typename Tag>
inline bool bst_iterator<T, Tag>::operator<=(const bst_iterator& _x) const noexcept {
  return *current <= *_x.current;
}
