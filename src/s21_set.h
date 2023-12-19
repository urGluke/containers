#ifndef CONTAINERS_SRC_S21_SET_H_
#define CONTAINERS_SRC_S21_SET_H_

#include <limits>
#include <utility>

#include "RBTree.h"

namespace S21 {

template <typename Key>
class Set {
  friend class RBTree<Key>;

 private:
  RBTree<Key> tree_;

 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = typename RBTree<Key>::iterator;
  using const_iterator = typename RBTree<Key>::iterator;
  using size_type = size_t;

  // Конструкторы
  Set() {}

  Set(std::initializer_list<value_type> const &items) {
    for (const auto &item : items) tree_.insert(item);
  }
  Set(const Set &s) { tree_ = s.tree_; }

  Set(Set &&s) : tree_(std::move(s.tree_)) {}

  ~Set() { tree_.clear(); }

  Set &operator=(Set &&s) {
    if (this != &s) tree_ = std::move(s.tree_);
    return *this;
  }

  // Итераторы
  iterator begin() { return tree_.begin(); }
  iterator end() {
    return iterator();  // Возвращает итератор на nullptr
  }

  // Вместимость
  bool empty() const { return tree_.isEmpty(); }
  size_type size() const { return tree_.size(); }
  size_type max_size() const { return std::numeric_limits<size_type>::max(); }

  // Модификаторы
  void clear() { tree_.clear(); }

  std::pair<iterator, bool> insert(const value_type &value) {
    auto result = tree_.find(value);
    // Проверьте, существует ли элемент в дереве (если итератор не указывает на
    // конец дерева).
    if (result != tree_.end()) {
      // Если элемент уже существует, возвращаем неудачу.
      return std::make_pair(result, false);
    }

    // Если элемент не найден, вставляем его.
    result = tree_.insert(value);
    return std::make_pair(result, true);
  }

  void erase(iterator pos) {
    if (pos != end()) tree_.remove(*pos);
  }

  void swap(Set &other) { std::swap(tree_, other.tree_); }

  void merge(Set &other) {
    for (const auto &item : other) tree_.insert(item);
    other.clear();
  }

  // Поиск
  iterator find(const key_type &key) { return tree_.find(key); }

  bool contains(const key_type &key) const {
    return tree_.find(key) != nullptr;
  }
};

}  // namespace S21

#endif  // CONTAINERS_SRC_S21_SET_H_
