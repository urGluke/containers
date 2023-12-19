#ifndef CPP2_S21_CONTAINERS_1_SRC_S21_VECTOR_H_
#define CPP2_S21_CONTAINERS_1_SRC_S21_VECTOR_H_

#include <algorithm>         // copy begin end
#include <cstddef>           // size_t
#include <initializer_list>  // initializer_list
#include <limits>            // max
#include <stdexcept>         // out_of_range

namespace s21 {
template <class T>
class Vector {
 public:
  class VectorConstIterator;
  class VectorIterator;
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator_pointer = value_type *;
  using const_iterator_pointer = const value_type *;
  using iterator = Vector<T>::VectorIterator;
  using const_iterator = Vector<T>::VectorConstIterator;
  using size_type = std::size_t;

  Vector() : size_(0), capacity_(0), data_(nullptr){};
  Vector(size_type n)
      : size_(n), capacity_(n), data_(new value_type[capacity_]()){};
  Vector(std::initializer_list<value_type> const &items)
      : size_(items.size()),
        capacity_(items.size()),
        data_(new value_type[capacity_]()) {
    std::copy(items.begin(), items.end(), data_);
  }
  Vector(const Vector &v)
      : size_(v.size_),
        capacity_(v.capacity_),
        data_(new value_type[capacity_]()) {
    for (std::size_t i = 0; i < size_; i++) {
      data_[i] = v.data_[i];
    }
  }
  Vector(Vector &&v) noexcept
      : size_(v.size_), capacity_(v.capacity_), data_(v.data_) {
    v.data_ = nullptr;
    v.size_ = 0;
    v.capacity_ = 0;
  }
  Vector &operator=(Vector &&v) noexcept {
    if (this != &v) {
      clear();

      data_ = v.data_;
      size_ = v.size_;
      capacity_ = v.capacity_;

      v.data_ = nullptr;
      v.size_ = 0;
      v.capacity_ = 0;
    }
    return *this;
  }
  ~Vector() noexcept {
    if (data_) {
      delete[] data_;
      data_ = nullptr;
    }
    size_ = 0;
    capacity_ = 0;
  }
  reference at(size_type pos) {
    if (pos >= size_) {
      throw std::out_of_range("Index out of range");
    }
    return data_[pos];
  }
  reference operator[](size_type pos) { return data_[pos]; }
  const_reference front() {
    if (size_ == 0) {
      throw std::out_of_range("Vector is empty");
    }
    return data_[0];
  }
  const_reference back() {
    if (size_ == 0) {
      throw std::out_of_range("Vector is empty");
    }
    return data_[size_ - 1];
  }
  value_type *data() const { return data_; }

  iterator begin() { return iterator(data_); }
  iterator end() { return iterator(data_ + size_); };
  const_iterator cbegin() { return const_iterator(data_); }
  const_iterator cend() { return const_iterator(data_ + size_); }

  bool empty() { return size_ == 0; }
  size_type size() const { return size_; }
  size_type max_size() const { return std::numeric_limits<size_type>::max(); }
  void reserve(size_type size) {
    if (size > capacity()) {
      value_type *new_data = new value_type[size];
      std::copy(data_, data_ + size_, new_data);
      delete[] data_;
      data_ = new_data;
      capacity_ = size;
    }
  }
  size_type capacity() const { return capacity_; }
  void shrink_to_fit() {
    if (size_ < capacity_) {
      value_type *new_data = new value_type[size_];
      for (size_type i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
      }
      delete[] data_;
      data_ = new_data;
      capacity_ = size_;
    }
  }
  void clear() {
    if (data_) {
      delete[] data_;
    }
    data_ = nullptr;
    size_ = 0;
  }
  iterator insert(iterator pos, const_reference value) {
    if (pos < begin() || pos > end()) {
      throw std::out_of_range("Out Of Range");
    }
    size_t n = pos - begin();
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    iterator new_pos = begin() + n;
    for (iterator it = end(); it != new_pos; --it) {
      *it = *(it - 1);
    }
    *new_pos = value;
    ++size_;
    return new_pos;
  }
  void erase(iterator pos) {
    if (pos < begin() || pos > end()) {
      throw std::out_of_range("Out Of Range");
    }

    for (iterator it = pos; it != end() - 1; ++it) {
      *it = *(it + 1);
    }
    --size_;
  }
  void push_back(const_reference value) {
    if (size_ == capacity_) {
      if (capacity_ == 0) {
        reserve(1);
      } else {
        reserve(capacity_ * 2);
      }
    }
    data_[size_] = value;
    size_++;
  }
  void pop_back() {
    if (size_ > 0) {
      size_--;
    }
  }
  void swap(Vector &other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
  }
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args) {
    Vector<value_type> temp_vector = {std::forward<Args>(args)...};
    iterator cur_pos = begin() + (pos - cbegin());

    size_type num_insertions = temp_vector.size();
    size_type cur_index = 0;
    while (cur_index < num_insertions) {
      cur_pos = insert(cur_pos, temp_vector[cur_index]);
      ++cur_pos;
      ++cur_index;
    }

    return --cur_pos;
  }

  template <typename... Args>
  void insert_many_back(Args &&...args) {
    insert_many(cend(), args...);
  }

 private:
  size_type size_;
  size_type capacity_;
  value_type *data_;
};

template <class T>
class Vector<T>::VectorIterator {
 public:
  VectorIterator() : ptr_(nullptr){};
  VectorIterator(iterator_pointer p) : ptr_(p){};
  reference operator*() { return *ptr_; };
  VectorIterator &operator++() {
    ++ptr_;
    return *this;
  };
  VectorIterator &operator--() {
    --ptr_;
    return *this;
  }
  VectorIterator operator++(int) {
    VectorIterator temp(*this);
    ++ptr_;
    return temp;
  }
  VectorIterator operator--(int) {
    VectorIterator temp(*this);
    --ptr_;
    return temp;
  }
  bool operator==(const VectorIterator &other) const {
    return ptr_ == other.ptr_;
  }
  bool operator!=(const VectorIterator &other) const {
    return ptr_ != other.ptr_;
  }
  VectorIterator operator+(int n) const {
    VectorIterator temp(*this);
    temp.ptr_ += n;
    return temp;
  }
  VectorIterator operator-(int n) const {
    VectorIterator temp(*this);
    temp.ptr_ -= n;
    return temp;
  }
  ptrdiff_t operator-(const VectorIterator &other) const {
    return ptr_ - other.ptr_;
  };
  bool operator>(const VectorIterator &other) const {
    return ptr_ - other.ptr_ > 0;
  };
  bool operator<(const VectorIterator &other) const {
    return ptr_ - other.ptr_ < 0;
  };

 private:
  iterator_pointer ptr_;
};
template <class T>
class Vector<T>::VectorConstIterator {
 public:
  VectorConstIterator() : ptr_(nullptr){};
  VectorConstIterator(const_iterator_pointer p) : ptr_(p){};
  const_reference operator*() const { return *ptr_; }
  VectorConstIterator &operator++() {
    ++ptr_;
    return *this;
  }
  VectorConstIterator &operator--() {
    --ptr_;
    return *this;
  }
  VectorConstIterator operator++(int) {
    VectorConstIterator temp(*this);
    ++ptr_;
    return temp;
  }
  VectorConstIterator operator--(int) {
    VectorConstIterator temp(*this);
    --ptr_;
    return temp;
  }
  bool operator==(const VectorConstIterator &other) const {
    return ptr_ == other.ptr_;
  };
  bool operator!=(const VectorConstIterator &other) const {
    return ptr_ != other.ptr_;
  };
  VectorConstIterator operator+(int n) const {
    VectorConstIterator temp(*this);
    if (n > 0) {
      for (int i = 0; i < n; ++i) {
        ++temp;
      }
    } else if (n < 0) {
      for (int i = 0; i > n; --i) {
        --temp;
      }
    }
    return temp;
  };
  VectorConstIterator operator-(int n) const {
    VectorConstIterator temp(*this);
    if (n > 0) {
      for (int i = 0; i < n; ++i) {
        --temp;
      }
    } else if (n < 0) {
      for (int i = 0; i > n; --i) {
        ++temp;
      }
    }
    return temp;
  };
  ptrdiff_t operator-(const VectorConstIterator &other) const {
    return ptr_ - other.ptr_;
  };
  bool operator>(const VectorConstIterator &other) const {
    return ptr_ - other.ptr_ > 0;
  };
  bool operator<(const VectorConstIterator &other) const {
    return ptr_ - other.ptr_ < 0;
  };

 private:
  const_iterator_pointer ptr_;
};
}  // namespace s21
#endif  // CPP2_S21_CONTAINERS_1_SRC_S21_VECTOR_H_