#ifndef CONTAINERS_SRC_S21_MULTISET_H_
#define CONTAINERS_SRC_S21_MULTISET_H_

#include <limits>
#include <utility>

#include "RBTree.h"
namespace S21 {

template <typename Key>
class multiset {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = typename RBTree<Key>::iterator;
  using const_iterator = typename RBTree<Key>::const_iterator;
  using size_type = size_t;

  // Конструкторы и деструктор
  multiset() : tree_() {}

  multiset(std::initializer_list<value_type> const &items) {
    for (const auto &item : items) tree_.insert(item);
  }

  multiset(const multiset &ms) : tree_(ms.tree_) {}

  multiset(multiset &&ms) : tree_(std::move(ms.tree_)) {}

  ~multiset() = default;

  // Оператор присваивания
  multiset &operator=(multiset &&ms) {
    tree_ = std::move(ms.tree_);
    return *this;
  }

  // Итераторы
  iterator begin() { return tree_.begin(); }

  iterator end() { return tree_.end(); }

  // Емкость
  bool empty() const { return tree_.isEmpty(); }

  size_type size() const { return tree_.size(); }

  size_type max_size() const { return tree_.max_size(); }

  // Модификаторы
  void clear() { tree_.clear(); }

  iterator insert(const value_type &value) { return tree_.insert(value); }

  void erase(iterator pos) {
    if (pos == end()) return;
    Key value = *pos;
    tree_.remove(value);
  }

  void swap(multiset &other) { tree_.swap(other.tree_); }

  void merge(multiset &other) { tree_.merge(other.tree_); }

  // Поиск
  size_type count(const Key &key) const { return tree_.count(key); }

  iterator find(const Key &key) { return tree_.find(key); }

  bool contains(const Key &key) const { return tree_.contains(key); }

  std::pair<iterator, iterator> equal_range(const Key &key) const {
    return tree_.equal_range(key);
  }

  iterator lower_bound(const Key &key) const { return tree_.lower_bound(key); }

  iterator upper_bound(const Key &key) const { return tree_.upper_bound(key); }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    return this->tree_.insert_many(std::forward<Args>(args)...);
  }

 private:
  RBTree<key_type> tree_;
};

}  // namespace S21

#endif  // CONTAINERS_SRC_S21_MULTISET_H_
