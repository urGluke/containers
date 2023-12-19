#ifndef __S21_MAP_H__
#define __S21_MAP_H__

#include <limits>
#include <utility>
#include <vector>

#include "RBTreeMap.h"

namespace s21 {

template <class K, class V>
class map {
 public:
  typedef K key_type;
  typedef V mapped_type;
  typedef std::pair<const key_type, mapped_type> value_type;
  typedef value_type& reference;
  typedef RBTreeMap<key_type, mapped_type> rbtree;
  typedef const value_type& const_reference;
  typedef typename rbtree::iterator iterator;
  typedef typename rbtree::const_iterator const_iterator;

  map() = default;

  map(std::initializer_list<value_type> const& items) {
    for (const auto& item : items) tree.insert(item);
  }

  map(const map& s) : tree(s.tree) {}

  map(map&& s) : tree(std::move(s.tree)) {}

  ~map() { tree.clear(); }

  map& operator=(const map& s) {
    map copy(s);
    *this = std::move(copy);
    return *this;
  }

  iterator begin() { return tree.begin(); }

  iterator end() { return tree.end(); }  // fix segv on access to value

  const_iterator begin() const { return tree.begin(); }

  const_iterator end() const { return tree.end(); }

  bool empty() const { return tree.isEmpty(); }

  size_t size() const { return tree.size(); }

  size_t max_size() const { return std::numeric_limits<size_t>::max(); }

  void clear() { tree.clear(); }

  std::pair<iterator, bool> insert(const value_type& value) {
    bool inserted = tree.contains(value.first);
    if (!inserted) return std::make_pair(tree.insert(value), true);
    return std::make_pair(iterator(tree.find(value.first)), false);
  }

  std::pair<iterator, bool> insert(const key_type& key,
                                   const mapped_type& val) {
    value_type value(key, val);
    bool inserted = tree.contains(key);
    if (!inserted) return std::make_pair(tree.insert(value), true);
    return std::make_pair(iterator(tree.find(key)), false);
  }

  std::pair<iterator, bool> insert_or_assign(const key_type& key,
                                             const mapped_type& val) {
    bool exist = tree.contains(key);
    value_type value(key, val);
    if (!exist) {
      tree.insert(value);
    } else {
      tree.setData(value);
    }
    iterator pos(tree.find(key));
    return std::make_pair(pos, !exist);
  }
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    std::initializer_list elements{args...};
    std::vector<std::pair<iterator, bool>> result(elements.size() / 2);
    size_t i = 1;
    key_type key;
    for (auto elem : elements) {
      if (i % 2 == 0) {
        result.push_back(insert(key, elem));
      } else {
        key = elem;
      }
      i++;
    }
    return result;
  }

  void erase(iterator pos) {
    if (pos != end()) tree.remove(pos->first);
  }

  void swap(map& other) { std::swap(tree, other.tree); }

  void merge(map& other) {
    for (const auto& item : other) tree.insert(item);
    other.clear();
  }

  iterator find(const key_type& key) { return iterator(tree.find(key)); }

  bool contains(const key_type& key) const { return tree.find(key) != nullptr; }

  mapped_type& at(const key_type& key) {
    return insert(key, mapped_type()).first->second;
  }

  mapped_type& operator[](const key_type& key) {
    return insert(key, mapped_type()).first->second;
  }

 private:
  rbtree tree;
};

}  // namespace s21

#endif  // __S21_MAP_H__