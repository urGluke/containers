#pragma once
#include "s21_stack.h"
namespace s21 {
template <class T>
class queue : public stack<T> {
  using value_type = T;
  using reference = T &;
  using cons_reference = const T &;
  using size_type = std::size_t;

 public:
  queue() noexcept : stack<value_type>() {}
  explicit queue(std::initializer_list<value_type> const &items) noexcept
      : stack<value_type>(items) {}
  queue(const queue &other) noexcept : stack<value_type>(other) {}
  queue(queue &&other) noexcept : stack<value_type>(other) {}
  ~queue() noexcept {}

  queue operator=(queue &&s) noexcept {
    this->size_ = s.size_;
    delete[] this->data_;
    this->data_ = s.data_;
    s.size_ = 0;
    s.data_ = nullptr;
  }

  bool operator==(const queue &other) = delete;
  bool operator!=(const queue &other) = delete;
  bool operator>(const queue &other) = delete;
  bool operator<(const queue &other) = delete;
  bool operator>=(const queue &other) = delete;
  bool operator<=(const queue &other) = delete;
  reference operator[](size_type) = delete;

  cons_reference back() noexcept { return this->data_[this->size_ - 1]; }
  cons_reference front() noexcept { return this->data_[0]; }
  cons_reference top() = delete;

  void pop() {
    if (this->size_ == 0) {
      throw std::out_of_range("stack is empty");
    }
    this->size_--;
    value_type *buff = new value_type[this->size_];
    for (size_t i = 1; i < this->size_ + 1; i++) buff[i - 1] = this->data_[i];
    delete[] this->data_;
    this->data_ = buff;
  }
};
}  // namespace s21