#ifndef CONTAINERS_SRC_S21_LIST_H_
#define CONTAINERS_SRC_S21_LIST_H_

#include <cstddef>
#include <initializer_list>
#include <limits>
#include <stdexcept>
namespace S21 {
template <typename T>
class List {
  struct ListNode;
  class ListIterator;
  class ListConstIterator;

 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = ListIterator;
  using const_iterator = ListConstIterator;
  using size_type = std::size_t;

  List() : sentinel_(new ListNode(T())), size_(0) {
    sentinel_->next_ = sentinel_;
    sentinel_->prev_ = sentinel_;
  }

  List(size_type n) : sentinel_(new ListNode(T())), size_(0) {
    for (; n > 0; --n) push_back(T{});
  }

  List(std::initializer_list<value_type> const &items)
      : sentinel_(new ListNode(T())), size_(0) {
    for (auto &i : items) push_back(i);
  }

  List(const List &l) : sentinel_(new ListNode(T())), size_(0) {
    sentinel_->next_ = sentinel_;
    sentinel_->prev_ = sentinel_;
    for (const_iterator it = l.begin(); it != l.end(); ++it) push_back(*it);
  }

  List(List &&l) : sentinel_(std::move(l.sentinel_)), size_(l.size_) {
    l.sentinel_ = nullptr;
    l.size_ = 0;
    // delete l.sentinel_;
  }

  ~List() {
    while (size_) pop_back();
    delete sentinel_;
  }

  List<T> &operator=(List &&l) {
    if (this != &l) {
      clear();
      delete sentinel_;
      new (this) List(std::move(l));
      l.sentinel_ = nullptr;
      l.size_ = 0;
    }
    return *this;
  }

  List<T> &operator=(const List &l) {
    if (this != &l) {
      clear();
      for (const_iterator it = l.begin(); it != l.end(); ++it) push_back(*it);
    }
    return *this;
  }

  const_reference front() const {
    if (size_)
      return sentinel_->next_->value_;
    else {
      static const_reference default_value_{};
      return default_value_;
    }
  }

  const_reference back() const {
    if (size_)
      return sentinel_->prev_->value_;
    else {
      static const_reference default_value_{};
      return default_value_;
    }
  }

  iterator begin() { return iterator(sentinel_->next_); }

  iterator end() { return iterator(sentinel_); }

  const_iterator begin() const { return const_iterator(sentinel_->next_); }

  const_iterator end() const { return const_iterator(sentinel_); }

  bool empty() { return size_ == 0; }
  size_type size() { return size_; }
  size_type max_size() { return std::numeric_limits<size_type>::max(); }

  void clear() {
    while (size_) pop_back();
  }

  iterator insert(iterator pos, const_reference value_) {
    ListNode *newNode = new ListNode(value_);
    pos.it->prev_->next_ = newNode;
    newNode->prev_ = pos.it->prev_;
    pos.it->prev_ = newNode;
    newNode->next_ = pos.it;
    size_++;
    return iterator(newNode);
  }

  void erase(iterator pos) {
    if (size_) {
      ListNode *tmp = pos.it;
      if (size_ == 1) {
        sentinel_->next_ = sentinel_->prev_ = sentinel_;
        pos = iterator(sentinel_);
      } else {
        tmp->next_->prev_ = tmp->prev_;
        tmp->prev_->next_ = tmp->next_;
        pos = iterator(tmp->next_);
      }
      delete tmp;
      size_--;
    }
  }

  void push_back(const_reference value_) {
    ListNode *newNode = new ListNode(value_);
    if (!size_) {
      newNode->next_ = newNode->prev_ = sentinel_;
      sentinel_->prev_ = sentinel_->next_ = newNode;
    } else {
      sentinel_->prev_->next_ = newNode;
      newNode->next_ = sentinel_;
      newNode->prev_ = sentinel_->prev_;
      sentinel_->prev_ = newNode;
    }
    size_++;
  }

  void pop_back() {
    if (size_) {
      ListNode *tmp = sentinel_->prev_;
      if (size_ == 1) {
        sentinel_->next_ = sentinel_;
        sentinel_->prev_ = sentinel_;
      } else {
        tmp->prev_->next_ = sentinel_;
        sentinel_->prev_ = tmp->prev_;
      }
      delete tmp;
      size_--;
    } else {  // Повтор ошибки оригинала
      int *ptr = reinterpret_cast<int *>(0x10);
      delete ptr;
    }
  }

  void push_front(const_reference value_) {
    ListNode *newNode = new ListNode(value_);
    if (!size_) {
      newNode->next_ = newNode->prev_ = sentinel_;
      sentinel_->prev_ = sentinel_->next_ = newNode;
    } else {
      sentinel_->next_->prev_ = newNode;
      newNode->prev_ = sentinel_;
      newNode->next_ = sentinel_->next_;
      sentinel_->next_ = newNode;
    }
    size_++;
  }

  void pop_front() {
    if (size_) {
      ListNode *tmp = sentinel_->next_;
      if (size_ == 1) {
        sentinel_->next_ = sentinel_;
        sentinel_->prev_ = sentinel_;
      } else {
        tmp->next_->prev_ = sentinel_;
        sentinel_->next_ = tmp->next_;
      }
      delete tmp;
      size_--;
    } else {  // Повтор ошибки оригинала
      int *ptr = reinterpret_cast<int *>(0x10);
      delete ptr;
    }
  }

  void swap(List &l) {
    if (this != &l) {
      std::swap(sentinel_, l.sentinel_);
      std::swap(size_, l.size_);
    }
  }

  void merge(List &l) {
    List tmp;
    auto itTmp = begin();
    auto itL = l.begin();
    while (itTmp != end() && itL != l.end())
      if (*itTmp < *itL) {
        tmp.push_back(*itTmp);
        ++itTmp;
      } else {
        tmp.push_back(*itL);
        ++itL;
      }
    while (itTmp != end()) {
      tmp.push_back(*itTmp);
      ++itTmp;
    }
    while (itL != l.end()) {
      tmp.push_back(*itL);
      ++itL;
    }
    clear();
    swap(tmp);
    l.clear();
  }

  void splice(const_iterator pos, List &l) {
    if (size_ && l.size_) {
      l.sentinel_->next_->prev_ = pos.it->prev_;
      l.sentinel_->prev_->next_ = pos.it;
      pos.it->prev_->next_ = l.sentinel_->next_;
      pos.it->prev_ = l.sentinel_->prev_;
      size_ += l.size_;
      l.size_ = 0;
      l.sentinel_->next_ = l.sentinel_;
      l.sentinel_->prev_ = l.sentinel_;
    } else if (!size_ && l.size_) {
      std::swap(*this, l);
      l.size_ = 0;
      l.sentinel_->next_ = l.sentinel_;
      l.sentinel_->prev_ = l.sentinel_;
    }
  }

  void reverse() {
    auto itStart = begin();
    auto itStop = --end();
    for (size_type i = 0; i < size_ / 2; i++, ++itStart, --itStop)
      std::swap(*itStart, *itStop);
  }

  void unique() {
    for (auto it = ++begin(), it2 = begin(); it != end(); ++it, ++it2)
      if (*it == *it2) erase(it2--);
  }

  void sort() {
    if (size_ > 1) quickSort(sentinel_->next_, sentinel_->prev_);
  }

  template <typename... Args>
  void insert_many_back(Args &&...args) {
    ([&] { push_back(args); }(), ...);
  }

  template <typename... Args>
  void insert_many_front(Args &&...args) {
    iterator it = begin();
    for (auto value_ : {std::forward<Args>(args)...}) insert(it, value_);
  }

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args) {
    iterator not_const_pos = pos.it;
    ([&] { insert(not_const_pos, args); }(), ...);
    return not_const_pos;
  }

 private:
  class ListIterator {
   public:
    friend class List;
    ListIterator(typename List<T>::ListNode *node) : it(node) {}

    ListIterator &operator++() {
      it = it->next_;
      return *this;
    }

    ListIterator &operator--() {
      it = it->prev_;
      return *this;
    }

    ListIterator operator++(int) {
      ListIterator temp = *this;
      it = it->next_;
      return temp;
    }

    ListIterator operator--(int) {
      ListIterator temp = *this;
      it = it->prev_;
      return temp;
    }

    bool operator==(const ListIterator &l) const { return it == l.it; }

    bool operator!=(const ListIterator &l) const { return it != l.it; }

    reference operator*() const { return it->value_; }

   private:
    typename List<T>::ListNode *it;
  };
  class ListConstIterator {
   public:
    friend class List;
    ListConstIterator(const iterator &other) : it(other.it) {}

    ListConstIterator(typename List<T>::ListNode *node) : it(node) {}

    ListConstIterator &operator++() {
      it = it->next_;
      return *this;
    }

    ListConstIterator &operator--() {
      it = it->prev_;
      return *this;
    }

    ListConstIterator operator++(int) {
      ListConstIterator temp = *this;
      it = it->next_;
      return temp;
    }

    ListConstIterator operator--(int) {
      ListConstIterator temp = *this;
      it = it->prev_;
      return temp;
    }

    bool operator==(const ListConstIterator &l) const { return it == l.it; }

    bool operator!=(const ListConstIterator &l) const { return it != l.it; }

    const_reference operator*() const { return it->value_; }

   private:
    typename List<T>::ListNode *it;
  };
  struct ListNode {
    value_type value_;
    ListNode *prev_;
    ListNode *next_;
    ListNode(const_reference val)
        : value_(val), prev_(nullptr), next_(nullptr) {}
  };

  ListNode *sentinel_;
  size_type size_;

  void quickSort(ListNode *start, ListNode *end) {
    if (start != end && end != nullptr && start != end->next_) {
      ListNode *pivot = partition(start, end);
      quickSort(start, pivot->prev_);
      quickSort(pivot->next_, end);
    }
  }

  ListNode *partition(ListNode *start, ListNode *end) {
    T pivotvalue_ = end->value_;
    ListNode *i = start->prev_;
    for (ListNode *j = start; j != end; j = j->next_)
      if (j->value_ <= pivotvalue_) {
        i = (i == nullptr) ? start : i->next_;
        std::swap(i->value_, j->value_);
      }
    i = (i == nullptr) ? start : i->next_;
    std::swap(i->value_, end->value_);
    return i;
  }
};

}  // namespace S21

#endif  // CONTAINERS_SRC_S21_LIST_H_
