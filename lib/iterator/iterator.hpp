#pragma once

#include <iterator>

struct Preorder {};
struct Inorder {};
struct Postorder {};

template<class T, typename Tag = Preorder>
class iterator {
 private:
  T* current;

 public:
  typedef std::ptrdiff_t pointer_difference;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef const T& const_reference;

  iterator() : current() {};
  iterator(const iterator<value_type>& _x) : current(_x.current) {};
  iterator(pointer current) : current(current) {};

//  iterator& operator=(const iterator<value_type>& _x);
//  iterator& operator=(pointer _x); fixme

  ~iterator() noexcept = default;

  reference operator*() const noexcept;
  reference operator->() const noexcept;

  iterator& operator++() noexcept;
  const iterator operator++(int) noexcept;

  iterator& operator--() noexcept;
  const iterator operator--(int) noexcept;

  iterator operator+(int n) noexcept;
  iterator operator-(int n) noexcept;

  iterator operator+=(int n) noexcept;
  iterator operator-=(int n) noexcept;

  bool operator==(const iterator& _x) const noexcept;
  bool operator!=(const iterator& _x) const noexcept;

  bool operator>(const iterator& _x) const noexcept;
  bool operator>=(const iterator& _x) const noexcept;

  bool operator<(const iterator& _x) const noexcept;
  bool operator<=(const iterator& _x) const noexcept;
};

//template<class T, typename Tag>
//iterator<T, Tag>& iterator<T, Tag>::operator=(iterator::pointer _x) {
//  current = _x;
//  return *this;
//}
//
//template<class T, typename Tag>
//iterator<T, Tag>& iterator<T, Tag>::operator=(const iterator<T>& _x) {
//  current = _x.current;
//  return *this;
//} fixme

template<class T, typename Tag>
inline iterator<T, Tag>::reference iterator<T, Tag>::operator*() const noexcept {
  return *current;
}

template<class T, typename Tag>
inline T& iterator<T, Tag>::operator->() const noexcept {
  return current;
}

template<class T, typename Tag>
inline iterator<T, Tag>& iterator<T, Tag>::operator++() noexcept {
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
inline const iterator<T, Tag> iterator<T, Tag>::operator++(int) noexcept {
  iterator<T, Tag> _tmp = *this;
  this->operator++();
  return _tmp;
}

template<class T, typename Tag>
inline iterator<T, Tag>& iterator<T, Tag>::operator--() noexcept {
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
inline const iterator<T, Tag> iterator<T, Tag>::operator--(int) noexcept {
  iterator<T, Tag> _tmp = *this;
  this->operator--();
  return _tmp;
}

template<class T, typename Tag>
inline iterator<T, Tag> iterator<T, Tag>::operator+(int n) noexcept {
  if (n == 0) { return *this; }
  if (n < 0) { return operator-(-n); }
  return iterator(current + n);
}

template<class T, typename Tag>
inline iterator<T, Tag> iterator<T, Tag>::operator-(int n) noexcept {
  if (n <= 0) { return operator+(-n); }
  return iterator(current - n);
}

template<class T, typename Tag>
inline iterator<T, Tag> iterator<T, Tag>::operator+=(int n) noexcept {
  current += n;
  return *this;
}

template<class T, typename Tag>
inline iterator<T, Tag> iterator<T, Tag>::operator-=(int n) noexcept {
  current -= n;
  return *this;
}

template<class T, typename Tag>
inline bool iterator<T, Tag>::operator==(const iterator& _x) const noexcept {
  return current == _x.current;
}

template<class T, typename Tag>
inline bool iterator<T, Tag>::operator!=(const iterator& _x) const noexcept {
  return !(this->operator==(_x));
}

template<class T, typename Tag>
inline bool iterator<T, Tag>::operator>(const iterator& _x) const noexcept {
  return current > _x.current;
}

template<class T, typename Tag>
inline bool iterator<T, Tag>::operator>=(const iterator& _x) const noexcept {
  return current >= _x.current;
}

template<class T, typename Tag>
inline bool iterator<T, Tag>::operator<(const iterator& _x) const noexcept {
  return current < _x.current;
}

template<class T, typename Tag>
inline bool iterator<T, Tag>::operator<=(const iterator& _x) const noexcept {
  return current <= _x.current;
}
