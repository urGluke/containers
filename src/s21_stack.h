#pragma once
#include <cassert>
namespace s21 {
template <class T>
class stack {
  using value_type = T;
  using reference = T &;
  using cons_reference = const T &;
  using size_type = std::size_t;

 public:
  stack() noexcept : size_(0), data_(nullptr) {}
  explicit stack(std::initializer_list<value_type> const &items) noexcept {
    size_ = items.size();
    if (data_ != nullptr) delete[] data_;
    data_ = new value_type[size_];
    for (size_t i = 0; i < size_; i++) {
      data_[i] = *(items.begin() + i);
    }
  }
  stack(const stack &other) noexcept {
    size_ = other.size_;
    if (data_ != nullptr) delete[] data_;
    data_ = new value_type[size_];
    for (size_t i = 0; i < size_; i++) {
      data_[i] = other.data_[i];
    }
  }
  stack(stack &&other) noexcept {
    size_ = other.size_;
    if (data_ != nullptr) delete[] data_;
    data_ = other.data_;
    other.size_ = 0;
    other.data_ = nullptr;
  }
  ~stack() noexcept {
    if (data_ != nullptr) delete[] data_;
  }

  stack operator=(stack &&s) noexcept {
    size_ = s.size_;
    delete[] data_;
    data_ = s.data_;
    s.size_ = 0;
    s.data_ = nullptr;
  }

  bool operator==(const stack &other) = delete;
  bool operator!=(const stack &other) = delete;
  bool operator>(const stack &other) = delete;
  bool operator<(const stack &other) = delete;
  bool operator>=(const stack &other) = delete;
  bool operator<=(const stack &other) = delete;
  reference operator[](size_type) = delete;

  cons_reference top() noexcept { return data_[size_ - 1]; }
  bool empty() noexcept { return size_ == 0; }
  size_type size() noexcept { return size_; }
  void push(cons_reference value) {
    size_++;
    value_type *buff;
    try {
      buff = new value_type[size_];
    } catch (...) {
      throw std::bad_alloc();
    }
    for (size_t i = 0; i < size_ - 1; i++) buff[i] = data_[i];
    buff[size_ - 1] = value;
    delete[] data_;
    data_ = buff;
  }
  void pop() {
    if (size_ == 0) {
      throw std::out_of_range("stack is empty");
    }
    size_--;
    value_type *buff = new value_type[size_];
    for (size_t i = 0; i < size_; i++) buff[i] = data_[i];
    delete[] data_;
    data_ = buff;
  }
  void swap(stack &other) noexcept {
    size_type sbuff = other.size_;
    value_type *pbuff = other.data_;
    other.size_ = size_;
    other.data_ = data_;
    size_ = sbuff;
    data_ = pbuff;
  }

 protected:
  size_type size_;
  value_type *data_ = nullptr;
};
}  // namespace s21